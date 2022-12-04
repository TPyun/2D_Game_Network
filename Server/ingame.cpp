#include "ingame.h"
#include "global.h"

void Ingame::create_object()		// �ʱ� �� ���� �����ϴ� �Լ� (����, ��, ������, �÷��̾� ���� ��)
{
	std::cout << "create_map ����" << std::endl;

	for (int i = 0; i < 10; ++i) {
		objects[i].object_position.x = uid(dre);
		objects[i].object_position.y = uid(dre);/*
		std::cout << i+1 << "��° rock" << std::endl;
		std::cout << "������Ʈ Ÿ�� : " << objects[i].object_type << std::endl;
		std::cout << "x : " << objects[i].object_position.x << std::endl;
		std::cout << "y : " << objects[i].object_position.y << std::endl;*/
	}
	for (int i = 10; i < 20; ++i) {
		objects[i].object_type = 1;
		objects[i].object_position.x = uid(dre);
		objects[i].object_position.y = uid(dre);/*
		std::cout << i + 1 << "��° wall" << std::endl;
		std::cout << "������Ʈ Ÿ�� : " << objects[i].object_type << std::endl;
		std::cout << "x : " << objects[i].object_position.x << std::endl;
		std::cout << "y : " << objects[i].object_position.y << std::endl;*/
	}
	for (int i = 20; i < 30; ++i) {
		objects[i].object_type = 2;
		objects[i].object_position.x = uid(dre);
		objects[i].object_position.y = uid(dre);/*
		std::cout << i + 1 << "��° wall" << std::endl;
		std::cout << "������Ʈ Ÿ�� : " << objects[i].object_type << std::endl;
		std::cout << "x : " << objects[i].object_position.x << std::endl;
		std::cout << "y : " << objects[i].object_position.y << std::endl;*/
	}

	for (int i = 30; i < 40; ++i) {			// rifle
		objects[i].object_type = 3;
		objects[i].object_position.x = uid(dre);
		objects[i].object_position.y = uid(dre);
	}
	for (int i = 40; i < 50; ++i) {			// sniper
		objects[i].object_type = 4;
		objects[i].object_position.x = uid(dre);
		objects[i].object_position.y = uid(dre);
	}

	// object ��ħ üũ
	for (int i = 0; i <= MAXITEM-1; ++i) {
		for (int j = i+1; j <= MAXITEM; ++j) {
			if (objects[i].object_position.x == objects[j].object_position.x && objects[i].object_position.y == objects[j].object_position.y) {
				objects[i].object_position.x = uid(dre);
				objects[i].object_position.y = uid(dre);
			}
		}
	}

	std::cout << "create_map ���� �Ϸ�" << std::endl;
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
	//cout << "�÷��̾� ��ġ: " << pos.x << ", " << pos.y << endl;
}

void Ingame::collide_check(PP* player, CI* input, TF bullet)
{
	// �÷��̾�� objects ���� collide_check
	for (auto& obj : objects)
	{
		if (abs(obj.object_position.x - player->player_state.player_position.x) < 100
			&& abs(obj.object_position.y - player->player_state.player_position.y) < 100)
		{
			//��
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
			//��1
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
			//��2
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
		}
	}

	// �÷��̾�� bullet ���� collide_check
	

	// bullet�� objects ���� collidecheck
	for(auto& obj : objects)
	{ }
}

//void Ingame::mouseEvent_ingame()
//{
//	//get mouse coordinates 
//	SDL_GetGlobalMouseState(&mouse_X, &mouse_Y);
//
//	mouse_point.x = mouse_X;
//	mouse_point.y = mouse_Y;
//
//	//����� �ػ� ��ǥ�� ���� ���� â ��ǥ �������� ��ȯ��Ŵ
//	SDL_GetWindowPosition(window, &window_moved_x, &window_moved_y);
//	mouse_point.x -= window_moved_x;
//	mouse_point.y -= window_moved_y;
//
//	//Get Character Angle by Mouse's Coordinates
//	my_char_angle = calcAngleFromPoints(mouse_point, middle_pos);
//
//	//������ ������ struct ������ ���콺 ��ǥ ����, clicked
//	input.mouse_rotation = my_char_angle;
//	//���콺 �� ��ư ������ �߻�
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
//	//���� �߻�ǰ� Ÿ�̸� �۵����Ѽ� 1�� �� �ٽ� �߻� ����awd
//	if (gun_fired) {
//		if (Timer(fired_time, 100) == 1) {
//			gun_fired = false;
//			fired_time = 0;
//		}
//	}
//}
