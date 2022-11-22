#pragma once

#define SERVERPORT	9000
#define BUFSIZE		4096
#define NAMESIZE	20

typedef struct TWO_Floats {
	float x;
	float y;
}TF;

typedef struct TWO_Ints {
	int x;
	int y;
}TI;

typedef struct players_state {
	int hp;
	int gun_type;
	int bullet[3];
	TI object_position;
	TF player_position;
	bool gun_fired;
	int game_state;		// 0:main, 1:find_match, 2:in_game, 3:lose, 4:win
}PS;

typedef struct players_info {
	int player_color[3];
	char name[3][20];
}PI;

typedef struct players_profile {
	PS player_state;
	PI player_info;
	int room_num;
}PP;

typedef struct created_object
{
	int object_type;	// 0:바위, 1:벽, 2:아이템
	TI object_position;
}CO;

// key event

//void Game::keyEvent_ingame()
//{
//	//123으로 weapon_type 변경
//	if (event.type == SDL_KEYDOWN) {
//		if (event.key.keysym.sym == SDLK_1) {
//			weapon_type = 0;
//		}
//		else if (event.key.keysym.sym == SDLK_2) {
//			weapon_type = 1;
//		}
//		else if (event.key.keysym.sym == SDLK_3) {
//			weapon_type = 2;
//		}
//	}
//
//	//Fix keyboard status when key pressed
//	if (event.type == SDL_KEYDOWN) {
//		if (event.key.keysym.sym == SDLK_w) {
//			w_Pressed = true;
//		}
//		if (event.key.keysym.sym == SDLK_a) {
//			a_Pressed = true;
//		}
//		if (event.key.keysym.sym == SDLK_s) {
//			s_Pressed = true;
//		}
//		if (event.key.keysym.sym == SDLK_d) {
//			d_Pressed = true;
//		}
//	}
//	if (event.type == SDL_KEYUP) {
//		if (event.key.keysym.sym == SDLK_w) {
//			w_Pressed = false;
//		}
//		if (event.key.keysym.sym == SDLK_a) {
//			a_Pressed = false;
//		}
//		if (event.key.keysym.sym == SDLK_s) {
//			s_Pressed = false;
//		}
//		if (event.key.keysym.sym == SDLK_d) {
//			d_Pressed = false;
//		}
//	}
//
//	//Add speed to character by status of key
//	float minimun_movement = 5.f * delayTime;
//	if (w_Pressed) {
//		MyVelo.y -= minimun_movement;
//	}
//	if (s_Pressed) {
//		MyVelo.y += minimun_movement;
//	}
//	if (a_Pressed) {
//		MyVelo.x -= minimun_movement;
//	}
//	if (d_Pressed) {
//		MyVelo.x += minimun_movement;
//	}
//
//	float MaxVelo = 200.f * delayTime;
//	float MinVelo = -200.f * delayTime;
//
//	//Limit Velocity
//	if (MyVelo.x > MaxVelo) {
//		MyVelo.x = MaxVelo;
//	}
//	if (MyVelo.x < MinVelo) {
//		MyVelo.x = MinVelo;
//	}
//	if (MyVelo.y > MaxVelo) {
//		MyVelo.y = MaxVelo;
//	}
//	if (MyVelo.y < MinVelo) {
//		MyVelo.y = MinVelo;
//	}
//
//	//Character Speed Friction
//	float friction = 2.f * delayTime;
//
//	if (MyVelo.x > 0) {
//		MyVelo.x -= friction;
//	}
//	if (MyVelo.x < 0) {
//		MyVelo.x += friction;
//	}
//	if (MyVelo.y > 0) {
//		MyVelo.y -= friction;
//	}
//	if (MyVelo.y < 0) {
//		MyVelo.y += friction;
//	}
//
//	//offset ignorable velocity
//	if (-minimun_movement / 2 <= MyVelo.x && MyVelo.x <= minimun_movement / 2) {
//		MyVelo.x = 0;
//	}
//	if (-minimun_movement / 2 <= MyVelo.y && MyVelo.y <= minimun_movement / 2) {
//		MyVelo.y = 0;
//	}
//
//	//Keep Character Visible in Window
//	if (MyCharPos.x > ground_size / 2 - player_size / 2) {
//		MyCharPos.x = ground_size / 2 - player_size / 2;
//		MyVelo.x = 0;
//	}
//	else if (MyCharPos.x < -ground_size / 2 + player_size / 2) {
//		MyCharPos.x = -ground_size / 2 + player_size / 2;
//		MyVelo.x = 0;
//	}
//	if (MyCharPos.y > ground_size / 2 - player_size / 2) {
//		MyCharPos.y = ground_size / 2 - player_size / 2;
//		MyVelo.y = 0;
//	}
//	else if (MyCharPos.y < -ground_size / 2 + player_size / 2) {
//		MyCharPos.y = -ground_size / 2 + player_size / 2;
//		MyVelo.y = 0;
//	}
//	//add velocity to XY coordinates
//	MyCharPos.x += MyVelo.x;
//	MyCharPos.y += MyVelo.y;
//}

//mouse event

//void Game::mouseEvent_ingame()
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
//	//마우스 왼 버튼 누르면 발사
//	if (event.type == SDL_MOUSEBUTTONDOWN) {
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
//	//총이 발사되고 타이머 작동시켜서 1초 뒤 다시 발사 가능
//	if (gun_fired) {
//		if (Timer(fired_time, 100) == 1) {
//			gun_fired = false;
//			fired_time = 0;
//		}
//	}
//}
