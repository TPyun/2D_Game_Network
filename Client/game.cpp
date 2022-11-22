#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include <Windows.h>

using namespace std;

void Game::clearRenderer()
{
	// Clear renderer
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}
void Game::updateRenderer()
{
	// Update screen
	SDL_RenderPresent(renderer);
}

void Game::initVariables()
{
	this->window = nullptr;
}
void Game::initWindow()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		cout << "Could not initialize SDL!" << SDL_GetError() << endl;
	}
	if (TTF_Init() < 0) {
		cout << "Could not initialize SDL!" << SDL_GetError() << endl;
	}


	this->iMonitorWidth = GetSystemMetrics(SM_CXSCREEN);    // 모니터 가로 해상도
	this->iMonitorHeight = GetSystemMetrics(SM_CYSCREEN); //새로  해상도

	//Create Window
	this->window = SDL_CreateWindow("", iMonitorWidth / 2 - WIDTH / 2, iMonitorHeight / 2 - HEIGHT / 2, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		cout << "Could not create window!" << SDL_GetError() << endl;;
	}

	// Create renderer
	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == NULL) {
		cout << "Could not create renderer!" << SDL_GetError() << endl;
	}
}

SDL_Texture* Game::loadImage(const char* directory)
{
	SDL_Texture* Tex;
	tmpSurface = IMG_Load(directory);
	if (!tmpSurface) {
		cout << "Image not loaded" << directory << endl;
	}
	Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	return Tex;
}
void Game::loadWavs()
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	gunsound = Mix_LoadWAV("Sounds/gunsound.wav");
}
void Game::loadFont()
{
	font = TTF_OpenFont("arial.ttf", 16);
	if (!font) {
		printf("Could not open font! (%s)\n", TTF_GetError());
	}
}

double Game::calcAngleFromPoints(TF first_point, TF second_point)
{
	float fAngle;
	float fdX = first_point.x - second_point.x;
	float fdY = first_point.y - second_point.y;
	float dRad = atan2(fdY, fdX);
	return fAngle = (dRad * 180) / 3.14159265;
}

int Game::Timer(int start_time, int delay)
{
	int curr_time = clock();
	if (curr_time - start_time > delay && start_time != 0) {
		//cout << curr_time << " " << start_time << endl;
		return 1;
	}
	else
		return 0;
}

void Game::collide_check(float a, float b, float c, float d, float e, float f, float g, float h)//x,y x,y a,b a,b
{
	if (a < g && c > e && b < h && d > f) {
		cout << "collide" << endl;
	}
}

void Game::mouseEvent_menu()
{
	//get mouse coordinates 
	SDL_GetGlobalMouseState(&mouse_X, &mouse_Y);

	mouse_point.x = mouse_X;
	mouse_point.y = mouse_Y;

	//모니터 해상도 좌표를 게임 게임 창 좌표 기준으로 변환시킴
	SDL_GetWindowPosition(window, &window_moved_x, &window_moved_y);
	mouse_point.x -= window_moved_x;
	mouse_point.y -= window_moved_y;
}

