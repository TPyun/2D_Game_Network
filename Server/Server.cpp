#include "Common.h"
#include "global.h"
#include "ingame.h"

#define MAX_CLIENT_IN_ROOM 3
using namespace std;

int len = 0;
char buffer[BUFSIZE]; // 가변 길이 데이터
//std::mutex mylock;
//mutex find_lock;
//들어온 순서
int hostnum;

map<int, char*> client_thread_list;
map<int, PP*> player_list;

//함수 선언
DWORD WINAPI matching_thread(LPVOID arg);
DWORD WINAPI process_client(LPVOID arg);
DWORD WINAPI ingame_thread(LPVOID arg);
bool find_match_3p(int);
void collider_checker(CI*, PP*);

//매칭쓰레드 
DWORD WINAPI matching_thread(LPVOID arg)
{
	//ingame_room map함수 관리
	int room_num = 1;

	//3player가 find_match를 눌렀는지 확인
	while (1) {
		if (!find_match_3p(room_num)){
			//cout << "\r";
			Sleep(100);
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
Ingame ingame;
DWORD WINAPI ingame_thread(LPVOID room_num)
{
	cout << (int)room_num <<" : 게임을 시작하지." << endl;
	ingame.create_object();
	int i = 0;
	for (auto& player : player_list) {
		/*if (player.second->room_num == (int)room_num) {
			if (i == 0) {
				player.second->player_state.player_position.x = 0;
				player.second->player_state.player_position.y = -600;
			}
			else if (i == 1) {
				player.second->player_state.player_position.x = -600;
				player.second->player_state.player_position.y = 200;
			}
			else if (i == 2) {
				player.second->player_state.player_position.x = 600;
				player.second->player_state.player_position.y = 200;
			}
			++i;
		}*/
	}
	
	while (1) {
		for (auto& player : player_list) {
			if (player.second->room_num == (int)room_num) {
				CI local_input = player.second->input;
				collider_checker(&local_input,player.second);
				ingame.character_movement(local_input, player.second->player_state.player_position);
				//cout << player.second->player_state.player_position.x << " " << player.second->player_state.player_position.y << endl;
				Sleep(10);	//조절해야함
			}
		}
	}
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

				cout << "플레이어 이름: " << *a.second->player_info.name << endl;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

void collider_checker(CI* local_input, PP* player_collider) {


	for (auto& obj : ingame.objects)
	{
		if (abs(obj.object_position.x - player_collider->player_state.player_position.x) < 100
			&& abs(obj.object_position.y - player_collider->player_state.player_position.y) < 100)
		{
			//돌
			if (obj.object_type == 0)
			{
				if(obj.object_position.x - player_collider->player_state.player_position.x < 53 &&
					obj.object_position.x - player_collider->player_state.player_position.x > -28)
				{
					if (obj.object_position.y - player_collider->player_state.player_position.y < 62
						&& obj.object_position.y - player_collider->player_state.player_position.y > 0)
						local_input->s_Pressed = false;
					if (obj.object_position.y - player_collider->player_state.player_position.y > -35
						&& obj.object_position.y - player_collider->player_state.player_position.y < 0)
						local_input->w_Pressed = false;
				}
				if (obj.object_position.y - player_collider->player_state.player_position.y < 53 &&
					obj.object_position.y - player_collider->player_state.player_position.y > -28)
				{
					if (obj.object_position.x - player_collider->player_state.player_position.x < 62
						&& obj.object_position.x - player_collider->player_state.player_position.x > 0)
						local_input->d_Pressed = false;
					if (obj.object_position.x - player_collider->player_state.player_position.x > -37
						&& obj.object_position.x - player_collider->player_state.player_position.x < 0)
						local_input->a_Pressed = false;
				}
			}
			//벽1
			if (obj.object_type == 1)
			{
				//cout << obj.object_position.x << " / " << obj.object_position.y << endl;
				if (obj.object_position.x - player_collider->player_state.player_position.x < 64 &&
					obj.object_position.x - player_collider->player_state.player_position.x > 8)
				{
					if (obj.object_position.y - player_collider->player_state.player_position.y < 72
						&& obj.object_position.y - player_collider->player_state.player_position.y > 0)
						local_input->s_Pressed = false;
					if (obj.object_position.y - player_collider->player_state.player_position.y > -72
						&& obj.object_position.y - player_collider->player_state.player_position.y < 0)
						local_input->w_Pressed = false;
				}
				if (obj.object_position.y - player_collider->player_state.player_position.y < 64 &&
					obj.object_position.y - player_collider->player_state.player_position.y > -64)
				{
					if (obj.object_position.x - player_collider->player_state.player_position.x < 72
						&& obj.object_position.x - player_collider->player_state.player_position.x > 30)
						local_input->d_Pressed = false;
					if (obj.object_position.x - player_collider->player_state.player_position.x > 5
						&& obj.object_position.x - player_collider->player_state.player_position.x < 20)
						local_input->a_Pressed = false;
				}
			}
			//벽2
			if (obj.object_type == 2)
			{
				if (obj.object_position.y - player_collider->player_state.player_position.y < 64 &&
					obj.object_position.y - player_collider->player_state.player_position.y > 8)
				{
					if (obj.object_position.x - player_collider->player_state.player_position.x < 72
						&& obj.object_position.x - player_collider->player_state.player_position.x > 0)
						local_input->d_Pressed = false;
					if (obj.object_position.x - player_collider->player_state.player_position.x > -72
						&& obj.object_position.x - player_collider->player_state.player_position.x < 0)
						local_input->a_Pressed = false;
				}
				if (obj.object_position.x - player_collider->player_state.player_position.x < 64 &&
					obj.object_position.x - player_collider->player_state.player_position.x > -64)
				{
					if (obj.object_position.y - player_collider->player_state.player_position.y < 72
						&& obj.object_position.y - player_collider->player_state.player_position.y > 30)
						local_input->s_Pressed = false;
					if (obj.object_position.y - player_collider->player_state.player_position.y > 5
						&& obj.object_position.y - player_collider->player_state.player_position.y < 20)
						local_input->w_Pressed = false;
				}
			}
		}
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
	printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",addr, ntohs(clientaddr.sin_port));
	int port = ntohs(clientaddr.sin_port);
	cout << "나의 포트번호: " << port << endl;
	
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
	client_thread_list[port] = name_buf;

	//보낼 player_list
	PS local_player_list[3];

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
				player_list[port] = &player_profile;
				player_profile.player_state.game_state = 1;	//findmath합니다
			}
		}
		else if (player_profile.player_state.game_state == 1) {		// 1:finding_match

		}
		else if (player_profile.player_state.game_state == 2) {		// 2:loading
			Sleep(500);
			cout<<"나의 포트번호: " << port << endl;
			//같은 방 사람 정보 넣기
			int other_player_num = 0;
			for (auto& player : player_list) {
				cout <<"존재하는 포트번호 " << (int)player.first << endl;
				if (player.second->room_num == player_profile.room_num)		//같은 방에 있는 사람
					player.second->player_info.player_color[other_player_num] = other_player_num;		//색깔 넣기
				if (player.second->player_info.name != player_profile.player_info.name) {		//본인은 [0]에 있으니 제외
					strcpy(player_profile.player_info.name[other_player_num + 1], (char*)player.second->player_info.name);
					other_player_num++;
				}
				if (other_player_num == MAX_CLIENT_IN_ROOM - 1) {
					break;
				}
			}
			
			//같은 방 사람 출력
			cout << endl << "같은 방에 있는 사람 목록" << endl;
			for (int i = 0; i < MAX_CLIENT_IN_ROOM; ++i) {
				cout << "이름: " << player_profile.player_info.name[i] << ", 색: " << player_profile.player_info.player_color[i];
			}
			cout << endl;

			//같은 방 사람 정보 보내기
			retval = send(client_sock, (char*)&player_profile.player_info, sizeof(PI), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			cout << "\n" << (char*)player_profile.player_info.name << "클라이언트 방 번호: " << player_profile.room_num << endl;
			
			//created_object 송신
			//완료되기전에 보내는거 방지(개선해야함)
			Sleep(100);

			retval = send(client_sock, (char*)&ingame.objects, sizeof(ingame.objects), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			cout << "보내는 byte : " << sizeof(ingame.objects) << endl;
			cout << "sent first created objects" << endl;
			for (int i = 0; i < MAXITEM; ++i) {
				cout << "보낸 obj[" << i << "] : " << ingame.objects[i].object_position.x << " / " << ingame.objects[i].object_position.y << endl;
			}

			//player_state 송신
			int cnt = 1;
			for (auto& a : player_list) {
				//같은 방에 걸린 플레이어 정보 가져옴
				if (a.second->room_num == player_profile.room_num) {
					if (a.second->player_info.name[0] == player_profile.player_info.name[0]) {
						local_player_list[0] = a.second->player_state;
					}
					else {
						local_player_list[cnt++] = a.second->player_state;
					}
				}
			}
			
			
			retval = send(client_sock, (char*)&local_player_list, sizeof(PS) * 3, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			player_profile.player_state.game_state = 3;
		}
		else if (player_profile.player_state.game_state == 3) {		// 3:ingame
			// 키입력 받기
			retval = recv(client_sock, (char*)&player_profile.input, sizeof(CI), MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				//예외처리
				break;
			}
			else if (retval == 0) {
				//예외처리
			}

			//cout << player_profile.input.clicked << "    "<< player_profile.input.mouse_rotation << endl;
			player_profile.player_state.player_rotation = player_profile.input.mouse_rotation;

			
			//local에 현재 pp에 있는 ps 넣어주기
			local_player_list[0] = player_profile.player_state;
			int i = 1;
			for (auto& player : player_list) {
				if (player.second->room_num == (int)player_profile.room_num && player.second->player_info.name[0] != player_profile.player_info.name[0]) {
					local_player_list[i] = player.second->player_state;
					++i;
				}
			}

			
			retval = send(client_sock, (char*)&local_player_list, sizeof(PS) * 3, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			//cout << local_player_list[0].player_position.x << " / " << local_player_list[0].player_position.y << endl;
		}
		else if (player_profile.player_state.game_state == 4) {		// 4:lose

		}
		else if (player_profile.player_state.game_state == 5) {		// 5:win

		}
	}
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
	//map에서 플레이어 제거
	player_list.erase(port);
	client_thread_list.erase(port);
	
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
