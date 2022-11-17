#pragma once

#define SERVERPORT	9000
#define BUFSIZE		4096
#define NAMESIZE	20

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

typedef struct players_info {
	int player_color[3];
	char name[3][20];
}PI;

typedef struct created_object
{
	int object_type;	// 0:바위, 1:벽, 2:아이템
	TI object_position;
}CO;
