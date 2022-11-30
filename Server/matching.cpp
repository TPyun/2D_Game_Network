#include "matching.h"

//find_match_3p 함수
bool Matching::find_match_3p(int room_num)
{
	int cnt = 0;
	for (auto& a : player_list) {
		if (a.second->player_state.game_state == 1)
			cnt++;
	}
	if (cnt >= MAX_CLIENT_IN_ROOM) {
		for (auto& a : player_list) {
			if (a.second->player_state.game_state == 1) {
				a.second->player_state.game_state = 2;

				a.second->room_num = room_num;	//PP에 room_num을 넣어줌

				cout << "플레이어 이름: " << *a.second->player_info.name << endl;
			}
		}
		return true;
	}
	else {
		return false;
	}
}