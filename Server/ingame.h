#pragma once
#include <iostream>
#include <random>
#include <cstring>
#include "global.h"



class Ingame
{
public:
	CO objects[MAXITEM]{};
	void create_object();
	void character_movement(CI, TF&);

private:
	// ȭ�� ũ�⿡ ���� ���� �ʿ�
	std::uniform_int_distribution<int> uid{ -900, 900 };
	std::default_random_engine dre;
};


