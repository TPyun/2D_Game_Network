#include "ingame.h"
#include "global.h"

void Ingame::create_object()		// 초기 맵 랜덤 생성하는 함수 (바위, 벽, 아이템, 플레이어 색깔 등)
{
	std::cout << "create_map 실행" << std::endl;

	for (int i = 0; i < 1; ++i) {
		objects[i].object_position.x = uid(dre);
		objects[i].object_position.y = uid(dre);
		objects[i].object_position.x *= OBJ_DISTANCE;
		objects[i].object_position.y *= OBJ_DISTANCE;

		/*std::cout << i+1 << "번째 rock" << std::endl;
		std::cout << "오브젝트 타입 : " << objects[i].object_type << std::endl;
		std::cout << "x : " << objects[i].object_position.x << std::endl;
		std::cout << "y : " << objects[i].object_position.y << std::endl;*/
	}
	for (int i = 10; i < 20; ++i) {
		objects[i].object_type = 1;
		objects[i].object_position.x = uid(dre);
		objects[i].object_position.y = uid(dre);

		objects[i].object_position.x *= OBJ_DISTANCE;
		objects[i].object_position.y *= OBJ_DISTANCE;
		/*std::cout << i + 1 << "번째 wall" << std::endl;
		std::cout << "오브젝트 타입 : " << objects[i].object_type << std::endl;
		std::cout << "x : " << objects[i].object_position.x << std::endl;
		std::cout << "y : " << objects[i].object_position.y << std::endl;*/
	}
	for (int i = 20; i < 30; ++i) {
		objects[i].object_type = 2;
		objects[i].object_position.x = uid(dre);
		objects[i].object_position.y = uid(dre);

		objects[i].object_position.x *= OBJ_DISTANCE;
		objects[i].object_position.y *= OBJ_DISTANCE;
		/*std::cout << i + 1 << "번째 wall" << std::endl;
		std::cout << "오브젝트 타입 : " << objects[i].object_type << std::endl;
		std::cout << "x : " << objects[i].object_position.x << std::endl;
		std::cout << "y : " << objects[i].object_position.y << std::endl;*/
	}

	for (int i = 30; i < 40; ++i) {			// rifle
		objects[i].object_type = 3;
		objects[i].object_position.x = uid(dre);
		objects[i].object_position.y = uid(dre);
		objects[i].object_position.x *= OBJ_DISTANCE;
		objects[i].object_position.y *= OBJ_DISTANCE;
	}
	for (int i = 40; i < 50; ++i) {			// sniper
		objects[i].object_type = 4;
		objects[i].object_position.x = uid(dre);
		objects[i].object_position.y = uid(dre);
		objects[i].object_position.x *= OBJ_DISTANCE;
		objects[i].object_position.y *= OBJ_DISTANCE;
	}

	list<int> changed_obj{};
	// object 겹침 체크
	for (int i = 0; i < MAXITEM - 1; ++i) {
		for (int j = i + 1; j <= MAXITEM; ++j)
			if (objects[i].object_position.x == objects[j].object_position.x && objects[i].object_position.y == objects[j].object_position.y) {
				changed_obj.push_back(i);
				objects[i].object_position.x = uid(dre) * OBJ_DISTANCE;
				objects[i].object_position.y = uid(dre) * OBJ_DISTANCE;
			}
	}

	while (changed_obj.empty() == 0) {
		for (list<int>::iterator iter = changed_obj.begin(); iter != changed_obj.end(); ++iter) {
			objects[*iter].object_position.x = uid(dre) * OBJ_DISTANCE;
			objects[*iter].object_position.y = uid(dre) * OBJ_DISTANCE;
		}
		changed_obj.clear();
		for (int i = 0; i < MAXITEM - 1; ++i) {
			for (int j = i + 1; j <= MAXITEM; ++j)
				if (objects[i].object_position.x == objects[j].object_position.x &&
					objects[i].object_position.y == objects[j].object_position.y) {
					changed_obj.push_back(i);
					objects[i].object_position.x = uid(dre) * OBJ_DISTANCE;
					objects[i].object_position.y = uid(dre) * OBJ_DISTANCE;
				}
		}
	}
	int i = 0;
	for (CO obj : objects) {
		cout << i << " " << obj.object_type << " " << obj.object_position.x << " " << obj.object_position.y << endl;
		i++;
	}

	std::cout << "create_map 생성 완료" << std::endl;
}

