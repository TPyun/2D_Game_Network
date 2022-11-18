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
char buffer[BUFSIZE]; // ���� ���� ������
//std::mutex mylock;
//mutex find_lock;
//���� ����
int hostnum;

//client ������ ����
map<char*, char*> client_thread_list;
map<char*, PP*> player_list;

//�Լ� ����
DWORD WINAPI matching_thread(LPVOID arg);
DWORD WINAPI process_client(LPVOID arg);
DWORD WINAPI ingame_thread(LPVOID arg);
bool find_match_3p(int);

//��Ī������ 
DWORD WINAPI matching_thread(LPVOID arg)
{
	//ingame_room map�Լ� ����
	int room_num = 0;

	//3player�� find_match�� �������� Ȯ��
	while (1) {
		if (!find_match_3p(room_num)){
			cout << "\rmatching";
			continue;
		}
		else {
			//ingame_thread����
			
			HANDLE  ingamethread;
			ingamethread = CreateThread(NULL, 0, ingame_thread,(LPVOID)room_num, 0, NULL);
			room_num++;
		}
	}
}
// �ΰ��Ӿ�����
ingame create_map;
DWORD WINAPI ingame_thread(LPVOID arg)
{
	create_map.create_object();
	cout << (int)arg <<" : ������ ��������." << endl;
	
	return 0;
}

//find_match_3p �Լ�
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
				
				a.second->room_num = room_num;	//PP�� room_num�� �־���
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
	strcpy(player_profile.player_info.name[0], name_buf);
		
	//client_thread_;list�� Ŭ���̾�Ʈ �߰�
	client_thread_list[addr] = name_buf;
	
	player_profile.player_state.game_state = 0;
	while (true) {
		if (player_profile.player_state.game_state == 0) {				// 0:main
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
				player_list[addr] = &player_profile;
				player_profile.player_state.game_state = 1;	//findmath�մϴ�
			}
		}
		else if (player_profile.player_state.game_state == 1) {		// 1:find_match

			cout << (char*)player_profile.player_info.name << "Ŭ���̾�Ʈ �� ��ȣ: " << player_profile.room_num << endl;

			int other_player_num = 0;
			for (auto& player : player_list) {
				if (player.second->room_num == player_profile.room_num)
					player.second->player_info.player_color[other_player_num] = other_player_num;
					if (player.second->player_info.name != player_profile.player_info.name) {		//������ [0]�� ������ ����
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

			//�ʱ� �����͸� ���´��� Ȯ��
			if (first_send)
			{	
				//created_object �۽�
				retval = send(client_sock, (char*)&create_map.objects, sizeof(create_map.objects), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				//player_state �۽�
				retval = send(client_sock, (char*)&player_profile.player_state, sizeof(PS), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				first_send = false;
			}
			else{
				//���� ������ ��� player_state ����
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

	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",addr, ntohs(clientaddr.sin_port));
	//map���� �÷��̾� ����
	player_list.erase(addr);
	client_thread_list.erase(addr);
	
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
