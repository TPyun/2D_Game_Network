#pragma once
#include <iostream>
#include <random>
#include <cstring>
#include <list>
#include "global.h"
#define OBJ_DISTANCE 180

using namespace std;
class Ingame
{
public:
	CO objects[MAXITEM]{};
	void create_object();
	void character_movement(CI, TF&, TF&);
	void collide_check(PP*, CI*);	// player_input, player, bullet_posion
	void bullet_movement(float, PP*);

	//TF bullet[3]{};
	float bullet_angle{};
	TF bulletVelo{};
	void rand_player_pos(PP*);
	void init_player(PP*);
	// void mouseEvent_ingame();

private:
	// 화면 크기에 따라 조정 필요
	uniform_int_distribution <int> uid{ -5, 5 };
	uniform_int_distribution <int> player_pos_rand{ -900, 900 };
	random_device rd;
	default_random_engine dre{ rd() };
	
	bool show_bullet = false;
	int ground_size = 2000;
	float angle;
};
