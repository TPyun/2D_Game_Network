#include "ingame.h"

void ingame::create_object()		// 초기 맵 랜덤 생성하는 함수 (바위, 벽, 아이템, 플레이어 색깔 등)
{
	std::cout << "create_map 실행" << std::endl;

	for (int i = 0; i < 10; ++i) {
		objects[i].object_position.x = uid(dre);
		objects[i].object_position.y = uid(dre);/*
		std::cout << i+1 << "번째 rock" << std::endl;
		std::cout << "오브젝트 타입 : " << objects[i].object_type << std::endl;
		std::cout << "x : " << objects[i].object_position.x << std::endl;
		std::cout << "y : " << objects[i].object_position.y << std::endl;*/
	}

	for (int i = 10; i < 20; ++i) {
		objects[i].object_type = 1;
		objects[i].object_position.x = uid(dre);
		objects[i].object_position.y = uid(dre);/*
		std::cout << i + 1 << "번째 wall" << std::endl;
		std::cout << "오브젝트 타입 : " << objects[i].object_type << std::endl;
		std::cout << "x : " << objects[i].object_position.x << std::endl;
		std::cout << "y : " << objects[i].object_position.y << std::endl;*/
	}

	std::cout << "create_map 생성 완료" << std::endl;
}
