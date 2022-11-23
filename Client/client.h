#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���
#include <iostream>
#include<map>
#include "game.h"

WSADATA wsa;
Game game;
//SOCKET sock;
int retval;
#define SERVERPORT 9000
char* SERVERIP = (char*)"127.0.0.1";
#define NAMESIZE 20

// �������� for event
bool w_check = true;
bool a_check = true;
bool s_check = true;
bool d_check = true;
int gun_type = 0;
char key = '0';

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
// ���� �Լ� ���� ���
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
// ���� �Լ� ���� ���
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[����] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


void send_name(SOCKET sock, Game* game)	// �÷��̾� �̸� ������
{
	char name_buf[NAMESIZE]{};
	strncpy(name_buf, game->Name, strlen(game->Name));
	cout << name_buf << endl;
	retval = send(sock, name_buf, NAMESIZE, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send_name()");
	}
}

void gun_change(SOCKET sock, int gun)	// �� ���� ������
{
	char cgun = gun + '0';
	cout << "�� Ÿ���� " << cgun << endl;
	retval = send(sock, (char*)&cgun, sizeof(char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("gun_change()");
	}
	gun_type = gun;
}

// Ŭ���̾�Ʈ �ΰ��ӿ����� �Է�
void send_event(SOCKET sock)
{
	// key input for gun changing
	if(gun_type != game.weapon_type)
		gun_change(sock, game.weapon_type);

	// key input for player move
	if (game.w_Pressed == true && w_check == false){
		cout << "w.Pressed!" << endl;
		key = 'w';
		retval = send(sock, (char*)&key, sizeof(char), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		w_check = true;
	}
	else if (game.w_Pressed == false && w_check == true) {
		cout << "w.Pressed UP!" << endl;
		key = 'w';
		retval = send(sock, (char*)&key, sizeof(char), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		w_check = false;
	}

	if (game.a_Pressed == true && a_check == false){
		cout << "a.Pressed!" << endl;
		key = 'a';
		retval = send(sock, (char*)&key, sizeof(char), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		a_check = true;
	}
	else if (game.a_Pressed == false && a_check == true) {
		cout << "a.Pressed UP!" << endl;
		key = 'a';
		retval = send(sock, (char*)&key, sizeof(char), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		a_check = false;
	}

	if (game.s_Pressed == true && s_check == false){
		cout << "s.Pressed!" << endl;
		key = 's';
		retval = send(sock, (char*)&key, sizeof(char), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		s_check = true;
	}
	else if (game.s_Pressed == false && s_check == true) {
		cout << "s.Pressed UP!" << endl;
		key = 's';
		retval = send(sock, (char*)&key, sizeof(char), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		s_check = false;
	}

	if (game.d_Pressed == true && d_check == false){
		cout << "d.Pressed!" << endl;
		key = 'd';
		retval = send(sock, (char*)&key, sizeof(char), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		d_check = true;
	}
	else if (game.d_Pressed == false && d_check == true) {
		cout << "d.Pressed UP!" << endl;
		key = 'd';
		retval = send(sock, (char*)&key, sizeof(char), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		d_check = false;
	}

	// mouse_input
}