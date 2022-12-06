#pragma once
#include <iostream>
#include <string.h>
#include <string>
#include <time.h>
#include <thread>
#include <random>

#include "Extra_Libs\SDL2-2.24.0\include\SDL.h"
#include "Extra_Libs\SDL2_image-2.6.2\include\SDL_image.h"
#include "Extra_Libs\SDL2_mixer-2.6.2\include\SDL_mixer.h"
#include "Extra_Libs\SDL2_ttf-2.0.15\include\SDL_ttf.h"

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크
using namespace std;

#define WIDTH 1240
#define HEIGHT 720
#define MAXITEM 50

//#define WIDTH 1200
//#define HEIGHT 900

typedef struct TWO_Floats {
	float x;
	float y;
}TF;

typedef struct TWO_Ints {
	int x;
	int y;
}TI;

typedef struct players_state {
	int hp = 100;
	int gun_type{};
	int bullet[3]{};
	TI object_position{};
	TF player_position{};
	float player_rotation{};
	bool gun_fired{};
	int game_state{};
}PS;

typedef struct players_info
{
	int player_color[3];
	char name[3][20];
}PI;

typedef struct created_object
{
	int object_type;
	TI object_position;
}CO;

typedef struct client_input {
	bool w_Pressed;
	bool a_Pressed;
	bool s_Pressed;
	bool d_Pressed;
	bool clicked;
	float mouse_rotation;
}CI;

typedef struct players_fire {
	int flash_i{};
	bool gun_fired{};
	bool show_bullet{};
	bool gun_flash{};
	float fired_angle{};
	int flash_angle{};
	float bullet_angle{};
	TF bulletPos{};
	TF bulletVelo{};
	TF fired_pos;
}PF;

class Game
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* tmpSurface;
	SDL_Event event;

	SDL_Texture* black_playerTex;
	SDL_Texture* red_playerTex;
	SDL_Texture* blue_playerTex;

	SDL_Texture* targetTex;
	SDL_Texture* backTex;
	SDL_Texture* groundTex;
	SDL_Texture* bulletTex;
	SDL_Texture* flashTex;

	SDL_Texture* pistolTex;
	SDL_Texture* rifleTex;
	SDL_Texture* sniperTex;

	SDL_Texture* whiteTex;
	SDL_Texture* greenTex;
	SDL_Texture* yellowTex;
	SDL_Texture* redTex;

	SDL_Texture* infinity_Tex;

	SDL_Texture* pistol_ammoTex;
	SDL_Texture* rifle_ammoTex;
	SDL_Texture* sniper_ammoTex;

	SDL_Texture* rock_Tex;
	SDL_Texture* wall1_Tex;
	SDL_Texture* wall2_Tex;

	Mix_Chunk* gunsound;

	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8* wavBuffer;

	TTF_Font* font;

	TF MyVelo;
	TF middle_pos;
	

	TI flash_size;

	SDL_Rect srcR, destR;
	SDL_Point center;

	int iMonitorWidth;    // 모니터 가로 해상도
	int iMonitorHeight; //새로  해상도

	int window_moved_x = 0;
	int window_moved_y = 0;

	int mouse_X = 0;
	int mouse_Y = 0;

	//bool show_bullet = false; 
	//bool gun_flash = false;
	
	/*int flash_i;
	int flash_j;
	int flash_k;
	int flash_angle;
	int flash_angle_1;
	int flash_angle_2;*/
	int fired_time = 0;

	int deviceId = 0;

	int background_size = 1800;
	int ground_size = 2000;
	int player_size = 40;
	int bullet_size = 100;
	int crosshair_size = 80;

	//float fired_angle = 0;
	//float bullet_angle = 0;

	int flash_sprite_w = 1667;
	int flash_sprite_h = 875;

	int my_health = 100;
	int enemy1_health = 100;
	int enemy2_health = 100;

	int rifle_ammo = 30;
	int sniper_ammo = 5;

	//메뉴화면 변수
	string text_in = "";
	char text_show[100]{};
	int text_in_height{ 100 };

	bool game_start = false;

	//함수
	void initVariables();
	void initWindow();

	void clearRenderer();
	void updateRenderer();

	SDL_Texture* loadImage(const char*);
	void loadWavs();
	void loadFont();

	void keyEvent_ingame();
	void mouseEvent_ingame();

	double calcAngleFromPoints(TF first_point, TF second_point);
	int Timer(int start_time, int delay);

	void drawBackground();
	void drawGround();
	void drawCharacter(SDL_Texture* tex, TF pos, float rot);
	void drawHealthbar(int health, TF pos);
	void drawBullet(int ,float char_angle, TF);
	void drawFlash(int , float char_angle, TF pos);
	void drawCrosshair();
	void drawText(int x, int y, char[], SDL_Color);
	void drawWeaponList();
	void drawObstacle();

	void drawMenu();
	void drawIngame();

	void mouseEvent_menu();
	void collide_check(float a, float b, float c, float d, float e, float f, float g, float h);
public:
	float delayTime;
	int done = 0;
	int curr_state = 0; // 0: menu, 1: ingame
	bool find_match = false;
	bool waiting_match = false;
	bool connect_server = false; //서버와 연결했다는 것을 game.cpp에서 인지하게끔. main에서 false해버리면 game.cpp에서 진행 안됨
	bool server_connected = false; //서버와 연결하면 connect다시 안하게끔

	char IPAdress[20] = "127.0.0.1";
	char Port[10] = "9000";
	char Name[20] = "wooyung";

	int weapon_type = 0; // 0: pistol, 1: rifle, 2: sniper
	
	bool p1_shoot_check = true;
	bool p2_shoot_check = true;
	
	//bool gun_fired = false;
	TF mouse_point;

	TF MyCharPos;

	float my_char_angle = 0.f;

	CO created_objects[MAXITEM];
	PS player_list[3];
	client_input input;
	PF player_fire[3];

	Game();
	~Game();
	void draw_enemy_fire(bool* shoot_check, int);
	void update();
};
