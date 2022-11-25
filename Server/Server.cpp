#include "Common.h"
#include "global.h"
#include "ingame.h"


#define MAX_CLIENT_IN_ROOM 1
using namespace std;

int len = 0;
char buffer[BUFSIZE]; // ���� ���� ������
//std::mutex mylock;
//mutex find_lock;
//���� ����
int hostnum;
char key_check; 
TF mouse_point;

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
	int room_num = 1;

	//3player�� find_match�� �������� Ȯ��
	while (1) {
		if (!find_match_3p(room_num)){
			cout << "\r";
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
Ingame ingame;
DWORD WINAPI ingame_thread(LPVOID room_num)
{
	cout << (int)room_num <<" : ������ ��������." << endl;
	ingame.create_object();
	while (1) {
		for (auto& player : player_list) {
			if (player.second->room_num == (int)room_num) {
				ingame.character_movement(player.second->player_key_mouse.key, player.second->player_state.player_position);
				//cout << player.second->player_state.player_position.x << " " << player.second->player_state.player_position.y << endl;
				Sleep(10);	//�����ؾ���

			}
		}
	}
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

	//���� player_list
	PS local_player_list[3];

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
		else if (player_profile.player_state.game_state == 1) {		// 1:finding_match

		}
		else if (player_profile.player_state.game_state == 2) {		// 2:loading
			

			//�ʱ� �����͸� ���´��� Ȯ��
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

			cout << "\n" << (char*)player_profile.player_info.name << "Ŭ���̾�Ʈ �� ��ȣ: " << player_profile.room_num << endl;
			//created_object �۽�
			retval = send(client_sock, (char*)&ingame.objects, sizeof(ingame.objects), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			cout << "������ byte : " << sizeof(ingame.objects) << endl;
			cout << "sent first created objects" << endl;
			for (int i = 0; i < 20; ++i) {
				cout << "���� created_objects[" << i << "] : " << ingame.objects[i].object_position.x << " / " << ingame.objects[i].object_position.y << endl;
			}

			//player_state �۽�
			int cnt = 0;
			for (auto& a : player_list) {

				//���� �濡 �ɸ� �÷��̾� ���� ������
				if (a.second->room_num == player_profile.room_num) {
					local_player_list[cnt++] = a.second->player_state;
				}
			}
			//�ӽ� 
			local_player_list[1].game_state = 2;
			local_player_list[2].game_state = 2;

			cout << "sent first player state" << endl;
			cout << local_player_list[0].game_state << endl;
			cout << local_player_list[1].game_state << endl;
			cout << local_player_list[2].game_state << endl;

			retval = send(client_sock, (char*)&local_player_list, sizeof(PS) * 3, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			
			player_profile.player_state.game_state = 3;
		}
		else if (player_profile.player_state.game_state == 3) {		// 3:ingame
			// Ű�Է� �ޱ�
			retval = recv(client_sock, (char*)&key_check, sizeof(char), MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				//����ó��
				break;
			}
			else if (retval == 0) {
				//����ó��
			}
			if (key_check != '0') {
				player_profile.player_key_mouse.key = key_check;
			}
			// ���Է� Ȯ��
			if (key_check == 'b') {
				retval = recv(client_sock, (char*)&mouse_point, sizeof(TF), MSG_WAITALL);
				cout << mouse_point.x << " : " << mouse_point.y << endl;
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					//����ó��
					break;
				}
				else if (retval == 0) {
					//����ó��
				}
			}
			

			//local�� ���� pp�� �ִ� ps �־��ֱ�
			local_player_list[0] = player_profile.player_state;
			int i = 1;
			for (auto& player : player_list) {
				if (player.second->room_num == (int)player_profile.room_num) {
					local_player_list[i] = player.second->player_state;
				}
			}
			//player state�����ֱ�
			retval = send(client_sock, (char*)&local_player_list, sizeof(PS) * 3, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			// cout << local_player_list[0].player_position.x << " / " << local_player_list[0].player_position.y << endl;

		}
		else if (player_profile.player_state.game_state == 4) {		// 4:lose

		}
		else if (player_profile.player_state.game_state == 5) {		// 5:win

		}
	}
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));
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
