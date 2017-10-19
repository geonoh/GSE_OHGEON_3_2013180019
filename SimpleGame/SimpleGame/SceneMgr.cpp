#include "stdafx.h"
#include "SceneMgr.h"
#include <time.h>
#include <stdlib.h>


SceneMgr::SceneMgr()
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) {
		m_objects[i] = new Object(
			-250 + rand() % 500, -250 + rand() % 500,
			-250 + rand() % 500, rand() % 10, 1.f, 1.f, 1.f, 0.0f
		);

		m_objects[i]->Set_velocity(-1 + rand() % 2, -1 + rand() % 2
			, -1 + rand() % 2, 0.1f);
	}
}


SceneMgr::~SceneMgr()
{
	printf("소멸스\n");
	delete[] m_objects;
}

void SceneMgr::Update() {
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) {
		m_objects[i]->Update(0);
	}
}

void SceneMgr::CollideCheck() {
	float pre_min_x = 0.0f;
	float pre_min_y = 0.0f;
	float pre_max_x = 0.0f;
	float pre_max_y = 0.0f;

	float nex_min_x = 0.0f;
	float nex_min_y = 0.0f;
	float nex_max_x = 0.0f;
	float nex_max_y = 0.0f;
	for (int i = 0; i < MAX_OBJECTS_COUNT - 1; ++i) {
		// MIN x,y  MAX x,y
		pre_min_x = m_objects[i]->Get_x() - (m_objects[i]->Get_size() / 2);
		pre_min_y = m_objects[i]->Get_y() - (m_objects[i]->Get_size() / 2);
		pre_max_x = m_objects[i]->Get_x() + (m_objects[i]->Get_size() / 2);
		pre_max_y = m_objects[i]->Get_y() + (m_objects[i]->Get_size() / 2);

		//nex_min_x = m_objects[i + 1]->Get_x() - (m_objects[i + 1]->Get_size() / 2);
		//nex_min_y = m_objects[i + 1]->Get_y() - (m_objects[i + 1]->Get_size() / 2);
		//nex_max_x = m_objects[i + 1]->Get_x() + (m_objects[i + 1]->Get_size() / 2);
		//nex_max_y = m_objects[i + 1]->Get_y() + (m_objects[i + 1]->Get_size() / 2);

		if (((pre_min_x <= m_objects[i + 1]->Get_x()) && (m_objects[i + 1]->Get_x() <= pre_max_x)) &&	// x가 범위 안에 있고,
			((pre_min_y <= m_objects[i + 1]->Get_y()) && (m_objects[i + 1]->Get_y() <= pre_max_y)))		// y가 범위 안에 있으면
		{
			m_objects[i]->SetRed();
			m_objects[i + 1]->SetRed();
			printf("%d번과 %d번 충돌!\n", i, i + 1);
		}
	}
}