void Ingame::character_movement(CI input, TF &pos)
{

	//cout << "W: " << w_pressed << ", A: " << a_pressed << ", S: " << s_pressed << ", D: " << d_pressed << endl;

	//float minimun_movement = 10.f;
	float minimun_movement = 3.f;
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

	float MaxVelo = 150.f;
	float MinVelo = -150.f;

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
	float friction = 0.1f;

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

void Ingame::collide_check(PP* player, CI* input)
{
	// 플레이어와 objects 간의 collide_check
	for (auto& obj : objects)
	{
		if (abs(obj.object_position.x - player->player_state.player_position.x) < 100
			&& abs(obj.object_position.y - player->player_state.player_position.y) < 100)
		{
			//돌
			if (obj.object_type == 0)
			{
				if (obj.object_position.x - player->player_state.player_position.x < 53 &&
					obj.object_position.x - player->player_state.player_position.x > -28)
				{
					if (obj.object_position.y - player->player_state.player_position.y < 62
						&& obj.object_position.y - player->player_state.player_position.y > 0)
						input->s_Pressed = false;
					if (obj.object_position.y - player->player_state.player_position.y > -35
						&& obj.object_position.y - player->player_state.player_position.y < 0)
						input->w_Pressed = false;
				}
				if (obj.object_position.y - player->player_state.player_position.y < 53 &&
					obj.object_position.y - player->player_state.player_position.y > -28)
				{
					if (obj.object_position.x - player->player_state.player_position.x < 62
						&& obj.object_position.x - player->player_state.player_position.x > 0)
						input->d_Pressed = false;
					if (obj.object_position.x - player->player_state.player_position.x > -37
						&& obj.object_position.x - player->player_state.player_position.x < 0)
						input->a_Pressed = false;
				}
			}
			//벽1 세로
			if (obj.object_type == 1)
			{
				//cout << obj.object_position.x << " / " << obj.object_position.y << endl;
				if (obj.object_position.x - player->player_state.player_position.x < 64 &&
					obj.object_position.x - player->player_state.player_position.x > 8)
				{
					if (obj.object_position.y - player->player_state.player_position.y < 72
						&& obj.object_position.y - player->player_state.player_position.y > 0)
						input->s_Pressed = false;
					if (obj.object_position.y - player->player_state.player_position.y > -72
						&& obj.object_position.y - player->player_state.player_position.y < 0)
						input->w_Pressed = false;
				}
				if (obj.object_position.y - player->player_state.player_position.y < 64 &&
					obj.object_position.y - player->player_state.player_position.y > -64)

				{
					if (obj.object_position.x - player->player_state.player_position.x < 72
						&& obj.object_position.x - player->player_state.player_position.x > 30)
						input->d_Pressed = false;
					if (obj.object_position.x - player->player_state.player_position.x > 5
						&& obj.object_position.x - player->player_state.player_position.x < 20)
						input->a_Pressed = false;
				}
			}
			//벽2 가로
			if (obj.object_type == 2)
			{
				if (obj.object_position.y - player->player_state.player_position.y < 64 &&
					obj.object_position.y - player->player_state.player_position.y > 8)
				{
					if (obj.object_position.x - player->player_state.player_position.x < 72
						&& obj.object_position.x - player->player_state.player_position.x > 57)
						input->d_Pressed = false;
					if (obj.object_position.x - player->player_state.player_position.x > -72
						&& obj.object_position.x - player->player_state.player_position.x < -57)
						input->a_Pressed = false;
				}
				if (obj.object_position.x - player->player_state.player_position.x < 64 &&
					obj.object_position.x - player->player_state.player_position.x > -64)
				{
					if (obj.object_position.y - player->player_state.player_position.y < 72
						&& obj.object_position.y - player->player_state.player_position.y > 30)
						input->s_Pressed = false;
					if (obj.object_position.y - player->player_state.player_position.y > 5
						&& obj.object_position.y - player->player_state.player_position.y < 20)
						input->w_Pressed = false;
				}
			}

			//rifle
			if (obj.object_type == 3)
			{
				if (obj.object_position.x - player->player_state.player_position.x < 60 &&
					obj.object_position.x - player->player_state.player_position.x > 0)
				{
					if (obj.object_position.y - player->player_state.player_position.y < 62
						&& obj.object_position.y - player->player_state.player_position.y > 0)
					{
						cout << "rifle get" << endl;
						obj.object_type = -1;
						player->player_state.object_position = obj.object_position;
						player->player_state.bullet[1] += 10;
						break;
					}
				}
				if (obj.object_position.y - player->player_state.player_position.y < 60 &&
					obj.object_position.y - player->player_state.player_position.y > 0)
				{
					if (obj.object_position.x - player->player_state.player_position.x < 62
						&& obj.object_position.x - player->player_state.player_position.x > 0)
					{
						cout << "rifle get" << endl;
						obj.object_type = -1;
						player->player_state.object_position = obj.object_position;
						player->player_state.bullet[1] += 10;
						break;
					}
				}
			}

			//sniper
			if (obj.object_type == 4)
			{
				if (obj.object_position.x - player->player_state.player_position.x < 60 &&
					obj.object_position.x - player->player_state.player_position.x > 0)
				{
					if (obj.object_position.y - player->player_state.player_position.y < 62
						&& obj.object_position.y - player->player_state.player_position.y > 0)
					{
						cout << "sniper get" << endl;
						obj.object_type = -1;
						player->player_state.object_position = obj.object_position;
						player->player_state.bullet[2] += 5;
						break;
					}
				}
				if (obj.object_position.y - player->player_state.player_position.y < 60 &&
					obj.object_position.y - player->player_state.player_position.y > 0)
				{
					if (obj.object_position.x - player->player_state.player_position.x < 62
						&& obj.object_position.x - player->player_state.player_position.x > 0)
					{
						cout << "sniper get" << endl;
						obj.object_type = -1;
						player->player_state.object_position = obj.object_position;
						player->player_state.bullet[2] += 5;
						break;
					}
				}
			}


		}
	}


	// 플레이어와 bullet 간의 collide_check
	/*if(abs(bullet.x - player->player_state.player_position.x < 50)
		&& abs(bullet.x - player->player_state.player_position.x < 50))
	{
		show_bullet = false;
		cout << "collide player!" << endl;
		player->player_state.collide = true;
		bullet.x = 0.f;
		bullet.y = 0.f;
	}*/

	// bullet과 objects 간의 collide_check
	for (auto& obj : objects)
	{
		if (abs(obj.object_position.x - player->player_state.bullet_pos.x) < 50
			&& abs(obj.object_position.y - player->player_state.bullet_pos.y) < 50)
		{
			if (obj.object_type != 3 && obj.object_type != 4) 
			{
				if (obj.object_type == 0) // 돌
				{
					if (player->player_state.bullet_pos.x - obj.object_position.x > 25) {
						continue;
					}
					else if (player->player_state.bullet_pos.y - obj.object_position.y > 20) {
						continue;
					}
					else {
						show_bullet = false;
						cout << "collide rok!" << endl;
						player->player_state.collide = true;
						player->player_state.bullet_pos.x = -float(ground_size / 2);
						player->player_state.bullet_pos.y = -float(ground_size / 2);
					}
				}
				else if (obj.object_type == 1)	// 세로 벽
				{
					if (player->player_state.bullet_pos.x - obj.object_position.x > 1) {
						continue;
					}
					show_bullet = false;
					cout << "collide 세로벽!" << endl;
					player->player_state.collide = true;
					player->player_state.bullet_pos.x = -float(ground_size / 2);
					player->player_state.bullet_pos.y = -float(ground_size / 2);
				}
				else if (obj.object_type == 2)	// 가로 벽
				{

					if (player->player_state.bullet_pos.y - obj.object_position.y > 1) {
						continue;
					}
					show_bullet = false;
					cout << "collide 가로벽!" << endl;
					player->player_state.collide = true;
					player->player_state.bullet_pos.x = -float(ground_size / 2);
					player->player_state.bullet_pos.y = -float(ground_size / 2);
				}
			}
		}
	}
}


void Ingame::bullet_movement(float fired_angle, PP* player)
{
	float bullet_angle = 3.14159265 * 2 * fired_angle / 360;
	if (player->player_state.gun_fired == true && show_bullet == true) {
		bulletVelo.x = cos(bullet_angle) * 30;
		bulletVelo.y = sin(bullet_angle) * 30;
		player->player_state.bullet_pos.x += bulletVelo.x;
		player->player_state.bullet_pos.y += bulletVelo.y;
		cout << "날으는 총알 x : " << player->player_state.bullet_pos.x << "\t y : " << player->player_state.bullet_pos.y << endl;
		
		if (player->player_state.bullet_pos.x > ground_size / 2 ||
			player->player_state.bullet_pos.x < -(ground_size / 2) ||
			player->player_state.bullet_pos.y > ground_size / 2 ||
			player->player_state.bullet_pos.y < -(ground_size / 2) )
		{
			cout << "bullet out" << endl;
			show_bullet = false;
			player->player_state.bullet_pos.x = -float(ground_size / 2);
			player->player_state.bullet_pos.y = -float(ground_size / 2);
		}
	}
	else if (player->player_state.gun_fired == false && show_bullet == false)
	{
		player->player_state.collide = false;
		show_bullet = true;
	}
}
//void Ingame::mouseEvent_ingame()
//{
//	//get mouse coordinates 
//	SDL_GetGlobalMouseState(&mouse_X, &mouse_Y);
//
//	mouse_point.x = mouse_X;
//	mouse_point.y = mouse_Y;
//
//	//모니터 해상도 좌표를 게임 게임 창 좌표 기준으로 변환시킴
//	SDL_GetWindowPosition(window, &window_moved_x, &window_moved_y);
//	mouse_point.x -= window_moved_x;
//	mouse_point.y -= window_moved_y;
//
//	//Get Character Angle by Mouse's Coordinates
//	my_char_angle = calcAngleFromPoints(mouse_point, middle_pos);
//
//	//서버에 전송할 struct 변수에 마우스 좌표 저장, clicked
//	input.mouse_rotation = my_char_angle;
//	//마우스 왼 버튼 누르면 발사
//	if (event.type == SDL_MOUSEBUTTONDOWN) {
//		input.clicked = true;
//		if (event.button.button == SDL_BUTTON_LEFT && gun_fired == false) {
//			gun_fired = true;
//			fired_time = clock();
//
//			show_bullet = true;
//			gun_flash = true;
//
//			Mix_PlayChannel(-1, gunsound, 0);
//		}
//	}
//
//	//총이 발사되고 타이머 작동시켜서 1초 뒤 다시 발사 가능awd
//	if (gun_fired) {
//		if (Timer(fired_time, 100) == 1) {
//			gun_fired = false;
//			fired_time = 0;
//		}
//	}
//}
