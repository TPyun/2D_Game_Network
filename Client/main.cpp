//#include <winsock2.h> // ����2 ���� ���
//#include <ws2tcpip.h> // ����2 Ȯ�� ���
#include "game.h"
#include "client.h"
#include <sstream>


#include <crtdbg.h>

#if _DEBUG
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#define malloc(s) _malloc_dbg(s,_NORMAL_BLOCK,__FILE__,__LINE__)
#endif


#define FPS 120
#define SERVERPORT 9000
#define BUFSIZE    1024

using namespace std;
//char* SERVERIP = (char*)"127.0.0.1";
//Game game;
//���� �ۼ��ſ� ������


DWORD WINAPI server_thread(LPVOID arg)
{
	// ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	// ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit("socket()");

	}
	
	game.p1_shoot_check = true;
	game.p2_shoot_check = true;
	
	while (!game.done) {
		if (game.curr_state == 0) {				// 0:menu
			if (game.connect_server && !game.server_connected) {
				string str(game.Port);
				int i = 0;
				stringstream ssInt(str);
				ssInt >> i;

				// connect()
				struct sockaddr_in serveraddr;
				memset(&serveraddr, 0, sizeof(serveraddr));
				serveraddr.sin_family = AF_INET;
				inet_pton(AF_INET, game.IPAdress, &serveraddr.sin_addr);
				serveraddr.sin_port = htons(i);
				retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
				if (retval == SOCKET_ERROR) {
					//err_quit("connect()");
					sock = socket_init(sock);
				}
				else {
					game.server_connected = true; //�ѹ��� connect�ϰԲ�
					cout << "\n������ �����" << endl;
				}

				// �÷��̾� �̸� ������
				send_name(sock, &game);
			}
			
			if (game.find_match) {
				//find_match ������, 
				retval = send(sock, (char*)&game.find_match, sizeof(bool), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					sock = socket_init(sock);

				}
				game.find_match = false; //true�ΰ� �ް� false�� �ٲ���
				cout << "\nfind_match ����" << endl;

				//recv player_info
				retval = recv(sock, (char*)&game.player_info, sizeof(PI), MSG_WAITALL);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					sock = socket_init(sock);
					
				}
				else if (retval == 0) {
					//����ó��
				}

				cout << "\nplayer_info ���� ����" << endl;
				for (int i = 0; i < 3; ++i){
					cout << "�÷��̾� " << i << " �̸�: " << game.player_info.name[i] << " ��: " << game.player_info.player_color[i] << endl;
				}

				//created_object ����
				recv_created_object(sock);
				
				retval = recv(sock, (char*)&game.player_list, sizeof(PS) * 3, MSG_WAITALL);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					sock = socket_init(sock);

				}
				cout << "recv first player state" << endl;
				cout << game.player_list[0].game_state << endl;
				cout << game.player_list[1].game_state << endl;
				cout << game.player_list[2].game_state << endl;
				

				game.weapon_type = 0;
				game.curr_state = 1;
			}
		}
		else if(game.curr_state == 1) {			// 1:ingame
			//�̺�Ʈ ����
			retval = send_event(sock);
			if (retval == SOCKET_ERROR) {
				//����ó��
				sock = socket_init(sock);

			}
			//�̺�Ʈ �ޱ�
			retval = recv_event(sock);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				//����ó��
				sock = socket_init(sock);
				game.curr_state = 0;

			}
			for (int i = 0; i < MAXITEM; i++) {
				for (int j = 0; j < 3; ++j) {
					if (game.created_objects[i].object_position.x == game.player_list[j].object_position.x
						&& game.created_objects[i].object_position.y == game.player_list[j].object_position.y)
					{
						game.created_objects[i].object_type == -1;
						game.created_objects[i].object_position.x = -9999;
						game.created_objects[i].object_position.y = -9999;
					}
				}
			}
			//cout << game.player_list[0].player_position.x << " / " << game.player_list[0].player_position.y << endl;
			game.MyCharPos = game.player_list[0].player_position;

			if (game.player_list[0].game_state > 3) {
				sock = socket_init(sock);
				game.curr_state = 2;
			}
		}
		else if (game.curr_state == 2) {			//����
		}
	}
	return 0;
}

int SDL_main(int argc, char* argv[])
{
	cout << "Start" << endl;

	int startTime, endTime;

	//game ȭ�� ���� ��ȯ (�ӽ�)
	game.curr_state = 0;

	HANDLE h_thread;
	h_thread = CreateThread(NULL, 0, server_thread,
		(LPVOID)0, 0, NULL);
	while (!game.done){
		startTime = clock();
		game.update();

		endTime = clock();
		game.delayTime = 1000 / FPS - (endTime - startTime);
		if (game.delayTime > 0) {
			Sleep(game.delayTime);
		}
		//cout << game.delayTime << endl;

	}
	//����
	cout << "End\n" << endl;
	
	return 0;

}
