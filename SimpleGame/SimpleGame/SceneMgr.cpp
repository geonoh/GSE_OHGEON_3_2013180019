#include "stdafx.h"
#include "SceneMgr.h"

float start_time;

SceneMgr::SceneMgr(float x, float y )
{	
	// Initialize Renderer

	renderer = new Renderer(x, y);
	if (!renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}


	// ����Ʈ ���� �������ֱ�.
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
	// �ð� ���� ����.
	if (is_clocking) {
		start_time = timeGetTime() / 1000;
		is_clocking = false;
	}


	// ���۽ð� üũ�ϰ� 500ms �� 0.5�ʰ� �����.
	if (is_clocking == false) {
		if (start_time + 2.f <= timeGetTime() / 1000) {
			is_shooting = true;
			// �Ѿ��� ���⼭ push_back ���ش�.
			m_objects.push_back(Object(m_objects[0].Get_x(), m_objects[0].Get_y(), m_objects[0].Get_z(),
				20.f, 1.f, 0.f, 0.f, 1.f,
				rand() % 3 - 2, rand() % 3 - 2, 0.0f,
				0.01f, OBJECT_BULLET,20
				));

			std::cout << "�߻�" << std::endl;
			is_clocking = true;
		}
	}

	CollideCheck();

	if (is_shooting == false) {

	}

	for (int i = 0; i < m_objects.size(); ++i) {
		// ������ ��������
		if (m_objects[i].GetLife() > 0) {
			m_objects[i].Update(0);
		}
		else {	// ������ ������ �������ش�.
			std::cout << i << "��° ������Ʈ ����, ������Ʈtype�� " << m_objects[i].type << "�̴�." << std::endl;
			m_objects.erase(m_objects.begin() + i);

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

	// vector�� object�� �������� �浹üũ�� �����ϰ� ���α׷���.
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
						// ĳ���Ϳ� ĳ������ ���
						// �浹�ص� ������ ����.
					}
					else if (m_objects[i].type == OBJECT_CHARACTER && m_objects[j].type == OBJECT_BUILDING) {
						// ĳ���Ϳ� ���� �浹�ϰԵǸ�


						// ������ ������ - ĳ���� ������
						m_objects[j].is_collide = true;
						m_objects[j].LostLife(m_objects[i].GetLife());
						std::cout << " : ����ü�� ::" << m_objects[j].GetLife() << std::endl;
						// ĳ���� �Ҹ�
						m_objects[i].is_collide = true;
						m_objects[i].SetLife(0);
						m_objects.erase(m_objects.begin() + i);
					}
					else if (m_objects[i].type == OBJECT_BUILDING && m_objects[j].type == OBJECT_CHARACTER) {
						// ����, ĳ���� �浹�ϰԵǸ�
						
						// ������ ������ - ĳ���� ������
						m_objects[i].is_collide = true;
						m_objects[i].LostLife(m_objects[j].GetLife());
						std::cout << " : ����ü�� ::" << m_objects[i].GetLife() << std::endl;

						// ĳ���� �Ҹ�
						m_objects[j].is_collide = true;
						m_objects[j].SetLife(0);
						// vector erase ���ְ������...
						m_objects.erase(m_objects.begin() + j);
						// �� �ȴ�!!!!!!!!!!!!!!!!!!!
					}
					
					// �Ѿ��� �浹üũ �������.
					else if (m_objects[i].type == OBJECT_CHARACTER && m_objects[j].type == OBJECT_BULLET) {
						// ĳ���� �浹��.
						m_objects[i].is_collide == true;
						m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

						// �Ѿ��� ������ �׳� erase
						m_objects[j].is_collide == true;
						m_objects.erase(m_objects.begin() + j);
					}

					else if (m_objects[i].type == OBJECT_BULLET && m_objects[j].type == OBJECT_CHARACTER) {
						// ĳ���� �浹��.
						m_objects[j].is_collide == true;
						m_objects[j].SetLife(m_objects[j].GetLife() - m_objects[i].GetLife());

						// �Ѿ��� ������ �׳� erase
						m_objects[i].is_collide == true;
						m_objects.erase(m_objects.begin() + i);
					}
				}
			}
		}

	}
}


void SceneMgr::SceneRender() {
	for (int i = 0; i < m_objects.size(); ++i) {
		// ������ ������쿡�� �׸���. 
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
	int num_of_character = 0;
	// ĳ���� ������Ʈ�� ���� ������Ʈ�Ѵ�.
	if (m_objects.size() > 0) {
		for (int i = 0; i < m_objects.size(); ++i) {
			if (m_objects[i].type == OBJECT_CHARACTER)
				num_of_character++;
		}
	}


	if (num_of_character > MAX_OBJECTS_COUNT) {
		std::cout << "Too much object" << std::endl;
	}
	// Ŭ������ �� ��ȯ�Ǵ� ������Ʈ�� ���߿� ���� �Է¹��� �� �ְ� �Ѵ�. 
	else {
		if (object_type == OBJECT_CHARACTER) {
			m_objects.push_back(Object(x - 250.f, 250.f - y,
				0.0f, 10.0f, 0.f, 0.f, 0.f, 0.0f,
				rand() % 3 - 2, rand() % 3 - 2, 0.0f,
				MOVE_SPEED, OBJECT_CHARACTER, 10));
		}
	}
}