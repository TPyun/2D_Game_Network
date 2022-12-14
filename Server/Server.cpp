#include "Common.h"
#include "global.h"
#include "ingame.h"
#include "matching.h"
#include"tbb/spin_rw_mutex.h"
#define FPS 144
using namespace std;

int len = 0;
char buffer[BUFSIZE]; // 가변 길이 데이터
int hostnum;

map<int,Ingame> ingames;
Matching matching;
tbb::spin_rw_mutex p_lock;
map<int, PP*> player_list;
PP null_temp;

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
		p_lock.lock_shared();
		if (!matching.find_match_3p(room_num)){
			Sleep(100);
			p_lock.unlock_shared();
			continue;
		}
		else {
			//ingame_thread생성
			p_lock.unlock_shared();
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
	int cnt = 0;
	bool bullet_cnt[MAX_CLIENT_IN_ROOM] = {};
	for (int i = 0; i < MAX_CLIENT_IN_ROOM; ++i)
	{
		bullet_cnt[i] = false;
	}
	p_lock.lock_shared();
	for (auto& player : player_list) {
		if (player.second->room_num == room_num) {
			ingames[room_num].init_player(player.second);
			ingames[room_num].rand_player_pos(player.second);
		}

	}
	p_lock.unlock_shared();
	while (1) {
		//cout << "서버 전체 인원 수: "<< player_list.size() << endl;
		//cout << "인게임 도는중 방번호: " << room_num << endl;
		int connected_players_inroom{0};
		int player_alive{ 0 };
		start = clock();
		p_lock.lock_shared();
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
				player.second->player_state.gun_type = player.second->input.gun_type;
				CI local_input = player.second->input;
				if (player.second->player_state.hp > 0) {
					player_alive++;
					ingames[room_num].bullet_movement(player.second->input.clicked_mouse_rotation, player.second);
					ingames[room_num].collide_check(player.second, &local_input);
					if (player.second->input.clicked == true)
					{
						if (bullet_cnt[cnt] == false)
						{
							cout << cnt << endl;
							player.second->player_state.bullet[player.second->player_state.gun_type]--;
							bullet_cnt[cnt] = true;
						}
					}
					if (player.second->input.clicked == false)
					{
						bullet_cnt[cnt] = false;
					}
					cnt++;
				}
				ingames[room_num].character_movement(local_input, player.second->player_state.player_position, player.second->player_state.velo);
			}
		}
		p_lock.unlock_shared();
		if (player_alive == 1 && MAX_CLIENT_IN_ROOM != 1) {
			p_lock.lock_shared();
			for (auto& player : player_list) {
				if (player.second->room_num == room_num) {
					if (player.second->player_state.hp > 0) {
						player.second->player_state.game_state = 5;
					}
					else {
						player.second->player_state.game_state = 4;
					}
				}
			}
			p_lock.unlock_shared();
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
		cnt = 0;
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
				p_lock.lock();
				player_list[port] = &player_profile;
				p_lock.unlock();
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
			p_lock.lock_shared();
			for (auto& player : player_list) {
				cout << "존재하는 포트번호 " << (int)player.first << endl;

				if (player.second->room_num == player_profile.room_num) {//같은 방에 있는 사람
					if (player.second->player_info.name[0] != player_profile.player_info.name[0]) {		//본인은 [0]에 있으니 제외
						strcpy(player_profile.player_info.name[other_player_num + 1], (char*)player.second->player_info.name);
						other_player_num++;
					}
					if (other_player_num == MAX_CLIENT_IN_ROOM - 1) {
						break;
					}
				}	
				
			}
			p_lock.unlock_shared();


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
			p_lock.lock_shared();
			for (auto& player : player_list) {
				//같은 방에 걸린 플레이어 정보 가져옴
				if (player.second->room_num == player_profile.room_num) {
					if (player.second->player_info.name[0] == player_profile.player_info.name[0]) {
						local_player_list[0] = player.second->player_state;
					}
					else {
						local_player_list[cnt++] = player.second->player_state;
					}
				}
			}
			p_lock.unlock_shared();

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
			player_profile.unconditinal_fired_pos = player_profile.input.unconditional_fired_pos_input;

			if (player_profile.player_state.gun_fired == true && bullet_check == true) {
				player_profile.player_state.player_rotation = player_profile.input.mouse_rotation;
				cout << player_profile.player_state.player_rotation << endl;
				player_profile.bullet_pos = player_profile.unconditinal_fired_pos;
				cout << player_profile.bullet_pos.x << "\t" << player_profile.bullet_pos.y << endl;
				cout << "총알 위치 x: " << player_profile.unconditinal_fired_pos.x << "\ty : " <<
					player_profile.unconditinal_fired_pos.y << endl;

				cout << "플레이어 rotation : " << player_profile.player_state.player_rotation << endl;
				bullet_check = false;
			}
			else if (player_profile.player_state.gun_fired == false && bullet_check == false) {
				bullet_check = true;
				player_profile.player_state.collide = false;
			}
			//cout << player_profile.player_state.player_rotation << endl;


			//local에 현재 pp에 있는 ps 넣어주기
			local_player_list[0] = player_profile.player_state;
			int i = 1;
			p_lock.lock_shared();
			for (auto& player : player_list) {
				
				if (player.second->room_num == (int)player_profile.room_num && player.second->player_info.name[0] != player_profile.player_info.name[0]) {
					local_player_list[i] = player.second->player_state;
					++i;
				}
			}
			p_lock.unlock_shared();

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




		//4:lose 5: win ===============================================================================================
		else if (player_profile.player_state.game_state == 4 || player_profile.player_state.game_state == 5) {

			// 키입력 받기
			retval = recv(client_sock, (char*)&player_profile.input, sizeof(CI), MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				//예외처리
			}

			//local에 현재 pp에 있는 ps 넣어주기
			local_player_list[0] = player_profile.player_state;
			int i = 1;
			p_lock.lock_shared();
			for (auto& player : player_list) {
				
				if (player.second->room_num == (int)player_profile.room_num && player.second->player_info.name[0] != player_profile.player_info.name[0]) {
					local_player_list[i] = player.second->player_state;
					++i;
				}
			}
			p_lock.unlock_shared();

			retval = send(client_sock, (char*)&local_player_list, sizeof(PS) * 3, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
	}
	player_profile.player_state.hp = 0;
	while (1) {
		if (ingames.find(player_profile.room_num) == ingames.end())
		{
			p_lock.lock();
			player_list.erase(port);
			p_lock.unlock();
			break;
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
	