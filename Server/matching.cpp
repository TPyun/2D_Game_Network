#include "matching.h"

//find_match_3p �Լ�
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

				a.second->room_num = room_num;	//PP�� room_num�� �־���

				cout << "�÷��̾� �̸�: " << *a.second->player_info.name << endl;
			}
		}
		return true;
	}
	else {
		return false;
	}
}