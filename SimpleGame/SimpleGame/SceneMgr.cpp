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



	srand((unsigned)time(NULL));

	// 오브젝트 초기화 사이즈 20으로 고정
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) {
		m_objects[i] = new Object(
			-250 + rand() % 500, -250 + rand() % 500,
			-250 + rand() % 500, 20, 1.f, 1.f, 1.f, 0.0f
		);

		m_objects[i]->Set_velocity(-1 + rand() % 2, -1 + rand() % 2
			, -1 + rand() % 2, 0.1f);
	}
	// 유저 오브젝트 초기화
	//user_obejct = new Object(0.0f, 0.0f, 0.0f, 50.f / 2, 0.0f, 1.0f, 1.0f, 1.0f);
	//user_obejct->Set_velocity(1.f, 1.f, 0.f, 1.f);
}


SceneMgr::~SceneMgr()
{

	printf("소멸스\n");
	delete[] m_objects;
	delete renderer;
}

void SceneMgr::Update() {
	long start_time;
	start_time = timeGetTime();
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) {
		// 생명이 있을때만
		if (m_objects[i]->GetLife() > 0) {
			m_objects[i]->Update(0);
		}
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


		if (((pre_min_x <= m_objects[i + 1]->Get_x()) && (m_objects[i + 1]->Get_x() <= pre_max_x)) &&	// x가 범위 안에 있고,
			((pre_min_y <= m_objects[i + 1]->Get_y()) && (m_objects[i + 1]->Get_y() <= pre_max_y)))		// y가 범위 안에 있으면
		{
			m_objects[i]->SetRed();
			m_objects[i + 1]->SetRed();
			//printf("%d번과 %d번 충돌!\n", i, i + 1);
			//Sleep(1000);
		}
	}
}
void SceneMgr::SceneRender(float x, float y, float z, float size, float r, float g, float b, float a) {
	renderer->DrawSolidRect(x, y, z, size, r, g, b, a);
}
void SceneMgr::SceneRender() {
	// 배경들 움직여버리기
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) {\
		if (m_objects[i]->GetLife() > 0) {
			renderer->DrawSolidRect(m_objects[i]->Get_x(),
				m_objects[i]->Get_y(), m_objects[i]->Get_z(),
				m_objects[i]->Get_size(), m_objects[i]->Get_R(),
				m_objects[i]->Get_G(), m_objects[i]->Get_B(),
				m_objects[i]->Get_A());
		}
	}
	
	// 유저 그리기
	//renderer->DrawSolidRect(user_obejct->Get_x(),
	//	user_obejct->Get_y(), user_obejct->Get_z(),
	//	user_obejct->Get_size(), user_obejct->Get_R(),
	//	user_obejct->Get_G(), user_obejct->Get_B(),
	//	user_obejct->Get_A());
}


void SceneMgr::MouseInput(int x, int y, int num) {
	// 이때 새로 오브젝트를 추가 해준다.
	m_objects[num]->Setter(x - 250.f, 250.f - y, 
		0.0f, 20.0f, 1.f, 1.f, 1.f, 0.0f);
	m_objects[num]->Set_velocity(rand() % 2 - 1, rand() % 2 - 1, 0, 0.1f);


	// 클릭하면 생명 10 받는다.
	m_objects[num]->SetLife(10);
}