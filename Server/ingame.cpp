#include "ingame.h"

void ingame::create_object()		// �ʱ� �� ���� �����ϴ� �Լ� (����, ��, ������, �÷��̾� ���� ��)
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

	std::cout << "create_map ���� �Ϸ�" << std::endl;
}
