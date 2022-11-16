#include "Common.h"
#include <iostream>
#include <fstream>
#include<vector>
#include<mutex>
#include<string>
#include<map>

#define SERVERPORT	9000
#define BUFSIZE		4096
#define NAMESIZE	20

using namespace std;

typedef struct TWO_Floats {
	float x;
	float y;
}TF;

typedef struct TWO_Ints {
	int x;
	int y;
}TI;

typedef struct players_state {
	int hp;
	int gun_type;
	int bullet[3];
	TI object_position;
	TF player_position;
	bool gun_fired;
	int game_state;		// 0:main, 1:find_match, 2:in_game, 3:lose, 4:win
}PS;

typedef struct players_info{
	int player_color[3];
	char name[3][20];
}PI;

int len = 0;
char buffer[BUFSIZE]; // ���� ���� ������
//std::mutex mylock;
//mutex find_lock;
//���� ����
int hostnum;

//client ������ ����
map<char*, char*> client_thread_list;
map<char*, bool> find_match_list;

//�Լ� ����
DWORD WINAPI matching_thread(LPVOID arg);
DWORD WINAPI process_client(LPVOID arg);
DWORD WINAPI ingame_thread(LPVOID arg);
bool find_match_3p();

//��Ī������ 
DWORD WINAPI matching_thread(LPVOID arg)
{
	//3player�� find_match�� �������� Ȯ��
	while (1) {
		if (!find_match_3p()){
			cout << "��. �ƴϾ�." << endl;
			continue;
		}
		else if (find_match_3p()) {
			//ingame_thread����
			HANDLE  ingamethread;
			ingamethread = CreateThread(NULL, 0, ingame_thread,
				0, 0, NULL);
		}
	}
}
// �ΰ��Ӿ�����
DWORD WINAPI ingame_thread(LPVOID arg)
{

	cout << "����� �ΰ����̴�. ���������̵��." << endl;
	return 0;
}

//find_match_3p �Լ�
bool find_match_3p()
{
	int cnt = 0;
	for (auto& a : find_match_list){
		if (a.second == true)
			cnt++;
	}
	if (cnt >= 1){
		return true;
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
	char buf[BUFSIZE + 1];
	char name_buf[NAMESIZE + 1];
	bool find_match;
	PS player_state;

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		addr, ntohs(clientaddr.sin_port));
	
	// �̸� �ޱ�
	retval = recv(client_sock, name_buf, NAMESIZE, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		//����ó��
	}
	else if (retval == 0) {
		//����ó��
	}
	// �̸� ���
	name_buf[retval] = '\0';
	cout << "name : " << name_buf << endl;

	//client_thread_list�� Ŭ���̾�Ʈ �߰�
	client_thread_list[addr] = name_buf;
	
	player_state.game_state = 0;
	while (true) {
		if (player_state.game_state == 0) {				// 0:main
			//recv find_match
			retval = recv(client_sock, (char*)&find_match, sizeof(bool), MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				//����ó��
			}
			else if (retval == 0) {
				//����ó��
			}

			//Print find_match
			if (find_match) {
				cout << "Find mathch!!!!" << endl;
				//���⼭ ��ġ ã�� ��� ������ ��
				find_match_list[addr] = find_match;
				player_state.game_state = 1;	//findmath�մϴ�
			}
		}
		else if (player_state.game_state == 1) {		// 1:find_match
			
			PI player_info;
			//player_info�� ���� �־������
			/*strcpy(player_info.name[0], "name1");
			strcpy(player_info.name[1], "name2");
			strcpy(player_info.name[2], "name3");
			player_info.player_color[0] = 0;
			player_info.player_color[1] = 1;
			player_info.player_color[2] = 2;
			
			cout << player_info.name[0] << endl;
			cout << player_info.player_color[0] << endl;
			cout << player_info.name[1] << endl;
			cout << player_info.player_color[1] << endl;
			cout << player_info.name[2] << endl;
			cout << player_info.player_color[2] << endl;*/

			retval = send(client_sock, (char*)&player_info, sizeof(PI), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}		
		else if (player_state.game_state == 2) {		// 2:in_game

		}
		else if (player_state.game_state == 3) {		// 3:lose

		}
		else if (player_state.game_state == 4) {		// 4:win

		}
	}

	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		addr, ntohs(clientaddr.sin_port));
	// ���� �ݱ�
	closesocket(client_sock);

	return 0;
}

int main(int argc, char* argv[])
{
	int retval;
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
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

	//��Ī ������ ����
	HANDLE  matchingthread;
	matchingthread = CreateThread(NULL, 0, matching_thread,
		0, 0, NULL);


	// ������ ��ſ� ����� ����
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

		// ������ Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

		cout << addr << endl;

		// ������ ����
		client_thread = CreateThread(NULL, 0, process_client,
			(LPVOID)client_sock, 0, NULL);
		//���� �ݱ�
		if (client_thread == NULL) { closesocket(client_sock); }
		else { CloseHandle(client_thread); }

	}

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}
