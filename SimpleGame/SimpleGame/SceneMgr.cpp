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

	// ������Ʈ �ʱ�ȭ ������ 20���� ����
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) {
		m_objects[i] = new Object(
			-250 + rand() % 500, -250 + rand() % 500,
			-250 + rand() % 500, 20, 1.f, 1.f, 1.f, 0.0f
		);

		m_objects[i]->Set_velocity(-1 + rand() % 2, -1 + rand() % 2
			, -1 + rand() % 2, MOVE_SPEED);
	}
	// ���� ������Ʈ �ʱ�ȭ
	//user_obejct = new Object(0.0f, 0.0f, 0.0f, 50.f / 2, 0.0f, 1.0f, 1.0f, 1.0f);
	//user_obejct->Set_velocity(1.f, 1.f, 0.f, 1.f);
}


SceneMgr::~SceneMgr()
{

	printf("�Ҹ꽺\n");
	delete[] m_objects;
	delete renderer;
}

void SceneMgr::Update() {
	DWORD start_time;
	start_time = timeGetTime();

	CollideCheck();


	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) {
		// ������ ��������
		if (m_objects[i]->GetLife() > 0) {
			m_objects[i]->Update(0);
		}
		else {
			//printf("%d�� ������Ʈ ü�� : %f\n", i, m_objects[i]->GetLife());
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

	for (int i = 0; i < MAX_OBJECTS_COUNT - 1; ++i) {
		left_a = m_objects[i]->Get_x() - m_objects[i]->Get_size() / 2;
		right_a = m_objects[i]->Get_x() + m_objects[i]->Get_size() / 2;
		bottom_a = m_objects[i]->Get_y() - m_objects[i]->Get_size() / 2;
		top_a = m_objects[i]->Get_y() + m_objects[i]->Get_size() / 2;

		left_b = m_objects[i + 1]->Get_x() - m_objects[i + 1]->Get_size() / 2;
		right_b = m_objects[i + 1]->Get_x() + m_objects[i + 1]->Get_size() / 2;
		bottom_b = m_objects[i + 1]->Get_y() - m_objects[i + 1]->Get_size() / 2;
		top_b = m_objects[i + 1]->Get_y() + m_objects[i + 1]->Get_size() / 2;

		if (left_a > right_b)continue;
		if (right_a < left_b)continue;
		if (top_a < bottom_b)continue;
		if (bottom_a > top_b)continue;

		m_objects[i]->SetRed();
		m_objects[i + 1]->SetRed();
		printf("%d���� %d�� �浹!\n", i, i + 1);

	}

	// �浹�� ������ü�� �ٽ� �������
	for (int i = 0; i < MAX_OBJECTS_COUNT - 1; ++i) {
		left_a = m_objects[i]->Get_x() - m_objects[i]->Get_size() / 2;
		right_a = m_objects[i]->Get_x() + m_objects[i]->Get_size() / 2;
		bottom_a = m_objects[i]->Get_y() - m_objects[i]->Get_size() / 2;
		top_a = m_objects[i]->Get_y() + m_objects[i]->Get_size() / 2;

		left_b = m_objects[i + 1]->Get_x() - m_objects[i + 1]->Get_size() / 2;
		right_b = m_objects[i + 1]->Get_x() + m_objects[i + 1]->Get_size() / 2;
		bottom_b = m_objects[i + 1]->Get_y() - m_objects[i + 1]->Get_size() / 2;
		top_b = m_objects[i + 1]->Get_y() + m_objects[i + 1]->Get_size() / 2;

		if (m_objects[i]->is_collide) {
			if (left_a > right_b) {
				m_objects[i]->SetWhite();
				m_objects[i + 1]->SetWhite();
			}
			if (right_a < left_b) {
				m_objects[i]->SetWhite();
				m_objects[i + 1]->SetWhite();
			}
			if (top_a < bottom_b) {
				m_objects[i]->SetWhite();
				m_objects[i + 1]->SetWhite();
			}
			if (bottom_a > top_b) {
				m_objects[i]->SetWhite();
				m_objects[i + 1]->SetWhite();
			}
		}
	}

}
void SceneMgr::SceneRender(float x, float y, float z, float size, float r, float g, float b, float a) {
	renderer->DrawSolidRect(x, y, z, size, r, g, b, a);
}
void SceneMgr::SceneRender() {
	// ���� ������������
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) {\
		if (m_objects[i]->GetLife() > 0) {
			renderer->DrawSolidRect(m_objects[i]->Get_x(),
				m_objects[i]->Get_y(), m_objects[i]->Get_z(),
				m_objects[i]->Get_size(), m_objects[i]->Get_R(),
				m_objects[i]->Get_G(), m_objects[i]->Get_B(),
				m_objects[i]->Get_A());
		}
	}
}


void SceneMgr::MouseInput(int x, int y, int num) {
	// �̶� ���� ������Ʈ�� �߰� ���ش�.
	m_objects[num]->Setter(x - 250.f, 250.f - y, 
		0.0f, 20.0f, 1.f, 1.f, 1.f, 0.0f);
	m_objects[num]->Set_velocity(rand() % 2 - 1, rand() % 2 - 1, 0, MOVE_SPEED);


	// Ŭ���ϸ� ���� 10 �޴´�.
	m_objects[num]->SetLife(10);
}