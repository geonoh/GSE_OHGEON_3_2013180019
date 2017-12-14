#include "stdafx.h"
#include "SceneMgr.h"

float start_time = 0.0f;
float start_time_building = 0.0f;
float start_time_team1 = 0.f;
float start_time_character_spawn = 0.f;
GLuint texture_id_1 = 0;
GLuint texture_id_2 = 0;
GLuint texture_id_3 = 0;
GLuint texture_id_4 = 0;

GLuint texture_id_background = 0;
GLuint texture_id_sprite = 0;
GLuint texture_id_particle = 0;


int sprite_x = 0;
int sprite1_x = 0;
float particle_time = 0.f;
DWORD previous_time_particle = 0;
float elapsed_time_in_sec_particle = 0;


SceneMgr::SceneMgr(float x, float y)
{
	// Initialize Renderer
	srand((unsigned)time(NULL));
	renderer = new Renderer((int)x, (int)y);
	if (!renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}


	// TEAM_1 빌딩 생성해주기.
	m_objects.push_back(Object(-200.f, 300.f, 0.f,
		SIZE_BUILDING, 1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, LIFE_BUILDING, LIFETIME_ULTIMATE, TEAM_1));

	// TEAM_1 빌딩 생성해주기.
	m_objects.push_back(Object(0.f, 300.f, 0.f,
		SIZE_BUILDING, 1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, LIFE_BUILDING, LIFETIME_ULTIMATE, TEAM_1));

	// TEAM_1 빌딩 생성해주기.
	m_objects.push_back(Object(200.f, 300.f, 0.f,
		SIZE_BUILDING, 1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, LIFE_BUILDING, LIFETIME_ULTIMATE, TEAM_1));


	// TEAM_2 빌딩 생성해주기.
	m_objects.push_back(Object(-200.f, -300.f, 0.f,
		SIZE_BUILDING, 1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, LIFE_BUILDING, LIFETIME_ULTIMATE, TEAM_2));

	// TEAM_2 빌딩 생성해주기.
	m_objects.push_back(Object(0.f, -300.f, 0.f,
		SIZE_BUILDING, 1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, LIFE_BUILDING, LIFETIME_ULTIMATE, TEAM_2));

	// TEAM_2 빌딩 생성해주기.
	m_objects.push_back(Object(200.f, -300.f, 0.f,
		SIZE_BUILDING, 1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, LIFE_BUILDING, LIFETIME_ULTIMATE, TEAM_2));




	// Image ID 만들어주기
	char team_1_path[] = "./Resource/emote_laugh.png";
	char team_2_path[] = "./Resource/emote_cry.png";
	char team_3_path[] = "./Resource/Blue_King_Laughing.png";
	char team_4_path[] = "./Resource/Red_King_Angry.png";
	char back_path[] = "./Resource/background.png";
	char sprite_path[] = "./Resource/sprite2.png";
	char particle_path[] = "./Resource/particle.png";

	texture_id_1 = renderer->CreatePngTexture(team_1_path);
	texture_id_2 = renderer->CreatePngTexture(team_2_path);
	texture_id_3 = renderer->CreatePngTexture(team_3_path);
	texture_id_4 = renderer->CreatePngTexture(team_4_path);
	texture_id_background = renderer->CreatePngTexture(back_path);
	texture_id_sprite = renderer->CreatePngTexture(sprite_path);
	texture_id_particle = renderer->CreatePngTexture(particle_path);
	
	char music_file_path[] = "./Resource/bgm_.mp3";
	char bullet_file_path[] = "./Resource/bullet.mp3";
	m_sound = new Sound();
	bg_sound = m_sound->CreateSound(music_file_path);
	bullet_sound = m_sound->CreateSound(bullet_file_path);

	m_sound->PlaySoundW(bg_sound, true, 5.2f);
}


SceneMgr::~SceneMgr()
{
	m_objects.clear();
	m_sound->DeleteSound(bg_sound);

	delete renderer;
}

