#include "stdafx.h"
#include "SceneMgr.h"

int number = 0;
float start_time = 0.0f;
GLuint texture_id = 0;

SceneMgr::SceneMgr(float x, float y )
{	
	// Initialize Renderer
	srand((unsigned)time(NULL));
	renderer = new Renderer(x, y);
	if (!renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}


	// 디폴트 빌딩 생성해주기.
	m_objects.push_back(Object(0.f, 0.f, 0.f,
		50.0f, 1.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, 500, LIFETIME_ULTIMATE));
	srand((unsigned)time(NULL));

	// Image ID 만들어주기
	char file_path[] = "./Resource/agumon.png";
	texture_id = renderer->CreatePngTexture(file_path);
}


SceneMgr::~SceneMgr()
{
	m_objects.clear();
	delete renderer;
}

void SceneMgr::Update(float elapsed_time) {
	float elapsed_time_in_sec = elapsed_time / 1000.f;


	// 시간 측정 시작
	if (is_clocking) {
		start_time += elapsed_time_in_sec;
		if (start_time >= 0.5f) {	// 0.5초가 넘어가면 clocking 중지하고, Bullet push back.
			is_clocking = false;
			start_time = 0.f;
		}
	}


	if (is_clocking == false) {
		is_shooting = true;
		// 총알을 여기서 push_back 해준다.
		m_objects.push_back(Object(m_objects[0].Get_x(), m_objects[0].Get_y(), m_objects[0].Get_z(),
			2.f, 1.f, 0.f, 0.f, 1.f,
			// 교수님 랜덤방식 참조
			SPEED_BULLET * ((float)std::rand() / (float)RAND_MAX) - 0.5f, SPEED_BULLET * ((float)std::rand() / (float)RAND_MAX) - 0.5f, 0.0f,
			0.01f, OBJECT_BULLET, 20, LIFETIME_BULLET
		));

		// 캐릭터의 Arrow 발사
		for (int i = 0; i < m_objects.size(); ++i) {
			// 캐릭터 오브젝터이면.
			if (m_objects[i].type == OBJECT_CHARACTER) {
				m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					2.f, 0.f, 1.f, 0.f, 1.f,
					SPEED_ARROW * ((float)std::rand() / (float)RAND_MAX) - 0.5f, SPEED_ARROW * ((float)std::rand() / (float)RAND_MAX) - 0.5f, 0.0f,
					0.01f, OBJECT_ARROW, 10,LIFETIME_ARROW
				));
			}
		}

		number++;
		//std::cout << "발사 !" << " 갯수 : " << number << std::endl;
		is_clocking = true;
	}






	// 시간에 따라 생명 소진
	for (int i = 0; i < m_objects.size(); ++i) {
		// 체력 무한이면 삭제 X
		if (m_objects[i].GetLifeTime() == LIFETIME_ULTIMATE)
			continue;

		m_objects[i].SetLifeTime(m_objects[i].GetLifeTime() - elapsed_time_in_sec);
		// object의 LifeTime보다 start_life_time이 더 크게되면
		if (m_objects[i].GetLifeTime() < 0.f) {
			// 해당 object 삭제.
			//std::cout << i << "번쨰 오브젝트 life time :" << m_objects[i].GetLifeTime() << std::endl;
			m_objects.erase(m_objects.begin() + i);
		}
	}


	CollideCheck();


	// 생명 다 소진한 Object 삭제해주는 부분.
	for (int i = 0; i < m_objects.size(); ++i) {
		// 생명이 있을때만
		if (m_objects[i].GetLife() > 0) {
			m_objects[i].Update(elapsed_time_in_sec);
		}
		else {	// 생명이 없으면 삭제해준다.
			std::cout << i << "번째 오브젝트 삭제, 오브젝트type은 " << m_objects[i].type << "이다." << std::endl;
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
						std::cout << " : 보스체력 ::" << m_objects[j].GetLife() << std::endl;
						// 캐릭터 소멸
						m_objects[i].is_collide = true;
						m_objects[i].SetLife(0);
						m_objects.erase(m_objects.begin() + i);
					}
					else if (m_objects[i].type == OBJECT_BUILDING && m_objects[j].type == OBJECT_CHARACTER) {
						// 빌딩, 캐릭터 충돌하게되면
						
						// 빌딩의 라이프 - 캐릭터 라이프
						m_objects[i].is_collide = true;
						m_objects[i].LostLife(m_objects[j].GetLife());
						std::cout << " : 보스체력 ::" << m_objects[i].GetLife() << std::endl;

						// 캐릭터 소멸
						m_objects[j].is_collide = true;
						m_objects[j].SetLife(0);
						// vector erase 해주고싶은데...
						m_objects.erase(m_objects.begin() + j);
						// 오 된다!!!!!!!!!!!!!!!!!!!
					}
					
					// 총알의 충돌체크 해줘야함.
					else if (m_objects[i].type == OBJECT_CHARACTER && m_objects[j].type == OBJECT_BULLET) {
						// 캐릭터 충돌함.
						m_objects[i].is_collide = true;
						m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

						// 총알은 닿으면 그냥 erase
						m_objects[j].is_collide = true;
						m_objects.erase(m_objects.begin() + j);
					}

					else if (m_objects[i].type == OBJECT_BULLET && m_objects[j].type == OBJECT_CHARACTER) {
						// 캐릭터 충돌함.
						m_objects[j].is_collide = true;
						m_objects[j].SetLife(m_objects[j].GetLife() - m_objects[i].GetLife());

						// 총알은 닿으면 그냥 erase
						m_objects[i].is_collide = true;
						m_objects.erase(m_objects.begin() + i);
					}

					// 캐릭터와 Arrow가 충돌할때는 아무고통 못하게 해야한당 ㅎ
					else if (m_objects[i].type == OBJECT_CHARACTER && m_objects[j].type == OBJECT_ARROW) {
						continue;
					}
					else if (m_objects[i].type == OBJECT_ARROW && m_objects[j].type == OBJECT_CHARACTER) {
						continue;
					}

					// Arrow와 빌딩
					else if (m_objects[i].type == OBJECT_BUILDING && m_objects[j].type == OBJECT_ARROW) {
						// 캐릭터 충돌함.
						m_objects[i].is_collide = true;
						m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

						// Arrow는 닿으면 그냥 erase
						m_objects[j].is_collide = true;
						m_objects.erase(m_objects.begin() + j);
					}
					else if (m_objects[i].type == OBJECT_ARROW && m_objects[j].type == OBJECT_BUILDING) {
						// 캐릭터 충돌함.
						m_objects[j].is_collide = true;
						m_objects[j].SetLife(m_objects[j].GetLife() - m_objects[i].GetLife());

						// Arrow은 닿으면 그냥 erase
						m_objects[i].is_collide = true;
						m_objects.erase(m_objects.begin() + i);
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
			// 빌딩에만 이미지 넣쟈
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
	// 캐릭터 오브젝트의 수를 업데이트한다.
	if (m_objects.size() > 0) {
		for (int i = 0; i < m_objects.size(); ++i) {
			if (m_objects[i].type == OBJECT_CHARACTER)
				num_of_character++;
		}
	}


	if (num_of_character > MAX_OBJECTS_COUNT) {
		std::cout << "Too much object" << std::endl;
	}
	// 클릭했을 때 소환되는 오브젝트는 나중에 따로 입력받을 수 있게 한다. 
	else {
		if (object_type == OBJECT_CHARACTER) {
			m_objects.push_back(Object(x - 250.f, 250.f - y,
				0.0f, 10.0f, 1.f, 1.f, 1.f, 1.0f,
				// 교수님 랜덤방식 참조
				SPEED_CHARACTER*((float)std::rand() / (float)RAND_MAX) - 0.5f, SPEED_CHARACTER*((float)std::rand() / (float)RAND_MAX) - 0.5f, 0.0f,
				MOVE_SPEED, OBJECT_CHARACTER, 10, LIFETIME_CHARACTER));
		}
	}
}