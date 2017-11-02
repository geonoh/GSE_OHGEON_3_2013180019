#include "stdafx.h"
#include "SceneMgr.h"


SceneMgr::SceneMgr(float x, float y )
{	
	// Initialize Renderer

	renderer = new Renderer(x, y);
	if (!renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	//m_objects.push_back(Object());

	srand((unsigned)time(NULL));


	//AddActorObject(100, 100, OBJECT_BUILDING);

}


SceneMgr::~SceneMgr()
{
	printf("소멸스\n");
	//delete[] m_objects;
	m_objects.clear();
	delete renderer;
}

void SceneMgr::Update() {
	/*DWORD start_time;
	start_time = timeGetTime();*/

	CollideCheck();


	for (int i = 0; i < m_objects.size(); ++i) {
		// 생명이 있을때만
		if (m_objects[i].GetLife() > 0) {
			m_objects[i].Update(0);
		}
		else {
			//printf("%d의 오브젝트 체력 : %f\n", i, m_objects[i]->GetLife());
		}
	}
}

void SceneMgr::CollideCheck() {
	float left_a = 0;
	float right_a = 0;
	float top_a = 0;
	float bottom_a = 0;


	float left_b = 0;
	float right_b = 0;
	float top_b = 0;
	float bottom_b = 0;

	if (m_objects.size() > 0) {
		for (int i = 0; i < m_objects.size() - 1; ++i) {
			if ((m_objects[i].GetLife() > 0) && (m_objects[i + 1].GetLife() > 0)) {
				left_a = m_objects[i].Get_x() - m_objects[i].Get_size() / 2;
				right_a = m_objects[i].Get_x() + m_objects[i].Get_size() / 2;
				bottom_a = m_objects[i].Get_y() - m_objects[i].Get_size() / 2;
				top_a = m_objects[i].Get_y() + m_objects[i].Get_size() / 2;

				left_b = m_objects[i + 1].Get_x() - m_objects[i + 1].Get_size() / 2;
				right_b = m_objects[i + 1].Get_x() + m_objects[i + 1].Get_size() / 2;
				bottom_b = m_objects[i + 1].Get_y() - m_objects[i + 1].Get_size() / 2;
				top_b = m_objects[i + 1].Get_y() + m_objects[i + 1].Get_size() / 2;

				if (left_a > right_b)continue;
				if (right_a < left_b)continue;
				if (top_a < bottom_b)continue;
				if (bottom_a > top_b)continue;

				m_objects[i].SetRed();
				m_objects[i + 1].SetRed();
				//printf("%d번과 %d번 충돌!\n", i, i + 1);
			}
		}
	}


	// 충돌이 끝난물체는 다시 흰색으로
	if (m_objects.size() > 0) {
		for (int i = 0; i < m_objects.size() - 1; ++i) {
			left_a = m_objects[i].Get_x() - m_objects[i].Get_size() / 2;
			right_a = m_objects[i].Get_x() + m_objects[i].Get_size() / 2;
			bottom_a = m_objects[i].Get_y() - m_objects[i].Get_size() / 2;
			top_a = m_objects[i].Get_y() + m_objects[i].Get_size() / 2;

			left_b = m_objects[i + 1].Get_x() - m_objects[i + 1].Get_size() / 2;
			right_b = m_objects[i + 1].Get_x() + m_objects[i + 1].Get_size() / 2;
			bottom_b = m_objects[i + 1].Get_y() - m_objects[i + 1].Get_size() / 2;
			top_b = m_objects[i + 1].Get_y() + m_objects[i + 1].Get_size() / 2;

			if (m_objects[i].is_collide) {
				if (left_a > right_b) {
					m_objects[i].SetWhite();
					m_objects[i + 1].SetWhite();
				}
				if (right_a < left_b) {
					m_objects[i].SetWhite();
					m_objects[i + 1].SetWhite();
				}
				if (top_a < bottom_b) {
					m_objects[i].SetWhite();
					m_objects[i + 1].SetWhite();
				}
				if (bottom_a > top_b) {
					m_objects[i].SetWhite();
					m_objects[i + 1].SetWhite();
				}
			}
		}
	}
}
void SceneMgr::SceneRender(float x, float y, float z, float size, float r, float g, float b, float a) {
	renderer->DrawSolidRect(x, y, z, size, r, g, b, a);
}
void SceneMgr::SceneRender() {
	// 배경들 움직여버리기
	for (int i = 0; i < m_objects.size(); ++i) {\
		if (m_objects[i].GetLife() > 0) {
			renderer->DrawSolidRect(m_objects[i].Get_x(),
				m_objects[i].Get_y(), m_objects[i].Get_z(),
				m_objects[i].Get_size(), m_objects[i].Get_R(),
				m_objects[i].Get_G(), m_objects[i].Get_B(),
				m_objects[i].Get_A());
		}
	}
}


void SceneMgr::MouseInput(int x, int y, int num) {
	if (num == MAX_OBJECTS_COUNT) {
		m_objects.clear();
		/*for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) {
			m_objects[i].SetLife(0);
		}*/
		return;
	}

	m_objects.push_back(Object(x - 250.f, 250.f - y,
		0.0f, 20.0f, 0.f, 1.f, 1.f, 1.0f,
		rand() % 3 - 2, rand() % 3 - 2, 0.0f, MOVE_SPEED));
}