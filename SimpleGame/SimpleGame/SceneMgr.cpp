#include "stdafx.h"
#include "SceneMgr.h"

int number = 0;
float start_time = 0.0f;
GLuint texture_id = 0;

SceneMgr::SceneMgr(float x, float y)
{
	// Initialize Renderer
	srand((unsigned)time(NULL));
	renderer = new Renderer((int)x, (int)y);
	if (!renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}


	// ����Ʈ ���� �������ֱ�.
	m_objects.push_back(Object(0.f, 0.f, 0.f,
		50.0f, 1.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, 500, LIFETIME_ULTIMATE));
	srand((unsigned)time(NULL));

	// Image ID ������ֱ�
	char file_path[] = "./Resource/coc.png";
	texture_id = renderer->CreatePngTexture(file_path);
}


SceneMgr::~SceneMgr()
{
	m_objects.clear();
	delete renderer;
}

void SceneMgr::Update(float elapsed_time) {
	float elapsed_time_in_sec = elapsed_time / 1000.f;


	// �ð� ���� ����
	if (is_clocking) {
		start_time += elapsed_time_in_sec;
		if (start_time >= 0.5f) {	// 0.5�ʰ� �Ѿ�� clocking �����ϰ�, Bullet push back.
			is_clocking = false;
			start_time = 0.f;
		}
	}


	// �̷��� ����� �� �������� ���α׷��� ������ �ʴ´ܴ�.
	if (is_clocking == false && m_objects.size() > 0) {
		is_shooting = true;
		// �Ѿ��� ���⼭ push_back ���ش�.

		// �߸� ������ ������ ��������!
		if (m_objects[0].GetLife() > 0 && m_objects[0].type == OBJECT_BUILDING) {
			m_objects.push_back(Object(m_objects[0].Get_x(), m_objects[0].Get_y(), m_objects[0].Get_z(),
				2.f, 1.f, 0.f, 0.f, 1.f,
				// ������ ������� ����
				SPEED_BULLET * ((float)std::rand() / (float)RAND_MAX) - 0.5f, SPEED_BULLET * ((float)std::rand() / (float)RAND_MAX) - 0.5f, 0.0f,
				0.01f, OBJECT_BULLET, 20, LIFETIME_BULLET
			));
		}

		// ĳ������ Arrow �߻�
		for (int i = 0; i < m_objects.size(); ++i) {
			// ĳ���� ���������̸�.
			if (m_objects[i].type == OBJECT_CHARACTER) {
				m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					2.f, 0.f, 1.f, 0.f, 1.f,
					SPEED_ARROW * ((float)std::rand() / (float)RAND_MAX) - 0.5f, SPEED_ARROW * ((float)std::rand() / (float)RAND_MAX) - 0.5f, 0.0f,
					0.01f, OBJECT_ARROW, 10, LIFETIME_ARROW
				));
				// ĳ���Ϳ��� �߻��� Arrow�� �� ĳ������ ���� Arrow_num�� �־���Ѵ�.
				// Arrow�� push_back �Ǿ����Ƿ� ���� �߰��� object�� ��������° �ִ�.

				m_objects[m_objects.size() - 1].SetArrowNumber(i);
				//std::cout << m_objects.size() - 1 << "��° Arrow�̰� �� Arrow�� " << i << "���� Character" << std::endl;
			}
		}

		number++;
		//std::cout << "�߻� !" << " ���� : " << number << std::endl;
		is_clocking = true;
	}






	// �ð��� ���� ���� ����
	for (int i = 0; i < m_objects.size(); ++i) {
		// ü�� �����̸� ���� X
		if (m_objects[i].GetLifeTime() == LIFETIME_ULTIMATE)
			continue;

		m_objects[i].SetLifeTime(m_objects[i].GetLifeTime() - elapsed_time_in_sec);
		// object�� LifeTime���� start_life_time�� �� ũ�ԵǸ�
		if (m_objects[i].GetLifeTime() < 0.f) {
			// �ش� object ����.
			//std::cout << i << "���� ������Ʈ life time :" << m_objects[i].GetLifeTime() << std::endl;
			m_objects.erase(m_objects.begin() + i);
		}
	}


	CollideCheck();


	// ���� �� ������ Object �������ִ� �κ�.
	for (int i = 0; i < m_objects.size(); ++i) {
		// ������ ��������
		if (m_objects[i].GetLife() > 0) {
			m_objects[i].Update(elapsed_time_in_sec);
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
						continue;
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
						m_objects[i].is_collide = true;
						m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

						// �Ѿ��� ������ �׳� erase
						m_objects[j].is_collide = true;
						m_objects.erase(m_objects.begin() + j);
					}

					else if (m_objects[i].type == OBJECT_BULLET && m_objects[j].type == OBJECT_CHARACTER) {
						// ĳ���� �浹��.
						m_objects[j].is_collide = true;
						m_objects[j].SetLife(m_objects[j].GetLife() - m_objects[i].GetLife());

						// �Ѿ��� ������ �׳� erase
						m_objects[i].is_collide = true;
						m_objects.erase(m_objects.begin() + i);
					}


					// ���� �� �̻��ѵ� �ٽ� Ȯ�� �غ���.
					// --------------------------------------------------------------------------------------------------
					// ĳ���Ϳ� Arrow�� �浹�Ҷ��� �ϴ� Arrow_number ���ϰ� �浹
					else if (m_objects[i].type == OBJECT_CHARACTER && m_objects[j].type == OBJECT_ARROW) {
						// Arrow_num�̶� ������Ʈ ��ȣ�� ������ �浹�ϸ� ����
						if (i == m_objects[j].GetArrowNumber())
							continue;
						else if (i != m_objects[j].GetArrowNumber()) {
							// Arrow�� �ٸ� Character�浹 .
							m_objects[i].is_collide = true;
							m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

							// �Ѿ��� ������ �׳� erase
							m_objects[j].is_collide = true;
							m_objects.erase(m_objects.begin() + j);
						}
					}
					else if (m_objects[i].type == OBJECT_ARROW && m_objects[j].type == OBJECT_CHARACTER) {
						// Arrow_num�̶� ������Ʈ ��ȣ�� ������ �浹�ϸ� ����
						if (j == m_objects[i].GetArrowNumber())
							continue;
						else if (m_objects[i].GetArrowNumber() != j) {
							// Arrow�� �ٸ� Character�浹 .
							m_objects[j].is_collide = true;
							m_objects[j].SetLife(m_objects[j].GetLife() - m_objects[i].GetLife());

							// �Ѿ��� ������ �׳� erase
							m_objects[i].is_collide = true;
							m_objects.erase(m_objects.begin() + i);
						}
					}
					// --------------------------------------------------------------------------------------------------




					// Arrow�� ����
					else if (m_objects[i].type == OBJECT_BUILDING && m_objects[j].type == OBJECT_ARROW) {
						// ĳ���� �浹��.
						m_objects[i].is_collide = true;
						m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

						// Arrow�� ������ �׳� erase
						m_objects[j].is_collide = true;
						m_objects.erase(m_objects.begin() + j);
						std::cout << " : ����ü�� ::" << m_objects[i].GetLife() << std::endl;

					}
					else if (m_objects[i].type == OBJECT_ARROW && m_objects[j].type == OBJECT_BUILDING) {
						// ĳ���� �浹��.
						m_objects[j].is_collide = true;
						m_objects[j].SetLife(m_objects[j].GetLife() - m_objects[i].GetLife());

						// Arrow�� ������ �׳� erase
						m_objects[i].is_collide = true;
						m_objects.erase(m_objects.begin() + i);
						std::cout << " : ����ü�� ::" << m_objects[j].GetLife() << std::endl;

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
			// �������� �̹��� ����
			if (m_objects[i].type == OBJECT_BUILDING) {
				renderer->DrawTexturedRect(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), m_objects[i].Get_R(), m_objects[i].Get_G(), m_objects[i].Get_B(), m_objects[i].Get_A(), texture_id);
			}
			else {
				renderer->DrawSolidRect(m_objects[i].Get_x(),
					m_objects[i].Get_y(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), m_objects[i].Get_R(),
					m_objects[i].Get_G(), m_objects[i].Get_B(),
					m_objects[i].Get_A());
			}
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
		m_objects.push_back(Object(x - 250.f, 250.f - y,
			0.0f, 10.0f, 1.f, 1.f, 1.f, 1.0f,
			// ������ ������� ����
			SPEED_CHARACTER*((float)std::rand() / (float)RAND_MAX) - 0.5f, SPEED_CHARACTER*((float)std::rand() / (float)RAND_MAX) - 0.5f, 0.0f,
			MOVE_SPEED, OBJECT_CHARACTER, 10, LIFETIME_CHARACTER));
	}
}