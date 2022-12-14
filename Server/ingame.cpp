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
			for (int j = i + 1; j <= MAXITEM; ++j) {
				if (objects[i].object_position.x == objects[j].object_position.x &&
					objects[i].object_position.y == objects[j].object_position.y) {
					changed_obj.push_back(i);
					objects[i].object_position.x = uid(dre) * OBJ_DISTANCE;
					objects[i].object_position.y = uid(dre) * OBJ_DISTANCE;
				}
			}
		}
	}
	int i = 0;

}

void Ingame::rand_player_pos(PP* player)
{
	
	bool object_checker = true;
	while (1) {
		player->player_state.player_position.x = player_pos_rand(dre);
		player->player_state.player_position.y = player_pos_rand(dre);
		object_checker = true;
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
							&& obj.object_position.y - player->player_state.player_position.y > 0) {
							object_checker = false;
							player->player_state.velo.y = 0;
						}

						if (obj.object_position.y - player->player_state.player_position.y > -35
							&& obj.object_position.y - player->player_state.player_position.y < 0) {
							object_checker = false;
							player->player_state.velo.y = 0;

						}
					}
					if (obj.object_position.y - player->player_state.player_position.y < 53 &&
						obj.object_position.y - player->player_state.player_position.y > -28)
					{
						if (obj.object_position.x - player->player_state.player_position.x < 62
							&& obj.object_position.x - player->player_state.player_position.x > 0) {
							object_checker = false;
							player->player_state.velo.x = 0;

						}
						if (obj.object_position.x - player->player_state.player_position.x > -37
							&& obj.object_position.x - player->player_state.player_position.x < 0) {
							object_checker = false;
							player->player_state.velo.x = 0;

						}
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
							&& obj.object_position.y - player->player_state.player_position.y > 0) {
							object_checker = false;
							player->player_state.velo.y = 0;
						}

						if (obj.object_position.y - player->player_state.player_position.y > -72
							&& obj.object_position.y - player->player_state.player_position.y < 0) {
							object_checker = false;
							player->player_state.velo.y = 0;

						}
					}
					if (obj.object_position.y - player->player_state.player_position.y < 64 &&
						obj.object_position.y - player->player_state.player_position.y > -64)

					{
						if (obj.object_position.x - player->player_state.player_position.x < 72
							&& obj.object_position.x - player->player_state.player_position.x > 30) {
							object_checker = false;
							player->player_state.velo.x = 0;
						}
						if (obj.object_position.x - player->player_state.player_position.x > 0
							&& obj.object_position.x - player->player_state.player_position.x < 20) {
							object_checker = false;
							player->player_state.velo.x = 0;
						}
					}
				}
				//벽2 가로
				if (obj.object_type == 2)
				{
					if (obj.object_position.y - player->player_state.player_position.y < 64 &&
						obj.object_position.y - player->player_state.player_position.y > 8)
					{
						if (obj.object_position.x - player->player_state.player_position.x < 72
							&& obj.object_position.x - player->player_state.player_position.x > 57) {
							object_checker = false;
							player->player_state.velo.x = 0;
						}
						if (obj.object_position.x - player->player_state.player_position.x > -72
							&& obj.object_position.x - player->player_state.player_position.x < -57) {
							object_checker = false;
							player->player_state.velo.x = 0;

						}
					}
					if (obj.object_position.x - player->player_state.player_position.x < 64 &&
						obj.object_position.x - player->player_state.player_position.x > -64)
					{
						if (obj.object_position.y - player->player_state.player_position.y < 72
							&& obj.object_position.y - player->player_state.player_position.y > 30) {
							object_checker = false;
							player->player_state.velo.y = 0;
						}
						if (obj.object_position.y - player->player_state.player_position.y > 0
							&& obj.object_position.y - player->player_state.player_position.y < 20) {
							object_checker = false;
							player->player_state.velo.y = 0;
						}
					}
				}
			}
			
		}
		if (object_checker)
			return;
	}
}

void Ingame::init_player(PP* player)
{
	player->player_state.hp = 100;
	for (int i = 1; i < 3; ++i){
		player->player_state.bullet[i] = 0;
	}
}

