#pragma once
#include<map>
#include <iostream>
#include <fstream>
#include<vector>
#include<mutex>
#include<string>

using namespace std;

#define MAX_CLIENT_IN_ROOM 2

#define SERVERPORT	9000
#define BUFSIZE		4096
#define NAMESIZE	20
#define MAXITEM		50

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
	int gun_type{};
	int bullet[3]{};
	TI object_position{};
	TF player_position{};
	float player_rotation{};
	bool gun_fired{};
	int game_state{};		// 0:main, 1:find_match, 2:in_game, 3:lose, 4:win
}PS;

typedef struct players_info {
	int player_color[3]{0};
	char name[3][20]{ 0 };
}PI;

typedef struct client_input {
	bool w_Pressed;
	bool a_Pressed;
	bool s_Pressed;
	bool d_Pressed;
	bool clicked;
	float mouse_rotation;
}CI;

typedef struct players_profile {
	int room_num = 0;
	PI player_info;
	PS player_state;
	CI input;
}PP;

typedef struct created_object
{
	int object_type;	// 0: 바위, 1:세로 벽, 2:가로 벽, 3: rifle, 4:sniper
	TI object_position;
}CO;


extern map<int, char*> client_thread_list;
extern map<int, PP*> player_list;
