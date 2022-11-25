#pragma once
#include<map>
#include <iostream>
#include <fstream>
#include<vector>
#include<mutex>
#include<string>

using namespace std;

#define SERVERPORT	9000
#define BUFSIZE		4096
#define NAMESIZE	20

typedef struct TWO_Floats {
	float x = 0;
	float y = 0;
}TF;

typedef struct TWO_Ints {
	int x = 0;
	int y = 0;
}TI;

typedef struct players_state {
	int hp = 100;
	int gun_type = 0;
	int bullet[3];
	TI object_position;
	TF player_position;
	bool gun_fired;
	int game_state;		// 0:main, 1:find_match, 2:in_game, 3:lose, 4:win
}PS;

typedef struct players_info {
	int player_color[3];
	char name[3][20];
}PI;

typedef struct player_key_mouse {
	char key = ' ';
	TF mouse_coordinates;
}PKM;

typedef struct players_profile {
	int room_num;
	PI player_info;
	PS player_state;
	PKM player_key_mouse;
}PP;

typedef struct created_object
{
	int object_type;	// 0:바위, 1:벽, 2:아이템
	TI object_position;
}CO;

extern map<char*, char*> client_thread_list;
extern map<char*, PP*> player_list;