void SceneMgr::Update(float elapsed_time) {
	float elapsed_time_in_sec = elapsed_time / 1000.f;
	team_2_timer = elapsed_time_in_sec;

	if (shake)
		shake = false;
	else
		shake = true;

	// 시간 측정 시작
	if (is_clock_building) {
		start_time_building += elapsed_time_in_sec;
		if (start_time_building >= BULLET_GENTIME) {	// 10초가 넘어가면 clocking 중지하고, Bullet push back.
			is_clock_building = false;
			start_time_building = 0.f;
		}
	}

	if (team_2_character_clocking) {
		start_time_character_spawn += elapsed_time_in_sec;
		if (start_time_character_spawn >= PLAYER_COOLTIME) {	// 10초가 넘어가면 clocking 중지하고, Bullet push back.
			team_2_character_clocking = false;
			start_time_character_spawn = 0.f;
		}
	}

	if (is_clocking) {
		start_time += elapsed_time_in_sec;
		if (start_time >= ARROW_GENTIME) {	// 3.0초가 넘어가면 clocking 중지하고, Character Arrow push back.
			is_clocking = false;
			start_time = 0.f;
		}
	}

	if (team_1_clocking) {
		start_time_team1 += elapsed_time_in_sec;
		if (start_time_team1 >= ENEMY_GENTIME) {	// 5.f초가 넘어가면 clocking 중지하고, Team1 Character 생성 push back.
			team_1_clocking = false;
			start_time_team1 = 0.f;
		}
	}
	(float)(rand() % 250 - rand() % 250) / 250;
	// 각 팀별 빌딩의 bullet 발사 부분
	if (is_clock_building == false && m_objects.size() > 0) {
		// TEAM_1 빌딩이 생명이 있을때만!
		for (int i = 0; i < 3; ++i) {
			if (m_objects[i].GetLife() > 0 && m_objects[i].type == OBJECT_BUILDING) {
				m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					SIZE_BULLET, 1.f, 0.f, 0.f, 1.f,
					// 교수님 랜덤방식 참조
					(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
					SPEED_BULLET, OBJECT_BULLET, LIFE_BULLET, LIFETIME_BULLET, TEAM_1
				));
				m_sound->PlaySoundW(bullet_sound, false, 5.2f);
				cout << "쏜다 1빌딩" << endl;
			}
		}

		// TEAM_2 빌딩이 생명이 있을때만!
		for (int i = 3; i < 6; ++i) {
			if (m_objects[i].GetLife() > 0 && m_objects[i].type == OBJECT_BUILDING) {
				m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					SIZE_BULLET, 0.f, 0.f, 1.f, 1.f,
					// 교수님 랜덤방식 참조
					(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
					SPEED_BULLET, OBJECT_BULLET, LIFE_BULLET, LIFETIME_BULLET, TEAM_2
				));
				m_sound->PlaySoundW(bullet_sound, false, 5.2f);
				cout << "쏜다 2빌딩" << endl;
			}
		}
		is_clock_building = true;
	}


	// 이렇게 해줘야 다 없어져도 프로그램이 터지지 않는단다.
	if (is_clocking == false && m_objects.size() > 0) {
		// 캐릭터의 Arrow 발사
		for (int i = 0; i < m_objects.size(); ++i) {
			// 캐릭터 오브젝터이면.
			if (m_objects[i].type == OBJECT_CHARACTER && m_objects[i].GetTeam() == TEAM_1) {
				m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					SIZE_ARROW, 0.5f, 0.2f, 0.7f, 1.f,
					 (float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
					SPEED_ARROW, OBJECT_ARROW, LIFE_ARROW, LIFETIME_ARROW, TEAM_1
				));
				// 캐릭터에서 발사한 Arrow는 각 캐릭터의 고유 Arrow_num이 있어야한다.
				// Arrow가 push_back 되었으므로 새로 추가된 object는 마지막번째 있다.

				m_objects[m_objects.size() - 1].SetArrowNumber(i);
				//std::cout << m_objects.size() - 1 << "번째 Arrow이고 이 Arrow는 " << i << "번의 Character" << std::endl;
			}

			// 팀2의 캐릭터 Arrow. 
			if (m_objects[i].type == OBJECT_CHARACTER && m_objects[i].GetTeam() == TEAM_2) {
				m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					SIZE_ARROW, 1.f, 1.f, 0.f, 1.f,
					(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
					SPEED_ARROW, OBJECT_ARROW, LIFE_ARROW, LIFETIME_ARROW, TEAM_2
				));
				// 캐릭터에서 발사한 Arrow는 각 캐릭터의 고유 Arrow_num이 있어야한다.
				// Arrow가 push_back 되었으므로 새로 추가된 object는 마지막번째 있다.

				m_objects[m_objects.size() - 1].SetArrowNumber(i);
				//std::cout << m_objects.size() - 1 << "번째 Arrow이고 이 Arrow는 " << i << "번의 Character" << std::endl;
			}

		}

		//std::cout << "발사 !" << " 갯수 : " << number << std::endl;
		is_clocking = true;
	}


	// Team1 의 빌딩생성
	if (team_1_clocking == false && m_objects.size() > 0) {
		// TEAM_1 빌딩이 생명이 있을때만!
		int is_team1_alive = 0;
		for (int i = 0; i < 3; ++i) {
			// 빌딩이 하나라도 살아있으면 
			if (m_objects[i].GetLife() > 0) {
				is_team1_alive++;
			}

		}
		if (is_team1_alive > 0) {
			std::cout << "캐릭터생성" << std::endl;
			m_objects.push_back(Object(-250 + rand() % 500, rand() % 400, 0.f,
				SIZE_CHARACTER, 1.f, 1.f, 1.f, 1.f,
				// 교수님 랜덤방식 참조
				(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
				SPEED_CHARACTER, OBJECT_CHARACTER, LIFE_CHARACTER, LIFETIME_CHARACTER, TEAM_1
			));
			is_team1_alive = 0;
		}
		team_1_clocking = true;
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
				// 팀이 달라야 충돌체크 시작
				if (m_objects[i].GetTeam() != m_objects[j].GetTeam()) {
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

						// 캐릭터와 캐릭터일 경우
						// 충돌해도 데미지 없음.
						if (m_objects[i].type == OBJECT_CHARACTER && m_objects[j].type == OBJECT_CHARACTER) {
							//std::cout << "캐릭터끼리 충돌" << std::endl;
							//m_objects.erase(m_objects.begin() + i);
							//m_objects.erase(m_objects.begin() + j);

							continue;
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


						// 총알의 충돌체크 해줘야함.
						else if (m_objects[i].type == OBJECT_BUILDING && m_objects[j].type == OBJECT_BULLET) {
							// 캐릭터 충돌함.
							m_objects[i].is_collide = true;
							m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

							// 총알은 닿으면 그냥 erase
							m_objects[j].is_collide = true;
							m_objects.erase(m_objects.begin() + j);
						}

						else if (m_objects[i].type == OBJECT_BULLET && m_objects[j].type == OBJECT_BUILDING) {
							// 캐릭터 충돌함.
							m_objects[j].is_collide = true;
							m_objects[j].SetLife(m_objects[j].GetLife() - m_objects[i].GetLife());

							// 총알은 닿으면 그냥 erase
							m_objects[i].is_collide = true;
							m_objects.erase(m_objects.begin() + i);
						}


						// 여기 좀 이상한데 다시 확인 해보기.
						// --------------------------------------------------------------------------------------------------
						// 캐릭터와 Arrow가 충돌할때는 일단 Arrow_number 비교하고 충돌
						else if (m_objects[i].type == OBJECT_CHARACTER && m_objects[j].type == OBJECT_ARROW) {
							// Arrow_num이랑 오브젝트 번호가 같으면 충돌하면 ㄴㄴ
							if (i == m_objects[j].GetArrowNumber())
								continue;
							else if (i != m_objects[j].GetArrowNumber()) {
								// Arrow랑 다른 Character충돌 .
								m_objects[i].is_collide = true;
								m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

								// 총알은 닿으면 그냥 erase
								m_objects[j].is_collide = true;
								m_objects.erase(m_objects.begin() + j);
							}
						}
						else if (m_objects[i].type == OBJECT_ARROW && m_objects[j].type == OBJECT_CHARACTER) {
							// Arrow_num이랑 오브젝트 번호가 같으면 충돌하면 ㄴㄴ
							if (j == m_objects[i].GetArrowNumber())
								continue;
							else if (m_objects[i].GetArrowNumber() != j) {
								// Arrow랑 다른 Character충돌 .
								m_objects[j].is_collide = true;
								m_objects[j].SetLife(m_objects[j].GetLife() - m_objects[i].GetLife());

								// 총알은 닿으면 그냥 erase
								m_objects[i].is_collide = true;
								m_objects.erase(m_objects.begin() + i);
							}
						}
						// --------------------------------------------------------------------------------------------------




						// Arrow와 빌딩
						else if (m_objects[i].type == OBJECT_BUILDING && m_objects[j].type == OBJECT_ARROW) {
							// 캐릭터 충돌함.
							m_objects[i].is_collide = true;
							m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

							// Arrow는 닿으면 그냥 erase
							m_objects[j].is_collide = true;
							m_objects.erase(m_objects.begin() + j);
							std::cout << " : 보스체력 ::" << m_objects[i].GetLife() << std::endl;

						}
						else if (m_objects[i].type == OBJECT_ARROW && m_objects[j].type == OBJECT_BUILDING) {
							// 캐릭터 충돌함.
							m_objects[j].is_collide = true;
							m_objects[j].SetLife(m_objects[j].GetLife() - m_objects[i].GetLife());

							// Arrow은 닿으면 그냥 erase
							m_objects[i].is_collide = true;
							m_objects.erase(m_objects.begin() + i);
							std::cout << " : 보스체력 ::" << m_objects[j].GetLife() << std::endl;

						}
					}
				}
			}
		}

	}
}