void Game::drawBackground()
{
	destR.w = background_size + 1000;
	destR.h = background_size;
	destR.x = WIDTH / 2 - (background_size + 1000) / 2;
	destR.y = HEIGHT / 2 - background_size / 2;

	//cout << MyCharPos.x << " " << MyCharPos.y << endl;

	//Draw Background
	SDL_RenderCopy(renderer, backTex, NULL, &destR);
}
void Game::drawGround()
{
	destR.w = ground_size;
	destR.h = ground_size;
	destR.x = WIDTH / 2 - ground_size / 2 - MyCharPos.x;
	destR.y = HEIGHT / 2 - ground_size / 2 - MyCharPos.y;

	//Draw Background
	SDL_RenderCopy(renderer, groundTex, NULL, &destR);
}
void Game::drawCharacter()
{
	destR.w = player_size;
	destR.h = player_size;
	destR.x = WIDTH / 2 - player_size / 2; //MyCharPos.x - player_size / 2;
	destR.y = HEIGHT / 2 - player_size / 2; //MyCharPos.y - player_size / 2;

	center.x = player_size / 2;
	center.y = player_size / 2;

	SDL_RenderCopyEx(renderer, black_playerTex, NULL, &destR, my_char_angle + 90, &center, SDL_FLIP_NONE);
}
void Game::drawHealthbar()
{
	destR.w = (float)my_health / 2.5f;
	destR.h = 10;
	destR.x = WIDTH / 2 - 20;
	destR.y = HEIGHT / 2 - 35;
	if (my_health > 50) {
		SDL_RenderCopy(renderer, greenTex, NULL, &destR);
	}
	else if (my_health > 25) {
		SDL_RenderCopy(renderer, yellowTex, NULL, &destR);
	}
	else {
		SDL_RenderCopy(renderer, redTex, NULL, &destR);
	}
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_Rect r = { WIDTH / 2 - 20, HEIGHT / 2 - 35, 40, 10 };
	SDL_RenderDrawRect(renderer, &r);
}
void Game::drawBullet()
{
	bullet_angle = 3.14159265 * 2 * fired_angle / 360;
	//총알이 보일 때
	if (show_bullet) {

		myBulletVelo.x = cos(bullet_angle) * 6000 * delayTime;
		myBulletVelo.y = sin(bullet_angle) * 6000 * delayTime;

		myBulletPos.x += (myBulletVelo.x + MyVelo.x);
		myBulletPos.y += (myBulletVelo.y + MyVelo.y);

		destR.w = bullet_size;
		destR.h = bullet_size;
		destR.x = myBulletPos.x - bullet_size / 2;
		destR.y = myBulletPos.y - bullet_size / 2;

		center.x = bullet_size / 2;
		center.y = bullet_size / 2;

		//SDL_RenderCopy(renderer, bulletTex, NULL, &destR);
		SDL_RenderCopyEx(renderer, bulletTex, NULL, &destR, fired_angle, &center, SDL_FLIP_NONE);

		if (myBulletPos.x > WIDTH || myBulletPos.x < 0 || myBulletPos.y >HEIGHT || myBulletPos.y < 0) {
			show_bullet = false;
		}
	}
	//총알이 보이지 않을 때
	else {
		myBulletPos.x = WIDTH / 2 + cos(bullet_angle) * 75;
		myBulletPos.y = HEIGHT / 2 + sin(bullet_angle) * 75;
		//캐릭터가 발사한 순간의 각도를 
		fired_angle = my_char_angle;
	}
}
void Game::drawFlash()
{
	flash_size.x = 150;
	flash_size.y = 80;

	if (gun_flash) {
		srcR.w = flash_sprite_w / 4;
		srcR.h = flash_sprite_h / 3;

		flash_i += 1;
		if (flash_i % 2 == 0) {
			srcR.x += srcR.w;
			srcR.y = 320;
		}

		if (srcR.x >= flash_sprite_w - flash_sprite_w % 4) {
			srcR.x = 0;

			flash_i = 0;
			gun_flash = false;
		}

		destR.w = flash_size.x;
		destR.h = flash_size.y;
		destR.x = WIDTH / 2 - flash_size.x / 2 + cos(bullet_angle) * 75;
		destR.y = HEIGHT / 2 - flash_size.y / 2 + sin(bullet_angle) * 75;

		center.x = flash_size.x / 2;
		center.y = flash_size.y / 2;

		SDL_RenderCopyEx(renderer, flashTex, &srcR, &destR, fired_angle, &center, SDL_FLIP_NONE);
	}
	else {
		flash_angle = my_char_angle;
	}
}

