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


	// 디폴트 빌딩 생성해주기.
	m_objects.push_back(Object(0.f, 0.f, 0.f,
		50.0f, 1.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, 500));
	srand((unsigned)time(NULL));


 
}


SceneMgr::~SceneMgr()
{
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

	// vector에 object가 있을때만 충돌체크가 가능하게 프로그램함.
	if (m_objects.size() > 0) {			
		for (int i = 0; i < m_objects.size() - 1; ++i) {
			for (int j = i + 1; j < m_objects.size(); ++j) {
				if ((m_objects[i].GetLife() > 0) && (m_objects[j].GetLife() > 0)) {
					left_a = m_objects[i].Get_x() - m_objects[i].Get_size() / 2;
					right_a = m_objects[i].Get_x() + m_objects[i].Get_size() / 2;
					bottom_a = m_objects[i].Get_y() - m_objects[i].Get_size() / 2;
					top_a = m_objects[i].Get_y() + m_objects[i].Get_size() / 2;

					left_b = m_objects[j].Get_x() - m_objects[j].Get_size() / 2;
					right_b = m_objects[j].Get_x() + m_objects[j].Get_size() / 2;
					bottom_b = m_objects[j].Get_y() - m_objects[j].Get_size() / 2;
					top_b = m_objects[j].Get_y() + m_objects[j].Get_size() / 2;

					if (left_a > right_b)continue;
					if (right_a < left_b)continue;
					if (top_a < bottom_b)continue;
					if (bottom_a > top_b)continue;

					if (m_objects[i].type == OBJECT_CHARACTER && m_objects[j].type == OBJECT_CHARACTER) {
						// 캐릭터와 캐릭터일 경우
						// 충돌해도 데미지 없음.
					}
					else if (m_objects[i].type == OBJECT_CHARACTER && m_objects[j].type == OBJECT_BUILDING) {
						// 캐릭터와 빌딩 충돌하게되면


						// 빌딩의 라이프 - 캐릭터 라이프
						m_objects[j].is_collide = true;
						m_objects[j].LostLife(m_objects[i].GetLife());
						std::cout << " : 보스체력임 ::" << m_objects[j].GetLife() << std::endl;
						// 캐릭터 소멸
						m_objects[i].is_collide = true;
						m_objects[i].SetLife(0);
					}
					else if (m_objects[i].type == OBJECT_BUILDING && m_objects[j].type == OBJECT_CHARACTER) {
						// 빌딩, 캐릭터 충돌하게되면
						
						// 빌딩의 라이프 - 캐릭터 라이프
						m_objects[i].is_collide = true;
						m_objects[i].LostLife(m_objects[j].GetLife());
						std::cout << " : 보스체력임 ::" << m_objects[i].GetLife() << std::endl;

						// 캐릭터 소멸
						m_objects[j].is_collide = true;
						m_objects[j].SetLife(0);
						// vector erase 해주고싶은데...
						m_objects.erase(m_objects.begin() + j);
						// 오 된다!!!!!!!!!!!!!!!!!!!
					}
				}
			}
		}

	}
}


void SceneMgr::SceneRender() {
	for (int i = 0; i < m_objects.size(); ++i) {
		// 생명이 있을경우에만 그린다. 
		if (m_objects[i].GetLife() > 0) {
			renderer->DrawSolidRect(m_objects[i].Get_x(),
				m_objects[i].Get_y(), m_objects[i].Get_z(),
				m_objects[i].Get_size(), m_objects[i].Get_R(),
				m_objects[i].Get_G(), m_objects[i].Get_B(),
				m_objects[i].Get_A());
		}
	}
}


void SceneMgr::MouseInput(int x, int y, int object_type) {
	if (m_objects.size() >= MAX_OBJECTS_COUNT) {
		std::cout << "Too much object" << std::endl;
	}
	// 빌딩
	else {
		if (object_type == OBJECT_CHARACTER) {
			m_objects.push_back(Object(x - 250.f, 250.f - y,
				0.0f, 10.0f, 0.f, 0.f, 0.f, 0.0f,
				rand() % 3 - 2, rand() % 3 - 2, 0.0f,
				MOVE_SPEED, OBJECT_CHARACTER, 10));
		}
	}
}