void SceneMgr::SceneRender() {
	// 배경그려주기
	renderer->DrawTexturedRect(0.f, 0.f, 0.f,
		1000.f, 1.f, 1.f, 1.f, 1.f,
		texture_id_background, DRAWRANK_BACKGROUND);
	renderer->DrawText(0,0,GLUT_BITMAP_HELVETICA_18,0.f,0.f,0.f,"Hi Guys");

	DWORD current_time = timeGetTime();
	DWORD elapsed_time = current_time - previous_time_particle;
	previous_time_particle = current_time;

	elapsed_time_in_sec_particle = elapsed_time / 1000.f;

	//renderer->SetSceneTransform(500.f, 1.f, 1.f, 1.f);


	if (sprite_x > 3)
		sprite_x = 0;

	if (sprite1_x > 3)
		sprite1_x = 0;


	if (particle_time > 10.f)
		particle_time = 0.f;
	//renderer->DrawTexturedRectSeq(0.f, 0.f, 0.f,
	//	256, 1.f, 1.f, 1.f, 1.f,
	//	texture_id_sprite,
	//	sprite_x++, 0, 6, 1, DRAWRANK_CHARACTER);

	for (int i = 0; i < m_objects.size(); ++i) {
		// 생명이 있을경우에만 그린다. 
		if (m_objects[i].GetLife() > 0) {
			// 빌딩에만 이미지 넣쟈
			if (m_objects[i].type == OBJECT_BUILDING && m_objects[i].GetTeam() == TEAM_1) {
				renderer->DrawTexturedRect(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), m_objects[i].Get_R(), m_objects[i].Get_G(), m_objects[i].Get_B(), m_objects[i].Get_A(),
					texture_id_1, m_objects[i].draw_rank);

				// 빌딩 게이지 넣어주기
				renderer->DrawSolidRectGauge(m_objects[i].Get_x(), m_objects[i].Get_y() - m_objects[i].Get_size() / 1.5, m_objects[i].Get_z(),
					m_objects[i].Get_size(), 5.f, 1.f, 0.f, 0.f,
					1.f, (float)m_objects[i].GetLife() / LIFE_BUILDING, m_objects[i].draw_rank);
			}
			else if (m_objects[i].type == OBJECT_BUILDING && m_objects[i].GetTeam() == TEAM_2) {
				renderer->DrawTexturedRect(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), m_objects[i].Get_R(), m_objects[i].Get_G(), m_objects[i].Get_B(), m_objects[i].Get_A(),
					texture_id_2, m_objects[i].draw_rank);

				// 빌딩 게이지 넣어주기
				renderer->DrawSolidRectGauge(m_objects[i].Get_x(), m_objects[i].Get_y() + m_objects[i].Get_size() / 1.5, m_objects[i].Get_z(),
					m_objects[i].Get_size(), 5.f, 0.f, 0.f, 1.f,
					1.f, (float)m_objects[i].GetLife() / LIFE_BUILDING, m_objects[i].draw_rank);

			}

			else if (m_objects[i].type == OBJECT_CHARACTER && m_objects[i].GetTeam() == TEAM_1) {
				//renderer->DrawTexturedRect(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
				//	m_objects[i].Get_size(), m_objects[i].Get_R(), m_objects[i].Get_G(), m_objects[i].Get_B(), m_objects[i].Get_A(),
				//	texture_id_4, m_objects[i].draw_rank);
				renderer->DrawTexturedRectSeq(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), 1.f, 0.f, 0.f, 1.f,
					texture_id_sprite,
					sprite_x++, 0, 14, 8, DRAWRANK_CHARACTER);

				// 빌딩 게이지 넣어주기
				renderer->DrawSolidRectGauge(m_objects[i].Get_x(), m_objects[i].Get_y() + m_objects[i].Get_size(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), 5.f, 1.f, 0.f, 0.f,
					1.f, (float)m_objects[i].GetLife() / LIFE_CHARACTER, m_objects[i].draw_rank);
			}
			else if (m_objects[i].type == OBJECT_CHARACTER && m_objects[i].GetTeam() == TEAM_2) {
				//renderer->DrawTexturedRect(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
				//	m_objects[i].Get_size(), m_objects[i].Get_R(), m_objects[i].Get_G(), m_objects[i].Get_B(), m_objects[i].Get_A(),
				//	texture_id_3, m_objects[i].draw_rank);

				renderer->DrawTexturedRectSeq(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), 0.f, 0.f, 1.f, 1.f,
					texture_id_sprite,
					sprite1_x++, 0, 14, 8, DRAWRANK_CHARACTER);


				// 빌딩 게이지 넣어주기
				renderer->DrawSolidRectGauge(m_objects[i].Get_x(), m_objects[i].Get_y() + m_objects[i].Get_size(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), 5.f, 0.f, 0.f, 1.f,
					1.f, (float)m_objects[i].GetLife() / LIFE_CHARACTER, m_objects[i].draw_rank);

			}
			else if (m_objects[i].type == OBJECT_BULLET) {
				//renderer->DrawTexturedRect(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
				//	m_objects[i].Get_size(), m_objects[i].Get_R(), m_objects[i].Get_G(), m_objects[i].Get_B(), m_objects[i].Get_A(),
				//	texture_id_3, m_objects[i].draw_rank);

				renderer->DrawSolidRect(m_objects[i].Get_x(),
					m_objects[i].Get_y(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), m_objects[i].Get_R(),
					m_objects[i].Get_G(), m_objects[i].Get_B(),
					m_objects[i].Get_A(), m_objects[i].draw_rank);
				particle_time += 0.0005f;
				renderer->DrawParticle(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					10, 1.f, 1.f, 1.f, 1.f,
					-1.f * m_objects[i].Get_Vx() / sqrt(m_objects[i].Get_Vx()*m_objects[i].Get_Vx() + m_objects[i].Get_Vy()*m_objects[i].Get_Vy()),
					-1.f * m_objects[i].Get_Vy() / sqrt(m_objects[i].Get_Vx()*m_objects[i].Get_Vx() + m_objects[i].Get_Vy()*m_objects[i].Get_Vy()),
					texture_id_particle, particle_time);
			}

			else {
				renderer->DrawSolidRect(m_objects[i].Get_x(),
					m_objects[i].Get_y(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), m_objects[i].Get_R(),
					m_objects[i].Get_G(), m_objects[i].Get_B(),
					m_objects[i].Get_A(), m_objects[i].draw_rank);
			}
		}
	}
	
}


void SceneMgr::MouseInput(int x, int y) {
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
	// TEAM_2여야한다.
	else {
		// 남쪽진영에만 배치 가능
		if (y > WINDOW_HEIGHT / 2) {
			if (team_2_character_clocking == false) {
				m_objects.push_back(Object(x - (WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2) - y,
					0.0f, SIZE_CHARACTER, 1.f, 1.f, 1.f, 1.f,
					// 교수님 랜덤방식 참조
					(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
					SPEED_CHARACTER, OBJECT_CHARACTER, LIFE_CHARACTER, LIFETIME_CHARACTER, TEAM_2));
				team_2_character_clocking = true;
			}
			else {
				std::cout << "쿨타임" << std::endl;
			}
		}
		else {
			std::cout << "남쪽진영에만 배치가능" << std::endl;
		}
	}
}