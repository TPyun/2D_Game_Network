#pragma once
#include <iostream>
#include <random>
#include <cstring>
#include "global.h"

class Ingame
{
public:
	CO objects[20]{};
	void create_object();
	void character_movement(char, TF&);

private:
	// ȭ�� ũ�⿡ ���� ���� �ʿ�
	std::uniform_int_distribution<int> uid{ -900, 900 };
	std::default_random_engine dre;
	bool w_pressed = false;
	bool a_pressed = false;
	bool s_pressed = false;
	bool d_pressed = false;
};