void Game::drawCrosshair()
{
	//마우스 좌표 먹이기
	destR.w = crosshair_size;
	destR.h = crosshair_size;
	destR.x = mouse_point.x - crosshair_size / 2;
	destR.y = mouse_point.y - crosshair_size / 2;

	//Draw Target
	SDL_RenderCopy(renderer, targetTex, NULL, &destR);
	//SDL_RenderCopyEx(renderer, targetTex, NULL, &destR, my_char_angle, &center, SDL_FLIP_NONE);
}
void Game::drawText(int x, int y, char text[], SDL_Color color)
{
	if (!font) {
		printf("Could not open font! (%s)\n", TTF_GetError());
		return;
	}
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
	if (!surface) {
		//cout << "no surface" << endl;
		return;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect r = { x, y, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, NULL, &r);
	//SDL_DestroyTexture(texture);
	//TTF_CloseFont(font);
}
void Game::drawWeaponList()
{
	destR.w = 100;
	destR.h = 100;
	if (weapon_type == 0) {
		destR.x = WIDTH / 2 - 151;
	}
	else if (weapon_type == 1) {
		destR.x = WIDTH / 2 - 50;
	}
	else if (weapon_type == 2) {
		destR.x = WIDTH / 2 + 51;
	}
	destR.y = HEIGHT - 100;
	SDL_RenderCopy(renderer, whiteTex, NULL, &destR);
	
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_Rect r = { WIDTH / 2 - 151, HEIGHT - 100, 100, 100 };
	SDL_RenderDrawRect(renderer, &r);
	r = { WIDTH / 2 - 50, HEIGHT - 100, 100, 100 }; 
	SDL_RenderDrawRect(renderer, &r);
	r = { WIDTH / 2 + 51, HEIGHT - 100, 100, 100 };
	SDL_RenderDrawRect(renderer, &r);

	destR.x = WIDTH / 2 - 151;
	destR.y = HEIGHT - 100;
	SDL_RenderCopy(renderer, pistolTex, NULL, &destR);
	destR.x = WIDTH / 2 - 50;
	destR.y = HEIGHT - 100;
	SDL_RenderCopy(renderer, rifleTex, NULL, &destR);
	destR.x = WIDTH / 2 + 51;
	destR.y = HEIGHT - 100;
	SDL_RenderCopy(renderer, sniperTex, NULL, &destR);
	
	
	destR.w = 20;
	destR.h = 20;
	destR.x = WIDTH / 2 - 140;
	destR.y = HEIGHT - 100;
	SDL_RenderCopy(renderer, infinity_Tex, NULL, &destR);
	
	SDL_Color color = { 0, 0, 0 };
	string tmp = to_string(rifle_ammo);
	char const* num_char = tmp.c_str();
	drawText(WIDTH / 2 - 40, HEIGHT - 100, (char*)num_char, color);
	tmp = to_string(sniper_ammo);
	num_char = tmp.c_str();
	drawText(WIDTH / 2 + 60, HEIGHT - 100, (char*)num_char, color);

}

void Game::drawObstacle()
{
	for (int i = 0; i < 20; i++) {
		destR.x = WIDTH / 2 - 100 / 2 - MyCharPos.x + created_objects[i].object_position.x;
		destR.y = HEIGHT / 2 - 100 / 2 - MyCharPos.y + created_objects[i].object_position.y;
		
		//cout << created_objects[i].object_position.x << " | " << created_objects[i].object_position.y << endl;
		//cout << MyCharPos.x << " / " << MyCharPos.y << endl;
		
		if (created_objects[i].object_type == 0) {		//rock
			destR.w = 74;
			destR.h = 74;
			SDL_RenderCopy(renderer, rock_Tex, NULL, &destR);
		}
		else if (created_objects[i].object_type == 1) {		//wall 1
			destR.w = 23;
			destR.h = 95;			
			SDL_RenderCopy(renderer, wall1_Tex, NULL, &destR);
		}
		else if (created_objects[i].object_type == 2){		//wall 2
			destR.w = 95;
			destR.h = 23;
			SDL_RenderCopy(renderer, wall2_Tex, NULL, &destR);
		}
	}
}

void Game::drawMenu()
{
	SDL_Color color = { 255, 255, 255 };
	drawBackground();

	if (!connect_server) {
		//Press enter to return
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB) {
			if (text_in_height == 100) {
				text_in_height += 100;
			}
			else if (text_in_height == 200) {
				text_in_height += 100;
			}
			else if (text_in_height == 300) {
				text_in_height = 100;
			}
			text_in = "";
		}
		//Press button to add text
		else if (event.type == SDL_TEXTINPUT) {
			text_in += event.text.text;
			if (text_in_height == 100) {
				strcpy(IPAdress, text_in.c_str());
			}
			else if (text_in_height == 200) {
				strcpy(Port, text_in.c_str());
			}
			else if (text_in_height == 300) {
				if(strlen(Name) < 20)
					strcpy(Name, text_in.c_str());
			}
		}
		//Press backspace to erase
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && text_in.size()) {
			text_in.pop_back();
			if (text_in_height == 100) {
				strcpy(IPAdress, text_in.c_str());
			}
			else if (text_in_height == 200) {
				strcpy(Port, text_in.c_str());
			}
			else if (text_in_height == 300) {
				strcpy(Name, text_in.c_str());
			}
		}
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
			connect_server = true;
		}
		//Exit event
		else if (event.type == SDL_QUIT) {
			done = 1;
		}

		//Draw subject
		drawText(100, 100, (char*)"IP Adress", color);
		drawText(100, 200, (char*)"Port", color);
		drawText(100, 300, (char*)"Name", color);

		// Draw input rect
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_Rect r = { 200, text_in_height, 200, 20 };
		SDL_RenderDrawRect(renderer, &r);

		drawText(200, 100, (char*)IPAdress, color);
		drawText(200, 200, (char*)Port, color);
		drawText(200, 300, (char*)Name, color);
		
		drawText(170, 400, (char*)"Enter to connect", color);
	}
	else {
		if (!waiting_match) {
			drawText(200, 100, (char*)Name, color);
			drawText(200, 200, (char*)"Press enter to find match", color);

			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
				find_match = true;
				waiting_match = true;
				//curr_state = 1;
			}
		}
	}
	if (waiting_match) {
		drawText(200, 200, (char*)"Finding match...", color);
	}
}


