#pragma once
#include <iostream>
#include <random>
#include <cstring>
#include "global.h"

class ingame
{
public:
	void create_object();
	CO objects[20]{};

private:
	// 화면 크기에 따라 조정 필요
	std::uniform_int_distribution<int> uid{ -900, 900 };
	std::default_random_engine dre;
};

