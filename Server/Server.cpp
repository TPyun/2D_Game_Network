#include "Common.h"
#include "global.h"
#include "ingame.h"
#include "matching.h"
#define FPS 200
using namespace std;

int len = 0;
char buffer[BUFSIZE]; // 가변 길이 데이터
//std::mutex mylock;
//mutex find_lock;
//들어온 순서
int hostnum;

map<int,Ingame> ingames;
Matching matching;

//map<int, char*> client_thread_list;
map<int, PP*> player_list;
PP null_temp{};

//함수 선언
DWORD WINAPI matching_thread(LPVOID arg);
DWORD WINAPI process_client(LPVOID arg);
DWORD WINAPI ingame_thread(LPVOID arg);

//매칭쓰레드 
DWORD WINAPI matching_thread(LPVOID arg)
{
	//ingame_room map함수 관리
	int room_num = 1;
	
	//3player가 find_match를 눌렀는지 확인
	while (1) {
		int disconnected_players_inserver{};
		
		for (auto& player : player_list) {
			if (player.second == nullptr) {
			}
			else if (player.second == &null_temp) {
				++disconnected_players_inserver;;
			}
		}
		if (disconnected_players_inserver == player_list.size() && player_list.size() != 0) {
			cout << "총 " << player_list.size() << "명의 플레이어 중 " << disconnected_players_inserver << "명이 종료하여 map 전체 삭제" << endl;
			for (auto player = player_list.cbegin(); player != player_list.cend();) {
				player_list.erase(player++);
			}
			disconnected_players_inserver = 0;
		}
		if (!matching.find_match_3p(room_num)){
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
DWORD WINAPI ingame_thread(LPVOID n)
{
	int room_num = (int)n;
	cout << room_num << " : 게임을 시작하지." << endl;
	ingames[room_num].create_object();
	
	cout << "맵생성 완료" << endl;
	int i{};
	int start{};
	int end{};
	int delay{};

	while (1) {
		//cout << "서버 전체 인원 수: "<< player_list.size() << endl;

		//cout << "인게임 도는중 방번호: " << room_num << endl;
		int connected_players_inroom{0};
		start = clock();
		for (auto& player : player_list) {
			//cout << player.first << endl;
			if (player.second == nullptr) {
				continue;
			}
			if (player.second == &null_temp) {
				continue;
			}
			if (player.second->room_num == room_num) {
				++connected_players_inroom;
				
				CI local_input = player.second->input;
				ingames[room_num].bullet_movement(player.second->input.clicked_mouse_rotation, player.second);
				ingames[room_num].collide_check(player.second, &local_input);
				ingames[room_num].character_movement(local_input, player.second->player_state.player_position);
				

				//cout << "moving" << endl;
				//cout << player.second->player_state.player_position.x << " " << player.second->player_state.player_position.y << endl;
				// ingame.collide_bullet_check(player.second, );
			}
		}
		if (MAX_CLIENT_IN_ROOM - connected_players_inroom == MAX_CLIENT_IN_ROOM) {
			cout << "모든 플레이어가 나갔습니다. " << room_num << " ingame thread를 삭제" << endl;
			return 0;
			//player_list.erase(player.first);
		}
		end = clock();
		delay = 1000 / FPS - (end - start);
		if (delay > 0) {
			Sleep(delay);
		}

	}
	return 0;
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
	PP player_profile{};
	bool bullet_check = true;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
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
	
	//보낼 player_list
	PS local_player_list[3]{};

	player_profile.player_state.game_state = 0;
	while (true) {
		if (player_profile.player_state.game_state == 0) {				// 0:main
			//recv find_match
			retval = recv(client_sock, (char*)&find_match, sizeof(bool), MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				//예외처리
				break;
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


		//1: finding_match ==================================================================================================================
		else if (player_profile.player_state.game_state == 1) {

		}



		//2: loading ==================================================================================================================
		else if (player_profile.player_state.game_state == 2) {
			Sleep(1000);
			cout << "나의 포트번호: " << port << endl;
			//같은 방 사람 정보 넣기
			int other_player_num = 0;
			for (auto& player : player_list) {
				if (player.second == &null_temp) {
					continue;
				}
				if (player.second == nullptr) {
					continue;
				}
				cout << "존재하는 포트번호 " << (int)player.first << endl;
				
				if (player.second->room_num == player_profile.room_num)		//같은 방에 있는 사람
					player.second->player_info.player_color[other_player_num] = other_player_num;		//색깔 넣기
				if (player.second->player_info.name[0] != player_profile.player_info.name[0]) {		//본인은 [0]에 있으니 제외
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
			Sleep(1000);

			retval = send(client_sock, (char*)&ingames[player_profile.room_num].objects, sizeof(ingames[player_profile.room_num].objects), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			cout << "보내는 byte : " << sizeof(ingames[player_profile.room_num].objects) << endl;
			cout << "sent first created objects" << endl;
			for (int i = 0; i < MAXITEM; ++i) {
				cout << "보낸 obj[" << i << "] : " << ingames[player_profile.room_num].objects[i].object_position.x << " / " << ingames[player_profile.room_num].objects[i].object_position.y << endl;
			}

			//player_state 송신
			int cnt = 1;
			for (auto& player : player_list) {
				//같은 방에 걸린 플레이어 정보 가져옴
				if (player.second == &null_temp) {
					continue;
				}
				if (player.second == nullptr) {
					continue;
				}
				if (player.second->room_num == player_profile.room_num) {
					if (player.second->player_info.name[0] == player_profile.player_info.name[0]) {
						local_player_list[0] = player.second->player_state;
					}
					else {
						local_player_list[cnt++] = player.second->player_state;
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



		//3:ingame ==================================================================================================================
		else if (player_profile.player_state.game_state == 3) {
			// 키입력 받기
			retval = recv(client_sock, (char*)&player_profile.input, sizeof(CI), MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				//예외처리
			}

			// 마우스 클릭 확인 출력
			player_profile.player_state.gun_fired = player_profile.input.clicked;
			player_profile.player_state.player_rotation = player_profile.input.mouse_rotation;
			player_profile.unconditinal_fired_pos = player_profile.input.uncounditional_fired_pos_input;
			
			if (player_profile.player_state.gun_fired == true && bullet_check == true) {
				player_profile.player_state.player_rotation = player_profile.input.mouse_rotation;
				cout << player_profile.player_state.player_rotation << endl;
				player_profile.player_state.bullet_pos = player_profile.unconditinal_fired_pos;
				cout << player_profile.player_state.bullet_pos.x << "\t" << player_profile.player_state.bullet_pos.y << endl;
				cout << "총알 위치 x: " << player_profile.unconditinal_fired_pos.x << "\ty : " <<
					player_profile.unconditinal_fired_pos.y << endl;

				cout << "플레이어 rotation : " << player_profile.player_state.player_rotation << endl;
				bullet_check = false;
			}
			else if (player_profile.player_state.gun_fired == false && bullet_check == false) {
				bullet_check = true;

			}
			//cout << player_profile.player_state.player_rotation << endl;


			//local에 현재 pp에 있는 ps 넣어주기
			local_player_list[0] = player_profile.player_state;
			int i = 1;
			for (auto& player : player_list) {
				if (player.second == nullptr) {
					continue;
				}
				if (player.second == &null_temp) {
					//cout << "null ingame" << endl;
					//player_list.erase(player.first);
				}
				else if (player.second->room_num == (int)player_profile.room_num && player.second->player_info.name[0] != player_profile.player_info.name[0]) {
					local_player_list[i] = player.second->player_state;
					++i;
				}
			}

			retval = send(client_sock, (char*)&local_player_list, sizeof(PS) * 3, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			/*cout << bool(player_profile.player_state.game_state == 6) << endl;
			cout << "Name: " << (char*)player_profile.player_info.name << endl;
			cout << "Room num: " << player_profile.room_num << endl;
			cout << "Coordinates: " << player_profile.player_state.player_position.x << " " << player_profile.player_state.player_position.y << endl;
			cout << "Health: " << player_profile.player_state.hp << endl;*/
		}

		


		//4:lose ==================================================================================================================
		else if (player_profile.player_state.game_state == 4) {

		}



		//5: win ==================================================================================================================
		else if (player_profile.player_state.game_state == 5) {
		}
	}
	

	for (auto& player : player_list) {
		if (player.first == port) {
			player.second = &null_temp;
		}
	}

	cout << "종료한 플레이어: " << name_buf << endl;
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", addr, port);
	//소켓 닫기
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
	