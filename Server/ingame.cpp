#include "ingame.h"
#include "global.h"

void Ingame::create_object()		// 초기 맵 랜덤 생성하는 함수 (바위, 벽, 아이템, 플레이어 색깔 등)
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

void Ingame::character_movement(CI input, TF &pos)
{

	//cout << "W: " << w_pressed << ", A: " << a_pressed << ", S: " << s_pressed << ", D: " << d_pressed << endl;

	//float minimun_movement = 10.f;
	float minimun_movement = 5.f;
	TF velo;
	velo.x = 0.f;
	velo.y = 0.f;

	if (input.w_Pressed) {
		velo.y -= minimun_movement;
	}
	if (input.s_Pressed) {
		velo.y += minimun_movement;
	}
	if (input.a_Pressed) {
		velo.x -= minimun_movement;
	}
	if (input.d_Pressed) {
		velo.x += minimun_movement;
	}

	float MaxVelo = 200.f;
	float MinVelo = -200.f;

	//Limit Velocity
	if (velo.x > MaxVelo) {
		velo.x = MaxVelo;
	}
	if (velo.x < MinVelo) {
		velo.x = MinVelo;
	}
	if (velo.y > MaxVelo) {
		velo.y = MaxVelo;
	}
	if (velo.y < MinVelo) {
		velo.y = MinVelo;
	}

	//Character Speed Friction
	float friction = 1.f;

	if (velo.x > 0) {
		velo.x -= friction;
	}
	if (velo.x < 0) {
		velo.x += friction;
	}
	if (velo.y > 0) {
		velo.y -= friction;
	}
	if (velo.y < 0) {
		velo.y += friction;
	}

	//offset ignorable velocity
	if (-minimun_movement / 2 <= velo.x && velo.x <= minimun_movement / 2) {
		velo.x = 0;
	}
	if (-minimun_movement / 2 <= velo.y && velo.y <= minimun_movement / 2) {
		velo.y = 0;
	}

	//Keep Character Visible in Window
	int ground_size = 2000;
	int player_size = 40;

	if (pos.x > ground_size / 2 - player_size / 2) {
		pos.x = ground_size / 2 - player_size / 2;
		velo.x = 0;
	}
	else if (pos.x < -ground_size / 2 + player_size / 2) {
		pos.x = -ground_size / 2 + player_size / 2;
		velo.x = 0;
	}
	if (pos.y > ground_size / 2 - player_size / 2) {
		pos.y = ground_size / 2 - player_size / 2;
		velo.y = 0;
	}
	else if (pos.y < -ground_size / 2 + player_size / 2) {
		pos.y = -ground_size / 2 + player_size / 2;
		velo.y = 0;
	}
	
	//add velocity to XY coordinates
	pos.x += velo.x;
	pos.y += velo.y;
	
	//cout << "velo: " << velo.x << ", " << velo.y << endl;
	//cout << "플레이어 위치: " << pos.x << ", " << pos.y << endl;
}


