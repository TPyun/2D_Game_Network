#pragma once
#include <iostream>
#include <random>
#include <cstring>
#include "global.h"

struct key_presseds {
	bool w_Pressed;
	bool a_Pressed;
	bool s_Pressed;
	bool d_Pressed;
};
class Ingame
{
public:
	CO objects[20]{};
	void create_object();
	void character_movement(char, TF&);
	key_presseds key_pressed;

private:
	// 화면 크기에 따라 조정 필요
	std::uniform_int_distribution<int> uid{ -900, 900 };
	std::default_random_engine dre;
	

};