void Ingame::character_movement(CI input, TF &pos, TF &velo)
{
	//cout << "W: " << w_pressed << ", A: " << a_pressed << ", S: " << s_pressed << ", D: " << d_pressed << endl;

	//float minimun_movement = 10.f;
	float minimun_movement = 2.f;

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
	
	float MaxVelo = 3.f;
	float MinVelo = -3.f;

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

	//cout << velo.x << "   " << velo.y << endl;

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
						&& obj.object_position.y - player->player_state.player_position.y > 0) {
						input->s_Pressed = false;
						player->player_state.velo.y = 0;
					}
						
					if (obj.object_position.y - player->player_state.player_position.y > -35
						&& obj.object_position.y - player->player_state.player_position.y < 0) {
						input->w_Pressed = false;
						player->player_state.velo.y = 0;

					}
				}
				if (obj.object_position.y - player->player_state.player_position.y < 53 &&
					obj.object_position.y - player->player_state.player_position.y > -28)
				{
					if (obj.object_position.x - player->player_state.player_position.x < 62
						&& obj.object_position.x - player->player_state.player_position.x > 0) {
						input->d_Pressed = false;
						player->player_state.velo.x = 0;

					}
					if (obj.object_position.x - player->player_state.player_position.x > -37
						&& obj.object_position.x - player->player_state.player_position.x < 0) {
						input->a_Pressed = false;
						player->player_state.velo.x = 0;

					}
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
						&& obj.object_position.y - player->player_state.player_position.y > 0) {
						input->s_Pressed = false;
						player->player_state.velo.y = 0;
					}
					
					if (obj.object_position.y - player->player_state.player_position.y > -72
						&& obj.object_position.y - player->player_state.player_position.y < 0) {
						input->w_Pressed = false;
						player->player_state.velo.y = 0;

					}
				}
				if (obj.object_position.y - player->player_state.player_position.y < 64 &&
					obj.object_position.y - player->player_state.player_position.y > -64)

				{
					if (obj.object_position.x - player->player_state.player_position.x < 72
						&& obj.object_position.x - player->player_state.player_position.x > 30) {
						input->d_Pressed = false;
						player->player_state.velo.x = 0;
					}
					if (obj.object_position.x - player->player_state.player_position.x > 0
						&& obj.object_position.x - player->player_state.player_position.x < 20) {
						input->a_Pressed = false;
						player->player_state.velo.x = 0;
					}
				}
			}
			//벽2 가로
			if (obj.object_type == 2)
			{
				if (obj.object_position.y - player->player_state.player_position.y < 64 &&
					obj.object_position.y - player->player_state.player_position.y > 8)
				{
					if (obj.object_position.x - player->player_state.player_position.x < 72
						&& obj.object_position.x - player->player_state.player_position.x > 57) {
						input->d_Pressed = false;
						player->player_state.velo.x = 0;
					}
					if (obj.object_position.x - player->player_state.player_position.x > -72
						&& obj.object_position.x - player->player_state.player_position.x < -57) {
						input->a_Pressed = false;
						player->player_state.velo.x = 0;

					}
				}
				if (obj.object_position.x - player->player_state.player_position.x < 64 &&
					obj.object_position.x - player->player_state.player_position.x > -64)
				{
					if (obj.object_position.y - player->player_state.player_position.y < 72
						&& obj.object_position.y - player->player_state.player_position.y > 30) {
						input->s_Pressed = false;
						player->player_state.velo.y = 0;
					}
					if (obj.object_position.y - player->player_state.player_position.y > 0
						&& obj.object_position.y - player->player_state.player_position.y < 20) {
						input->w_Pressed = false;
player->player_state.velo.y = 0;
					}
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
	int my_room_num = player->room_num;
	char my_name[50];
	strcpy(my_name, player->player_info.name[0]);
	PP* another_player[MAX_CLIENT_IN_ROOM]{};
	int cnt{ 1 };
	for (auto& player : player_list) {
		//cout << player.first << endl;
		if (player.second == nullptr) {
			continue;
		}
		if (player.second == &null_temp) {
			continue;
		}
		if (strcmp(player.second->player_info.name[0], my_name) != 0 && player.second->room_num == my_room_num) {
			if (cnt < MAX_CLIENT_IN_ROOM) {
				another_player[cnt - 1] = player.second;
				cnt++;
			}
		}
	}

	if (player->player_state.gun_fired == true && show_bullet == true)
	{
		for (int i = 0; i < cnt - 1; ++i)
		{
			if (another_player[i]->player_state.hp > 0 &&
				abs(player->player_state.player_position.x - another_player[i]->player_state.player_position.x) < 75 &&
				abs(player->player_state.player_position.y - another_player[i]->player_state.player_position.y) < 75)
			{
				if (abs(player->bullet_pos.x - another_player[i]->player_state.player_position.x) < 95
					&& abs(player->bullet_pos.y - another_player[i]->player_state.player_position.y) < 95)
					{
					show_bullet = false;
					cout << "collide 닝겐!" << endl;
					player->player_state.collide = true;
					player->bullet_pos.x = -float(ground_size);
					player->bullet_pos.y = -float(ground_size);
					if (player->player_state.gun_type == 0)
						another_player[i]->player_state.hp -= 10;
					else if (player->player_state.gun_type == 1)
						another_player[i]->player_state.hp -= 20;
					else if (player->player_state.gun_type == 2)
						another_player[i]->player_state.hp -= 40;
				}
			}
			else if (another_player[i]->player_state.hp > 0 &&
				abs(player->bullet_pos.x - another_player[i]->player_state.player_position.x) < 20
				&& abs(player->bullet_pos.y - another_player[i]->player_state.player_position.y) < 20)
			{
				show_bullet = false;
				cout << "collide 닝겐!" << endl;
				player->player_state.collide = true;
				player->bullet_pos.x = -float(ground_size);
				player->bullet_pos.y = -float(ground_size);
				if (player->player_state.gun_type == 0)
					another_player[i]->player_state.hp -= 10;
				else if (player->player_state.gun_type == 1)
					another_player[i]->player_state.hp -= 20;
				else if (player->player_state.gun_type == 2)
					another_player[i]->player_state.hp -= 40;
			}
		}

		// bullet과 objects 간의 collide_check
		for (auto& obj : objects)
		{
			// bullet과 objects가 너무 가까울 때.
			if (obj.object_type == 0) 
			{
				if (abs(player->player_state.player_position.x - obj.object_position.x) < 100 &&
					abs(player->player_state.player_position.y - obj.object_position.y) < 100)
				{
					if (0 < player->bullet_pos.x - obj.object_position.x &&
						player->bullet_pos.x - obj.object_position.x < 45 &&
						-60 < obj.object_position.y - player->bullet_pos.y &&
						obj.object_position.y - player->bullet_pos.y < 60)
					{
						cout << "too close to rock." << endl;
						show_bullet = false;
						player->player_state.collide = true;
						player->bullet_pos.x = -float(ground_size);
						player->bullet_pos.y = -float(ground_size);
					}
					else if (45 < obj.object_position.x - player->bullet_pos.x &&
						obj.object_position.x - player->bullet_pos.x < 70 &&
						-22 < obj.object_position.y - player->bullet_pos.y &&
						obj.object_position.y - player->bullet_pos.y < 52)
					{
						cout << "too close to rock." << endl;
						show_bullet = false;
						player->player_state.collide = true;
						player->bullet_pos.x = -float(ground_size);
						player->bullet_pos.y = -float(ground_size);
					}
				}
			}
			else if (obj.object_type == 1)
			{
				if (abs(player->player_state.player_position.x - obj.object_position.x) < 100 &&
					abs(player->player_state.player_position.y - obj.object_position.y) < 100)
				{
					if (-110 < player->bullet_pos.x - obj.object_position.x &&
						player->bullet_pos.x - obj.object_position.x < -30 &&
						-55 < player->bullet_pos.y - obj.object_position.y &&
						player->bullet_pos.y - obj.object_position.y < 50)
					{
						cout << "too close to 세로벽." << endl;
						show_bullet = false;
						player->player_state.collide = true;
						player->bullet_pos.x = -float(ground_size);
						player->bullet_pos.y = -float(ground_size);
					}
					else if (-10 < player->bullet_pos.x - obj.object_position.x &&
						player->bullet_pos.x - obj.object_position.x < 35 &&
						-63 < player->bullet_pos.y - obj.object_position.y &&
						player->bullet_pos.y - obj.object_position.y < 51)
					{
						cout << "too close to 세로벽." << endl;
						show_bullet = false;
						player->player_state.collide = true;
						player->bullet_pos.x = -float(ground_size);
						player->bullet_pos.y = -float(ground_size);
					}
				}
			}
			else if (obj.object_type == 2)
			{
				if (abs(player->player_state.player_position.x - obj.object_position.x) < 150 &&
					abs(player->player_state.player_position.y - obj.object_position.y) < 150)
				{
					cout << endl << "close to 가로벽." << endl;
					cout << "총알x : " << player->bullet_pos.x << endl;
					cout << "옵젝x : " << obj.object_position.x << endl;
					cout << "총 - 옵x : " << player->bullet_pos.x - obj.object_position.x << endl;
					cout << "옵 - 총x : " << obj.object_position.x - player->bullet_pos.x << endl << endl;

					cout << "총알y : " << player->bullet_pos.y << endl;
					cout << "옵젝y : " << obj.object_position.y << endl;
					cout << "총 - 옵y : " << player->bullet_pos.y - obj.object_position.y << endl;
					cout << "옵 - 총y : " << obj.object_position.y - player->bullet_pos.y << endl;
					
					if (-85 < player->bullet_pos.x - obj.object_position.x &&
						player->bullet_pos.x - obj.object_position.x < 20 &&
						-110 < player->bullet_pos.y - obj.object_position.y &&
						player->bullet_pos.y - obj.object_position.y < -20)
					{
						cout << "too close to 가로벽." << endl;
						show_bullet = false;
						player->player_state.collide = true;
						player->bullet_pos.x = -float(ground_size);
						player->bullet_pos.y = -float(ground_size);
					}
					else if (-85 < player->bullet_pos.x - obj.object_position.x &&
						player->bullet_pos.x - obj.object_position.x < 20 &&
						0 < player->bullet_pos.y - obj.object_position.y &&
						player->bullet_pos.y - obj.object_position.y < 33)
					{
						cout << "too close to 가로벽." << endl;
						show_bullet = false;
						player->player_state.collide = true;
						player->bullet_pos.x = -float(ground_size);
						player->bullet_pos.y = -float(ground_size);
					}
				}
			}


			// bullet과 objects가 거리가 있을 때.
			if (abs(obj.object_position.x - player->bullet_pos.x) < 50
				&& abs(obj.object_position.y - player->bullet_pos.y) < 50)
			{
				if (obj.object_type != 3 && obj.object_type != 4)
				{
					if (obj.object_type == 0) // 돌
					{
						if (player->bullet_pos.x - obj.object_position.x > 25) {
							continue;
						}
						else if (player->bullet_pos.y - obj.object_position.y > 20) {
							continue;
						}
						else {
							show_bullet = false;
							cout << "collide rock!" << endl;
							cout << player->bullet_pos.x << " " << player->bullet_pos.y << endl;
							cout << obj.object_position.x << " " << obj.object_position.y << endl;
							cout << "=============================" << endl;
							player->player_state.collide = true;
							player->bullet_pos.x = -float(ground_size);
							player->bullet_pos.y = -float(ground_size);
						}
					}
					else if (obj.object_type == 1)	// 세로 벽
					{
						if (player->bullet_pos.x - obj.object_position.x > 1) {
							continue;
						}
						show_bullet = false;
						cout << "collide 세로벽!" << endl;
						cout << player->bullet_pos.x << " " << player->bullet_pos.y << endl;
						cout << obj.object_position.x << " " << obj.object_position.y << endl;
						cout << "=============================" << endl;
						player->player_state.collide = true;
						player->bullet_pos.x = -float(ground_size);
						player->bullet_pos.y = -float(ground_size);
					}
					else if (obj.object_type == 2)	// 가로 벽
					{
						if (player->bullet_pos.y - obj.object_position.y > 1) {
							continue;
						}
						show_bullet = false;
						cout << "collide 가로벽!" << endl;
						cout << player->bullet_pos.x << " " << player->bullet_pos.y << endl;
						cout << obj.object_position.x << " " << obj.object_position.y << endl;
						cout << "=============================" << endl;
						player->player_state.collide = true;
						player->bullet_pos.x = -float(ground_size);
						player->bullet_pos.y = -float(ground_size);
					}
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
		player->bullet_pos.x += bulletVelo.x;
		player->bullet_pos.y += bulletVelo.y;
		cout << "날으는 총알 x : " << player->bullet_pos.x << "\t y : " << player->bullet_pos.y << endl;
		
		if (player->bullet_pos.x > ground_size / 2 ||
			player->bullet_pos.x < -(ground_size / 2) ||
			player->bullet_pos.y > ground_size / 2 ||
			player->bullet_pos.y < -(ground_size / 2) )
		{			
			cout << endl << endl << "bullet out" << endl << endl;
			show_bullet = false;
			player->bullet_pos.x = -float(ground_size);
			player->bullet_pos.y = -float(ground_size);
		}
	}
	else if (player->player_state.gun_fired == false && show_bullet == false)
	{
		show_bullet = true;
	}
}
