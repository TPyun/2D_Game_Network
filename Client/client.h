#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더
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

// 전역변수 for event
bool w_check = true;
bool a_check = true;
bool s_check = true;
bool d_check = true;
int gun_type = 0;
char key = '0';
char bullet = 'b';
bool shoot_check = true;

// 소켓 함수 오류 출력 후 종료
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
// 소켓 함수 오류 출력
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
// 소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[오류] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


void send_name(SOCKET sock, Game* game)	// 플레이어 이름 보내기
{
	char name_buf[NAMESIZE]{};
	strncpy(name_buf, game->Name, strlen(game->Name));
	cout << name_buf << endl;
	retval = send(sock, name_buf, NAMESIZE, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send_name()");
	}
}
//
//void gun_change(SOCKET sock, int gun)	// 총 종류 보내기
//{
//	char cgun = gun + '0';
//	cout << "건 타입은 " << cgun << endl;
//	retval = send(sock, (char*)&cgun, sizeof(char), 0);
//	if (retval == SOCKET_ERROR) {
//		err_display("gun_change()");
//	}
//	gun_type = gun;
//}

//void mouse_input(SOCKET sock)
//{
//	if (game.gun_fired == true && shoot_check == true) {
//		cout << "shoot!" << endl;
//		retval = send(sock, (char*)&bullet, sizeof(char), 0);
//		if (retval == SOCKET_ERROR) {
//			err_display("mouse_pressed()");
//		}
//		retval = send(sock, (char*)&game.mouse_point, sizeof(TF), 0);
//		if (retval == SOCKET_ERROR) {
//			err_display("mouse_pressed()");
//		}
//	}
//	shoot_check = false;
//	if (game.gun_fired == false && shoot_check == false) {
//		shoot_check = true;
//	}
//}

// 클라이언트 인게임에서의 입력
void send_event(SOCKET sock)
{
	// key input for player move
	retval = send(sock, (char*)&game.input, sizeof(CI), 0);
	if (retval == SOCKET_ERROR) {
		err_display("key_pressed()");
	}

}