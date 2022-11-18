#include "Common.h"
#include "global.h"
#include "ingame.h"
#include <iostream>
#include <fstream>
#include<vector>
#include<mutex>
#include<string>
#include<map>

using namespace std;

#define MAX_CLIENT_IN_ROOM 1

int len = 0;
char buffer[BUFSIZE]; // 가변 길이 데이터
//std::mutex mylock;
//mutex find_lock;
//들어온 순서
int hostnum;

//client 스레드 관리
map<char*, char*> client_thread_list;
map<char*, PP*> player_list;

//함수 선언
DWORD WINAPI matching_thread(LPVOID arg);
DWORD WINAPI process_client(LPVOID arg);
DWORD WINAPI ingame_thread(LPVOID arg);
bool find_match_3p(int);

//매칭쓰레드 
DWORD WINAPI matching_thread(LPVOID arg)
{
	//ingame_room map함수 관리
	int room_num = 0;

	//3player가 find_match를 눌렀는지 확인
	while (1) {
		if (!find_match_3p(room_num)){
			cout << "\rmatching";
			continue;
		}
		else {
			//ingame_thread생성
			
			HANDLE  ingamethread;
			ingamethread = CreateThread(NULL, 0, ingame_thread,(LPVOID)room_num, 0, NULL);
			room_num++;
		}
	}
}
// 인게임쓰레드
ingame create_map;
DWORD WINAPI ingame_thread(LPVOID arg)
{
	create_map.create_object();
	cout << (int)arg <<" : 게임을 시작하지." << endl;
	
	return 0;
}

//find_match_3p 함수
bool find_match_3p(int room_num)
{
	int cnt = 0;
	for (auto& a : player_list){
		if (a.second->player_state.game_state == 1)
			cnt++;
	}
	if (cnt >= MAX_CLIENT_IN_ROOM){
		for (auto& a : player_list) {
			if (a.second->player_state.game_state == 1){
				a.second->player_state.game_state = 2;
				
				a.second->room_num = room_num;	//PP에 room_num을 넣어줌
			}
			return true;
		}
	}
	else {
		return false;
	}
}

DWORD WINAPI process_client(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char name_buf[NAMESIZE + 1];
	bool find_match;
	PP player_profile;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));
	
	// 이름 받기
	retval = recv(client_sock, name_buf, NAMESIZE, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		//예외처리
	}
	else if (retval == 0) {
		//예외처리
	}
	// 이름 출력
	name_buf[retval] = '\0';
	cout << "name : " << name_buf << endl;
	strcpy(player_profile.player_info.name[0], name_buf);
		
	//client_thread_;list에 클라이언트 추가
	client_thread_list[addr] = name_buf;
	
	player_profile.player_state.game_state = 0;
	while (true) {
		if (player_profile.player_state.game_state == 0) {				// 0:main
			//recv find_match
			retval = recv(client_sock, (char*)&find_match, sizeof(bool), MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				//예외처리
			}
			else if (retval == 0) {
				//예외처리
			}

			//Print find_match
			if (find_match) {
				cout << "Find mathch!!!!" << endl;
				//여기서 매치 찾는 기능 넣으면 됨
				player_list[addr] = &player_profile;
				player_profile.player_state.game_state = 1;	//findmath합니다
			}
		}
		else if (player_profile.player_state.game_state == 1) {		// 1:find_match

			cout << (char*)player_profile.player_info.name << "클라이언트 방 번호: " << player_profile.room_num << endl;

			int other_player_num = 0;
			for (auto& player : player_list) {
				if (player.second->room_num == player_profile.room_num)
					player.second->player_info.player_color[other_player_num] = other_player_num;
					if (player.second->player_info.name != player_profile.player_info.name) {		//본인은 [0]에 있으니 제외
						strcpy(player_profile.player_info.name[other_player_num + 1], (char*)player.second->player_info.name);
						other_player_num++;
					}
				if (other_player_num == MAX_CLIENT_IN_ROOM - 1) {
					break;
				}
			}
			
			retval = send(client_sock, (char*)&player_profile.player_info, sizeof(PI), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}		
		else if (player_profile.player_state.game_state == 2) {		// 2:in_game
			static int first_send = true;

			//초기 데이터를 보냈는지 확인
			if (first_send)
			{	
				//created_object 송신
				retval = send(client_sock, (char*)&create_map.objects, sizeof(create_map.objects), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				//player_state 송신
				retval = send(client_sock, (char*)&player_profile.player_state, sizeof(PS), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				first_send = false;
			}
			else{
				//게임 내에서 계속 player_state 전송
				retval = send(client_sock, (char*)&player_profile.player_state, sizeof(PS), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
			}
		}
		else if (player_profile.player_state.game_state == 3) {		// 3:lose

		}
		else if (player_profile.player_state.game_state == 4) {		// 4:win

		}
	}

	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",addr, ntohs(clientaddr.sin_port));
	//map에서 플레이어 제거
	player_list.erase(addr);
	client_thread_list.erase(addr);
	
	// 소켓 닫기
	closesocket(client_sock);

	return 0;
}

int main(int argc, char* argv[])
{
	int retval;
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	//매칭 쓰레드 생성
	HANDLE  matchingthread;
	matchingthread = CreateThread(NULL, 0, matching_thread,
		0, 0, NULL);

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE client_thread;

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

		cout << addr << endl;

		// 스레드 생성
		client_thread = CreateThread(NULL, 0, process_client,
			(LPVOID)client_sock, 0, NULL);
		//소켓 닫기
		if (client_thread == NULL) { closesocket(client_sock); }
		else { CloseHandle(client_thread); }

	}

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