void Game::drawIngame()
{
	drawBackground();
	drawGround();
	drawObstacle();
	drawBullet();
	drawFlash();
	drawCharacter();
	drawHealthbar();
	drawCrosshair();
	drawWeaponList();
}

void Game::update()
{
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT) {
		done = 1;
	}
	clearRenderer();

	if (curr_state == 0) {
		drawMenu();
	}
	else if (curr_state == 1) {
		mouseEvent_ingame();
		//keyEvent_ingame();
		drawIngame();
	}

	updateRenderer();
}

void Game::send_name()
{
	/*cout << Name << endl;

	char name_buf[NAMESIZE]{};
	int retval;
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	strncpy(name_buf, Name, strlen(Name));
	retval = send(sock, name_buf, NAMESIZE, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send_name()");
	}*/
}

Game::Game()
{
	this->initVariables();
	this->initWindow();

	black_playerTex = this->loadImage("Images/Player_Black.png");
	red_playerTex = this->loadImage("Images/Player_Red.png");
	blue_playerTex = this->loadImage("Images/Player_Blue.png");
	
	targetTex = this->loadImage("Images/Target.png");
	backTex = this->loadImage("Images/Space.jpg");
	groundTex = this->loadImage("Images/Ground.png");
	bulletTex = this->loadImage("Images/Bullet.png");
	flashTex = this->loadImage("Images/Flash.png");
	
	pistolTex = this->loadImage("Images/Pistol.png");
	rifleTex = this->loadImage("Images/Rifle.png");
	sniperTex = this->loadImage("Images/Sniper.png");
	
	whiteTex = this->loadImage("Images/White.png");
	greenTex = this->loadImage("Images/Green.png");
	yellowTex = this->loadImage("Images/Yellow.png");
	redTex = this->loadImage("Images/Red.png");
	
	pistol_ammoTex = this->loadImage("Images/Pistol_ammo.png");
	rifle_ammoTex = this->loadImage("Images/Rifle_ammo.png");
	sniper_ammoTex = this->loadImage("Images/Sniper_ammo.png");

	infinity_Tex = this->loadImage("Images/Infinity.png");

	rock_Tex = this->loadImage("Images/Rock.png");
	wall1_Tex = this->loadImage("Images/Wall1.png");
	wall2_Tex = this->loadImage("Images/Wall2.png");
	
	this->loadWavs();
	this->loadFont();

	//Initialize player's position to middle of window
	MyCharPos.x = 0;
	MyCharPos.y = 0;

	middle_pos.x = WIDTH / 2;
	middle_pos.y = HEIGHT / 2;
}
Game::~Game()
{
	Mix_FreeChunk(gunsound);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}


void Game::mouseEvent_ingame()
{
	//get mouse coordinates 
	SDL_GetGlobalMouseState(&mouse_X, &mouse_Y);

	mouse_point.x = mouse_X;
	mouse_point.y = mouse_Y;

	//모니터 해상도 좌표를 게임 게임 창 좌표 기준으로 변환시킴
	SDL_GetWindowPosition(window, &window_moved_x, &window_moved_y);
	mouse_point.x -= window_moved_x;
	mouse_point.y -= window_moved_y;

	//Get Character Angle by Mouse's Coordinates
	my_char_angle = calcAngleFromPoints(mouse_point, middle_pos);

	//마우스 왼 버튼 누르면 발사
	/*if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT && gun_fired == false) {
			gun_fired = true;
			fired_time = clock();

			show_bullet = true;
			gun_flash = true;

			Mix_PlayChannel(-1, gunsound, 0);
		}
	}*/
	//총이 발사되고 타이머 작동시켜서 1초 뒤 다시 발사 가능
	/*if (gun_fired) {
		if (Timer(fired_time, 100) == 1) {
			gun_fired = false;
			fired_time = 0;
		}
	}*/
}

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

