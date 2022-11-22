#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���
#include <iostream>
#include "game.h"

WSADATA wsa;
Game game;
//SOCKET sock;
int retval;
#define SERVERPORT 9000
char* SERVERIP = (char*)"127.0.0.1";
#define NAMESIZE 20

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

// Ŭ���̾�Ʈ �ΰ��ӿ����� �Է�
bool w_check = true;
bool a_check = true;
bool s_check = true;
bool d_check = true;
void send_event(SOCKET sock)
{
	// key input for player move
	if (game.w_Pressed == true && w_check == false )
	{
		cout << "w.Pressed!" << endl;
		retval = send(sock, (char*)&game.w_Pressed, sizeof(bool), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		w_check = true;
	}
	else if ( game.w_Pressed == false && w_check == true ) {
		cout << "w.Pressed UP!" << endl;
		retval = send(sock, (char*)&game.w_Pressed, sizeof(bool), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		w_check = false;
	}

	if (game.a_Pressed == true && a_check == false)
	{
		cout << "a.Pressed!" << endl;
		retval = send(sock, (char*)&game.a_Pressed, sizeof(bool), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		a_check = true;
	}
	else if (game.a_Pressed == false && a_check == true) {
		cout << "a.Pressed UP!" << endl;
		retval = send(sock, (char*)&game.a_Pressed, sizeof(bool), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		a_check = false;
	}

	if (game.s_Pressed == true && s_check == false)
	{
		cout << "s.Pressed!" << endl;
		retval = send(sock, (char*)&game.s_Pressed, sizeof(bool), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		s_check = true;
	}
	else if (game.s_Pressed == false && s_check == true) {
		cout << "s.Pressed UP!" << endl;
		retval = send(sock, (char*)&game.s_Pressed, sizeof(bool), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		s_check = false;
	}

	if (game.d_Pressed == true && d_check == false)
	{
		cout << "d.Pressed!" << endl;
		retval = send(sock, (char*)&game.d_Pressed, sizeof(bool), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		d_check = true;
	}
	else if (game.d_Pressed == false && d_check == true) {
		cout << "d.Pressed UP!" << endl;
		retval = send(sock, (char*)&game.d_Pressed, sizeof(bool), 0);
		if (retval == SOCKET_ERROR) {
			err_display("key_pressed()");
		}
		d_check = false;
	}

	// mouse_input
}