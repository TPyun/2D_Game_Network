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
	void character_movement(CI, TF&);
	void collide_check(PP*, CI*, TF);	// player_input, player, bullet_posion
	// void mouseEvent_ingame();

private:
	// ȭ�� ũ�⿡ ���� ���� �ʿ�
	uniform_int_distribution <int> uid{ -5, 5 };
	random_device rd;
	default_random_engine dre{ rd() };
};


