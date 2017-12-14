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


	// TEAM_1 ���� �������ֱ�.
	m_objects.push_back(Object(-200.f, 300.f, 0.f,
		SIZE_BUILDING, 1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, LIFE_BUILDING, LIFETIME_ULTIMATE, TEAM_1));

	// TEAM_1 ���� �������ֱ�.
	m_objects.push_back(Object(0.f, 300.f, 0.f,
		SIZE_BUILDING, 1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, LIFE_BUILDING, LIFETIME_ULTIMATE, TEAM_1));

	// TEAM_1 ���� �������ֱ�.
	m_objects.push_back(Object(200.f, 300.f, 0.f,
		SIZE_BUILDING, 1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, LIFE_BUILDING, LIFETIME_ULTIMATE, TEAM_1));


	// TEAM_2 ���� �������ֱ�.
	m_objects.push_back(Object(-200.f, -300.f, 0.f,
		SIZE_BUILDING, 1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, LIFE_BUILDING, LIFETIME_ULTIMATE, TEAM_2));

	// TEAM_2 ���� �������ֱ�.
	m_objects.push_back(Object(0.f, -300.f, 0.f,
		SIZE_BUILDING, 1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, LIFE_BUILDING, LIFETIME_ULTIMATE, TEAM_2));

	// TEAM_2 ���� �������ֱ�.
	m_objects.push_back(Object(200.f, -300.f, 0.f,
		SIZE_BUILDING, 1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 0.f,
		OBJECT_BUILDING, LIFE_BUILDING, LIFETIME_ULTIMATE, TEAM_2));




	// Image ID ������ֱ�
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

	// �ð� ���� ����
	if (is_clock_building) {
		start_time_building += elapsed_time_in_sec;
		if (start_time_building >= BULLET_GENTIME) {	// 10�ʰ� �Ѿ�� clocking �����ϰ�, Bullet push back.
			is_clock_building = false;
			start_time_building = 0.f;
		}
	}

	if (team_2_character_clocking) {
		start_time_character_spawn += elapsed_time_in_sec;
		if (start_time_character_spawn >= PLAYER_COOLTIME) {	// 10�ʰ� �Ѿ�� clocking �����ϰ�, Bullet push back.
			team_2_character_clocking = false;
			start_time_character_spawn = 0.f;
		}
	}

	if (is_clocking) {
		start_time += elapsed_time_in_sec;
		if (start_time >= ARROW_GENTIME) {	// 3.0�ʰ� �Ѿ�� clocking �����ϰ�, Character Arrow push back.
			is_clocking = false;
			start_time = 0.f;
		}
	}

	if (team_1_clocking) {
		start_time_team1 += elapsed_time_in_sec;
		if (start_time_team1 >= ENEMY_GENTIME) {	// 5.f�ʰ� �Ѿ�� clocking �����ϰ�, Team1 Character ���� push back.
			team_1_clocking = false;
			start_time_team1 = 0.f;
		}
	}
	(float)(rand() % 250 - rand() % 250) / 250;
	// �� ���� ������ bullet �߻� �κ�
	if (is_clock_building == false && m_objects.size() > 0) {
		// TEAM_1 ������ ������ ��������!
		for (int i = 0; i < 3; ++i) {
			if (m_objects[i].GetLife() > 0 && m_objects[i].type == OBJECT_BUILDING) {
				m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					SIZE_BULLET, 1.f, 0.f, 0.f, 1.f,
					// ������ ������� ����
					(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
					SPEED_BULLET, OBJECT_BULLET, LIFE_BULLET, LIFETIME_BULLET, TEAM_1
				));
				m_sound->PlaySoundW(bullet_sound, false, 5.2f);
				cout << "��� 1����" << endl;
			}
		}

		// TEAM_2 ������ ������ ��������!
		for (int i = 3; i < 6; ++i) {
			if (m_objects[i].GetLife() > 0 && m_objects[i].type == OBJECT_BUILDING) {
				m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					SIZE_BULLET, 0.f, 0.f, 1.f, 1.f,
					// ������ ������� ����
					(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
					SPEED_BULLET, OBJECT_BULLET, LIFE_BULLET, LIFETIME_BULLET, TEAM_2
				));
				m_sound->PlaySoundW(bullet_sound, false, 5.2f);
				cout << "��� 2����" << endl;
			}
		}
		is_clock_building = true;
	}


	// �̷��� ����� �� �������� ���α׷��� ������ �ʴ´ܴ�.
	if (is_clocking == false && m_objects.size() > 0) {
		// ĳ������ Arrow �߻�
		for (int i = 0; i < m_objects.size(); ++i) {
			// ĳ���� ���������̸�.
			if (m_objects[i].type == OBJECT_CHARACTER && m_objects[i].GetTeam() == TEAM_1) {
				m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					SIZE_ARROW, 0.5f, 0.2f, 0.7f, 1.f,
					 (float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
					SPEED_ARROW, OBJECT_ARROW, LIFE_ARROW, LIFETIME_ARROW, TEAM_1
				));
				// ĳ���Ϳ��� �߻��� Arrow�� �� ĳ������ ���� Arrow_num�� �־���Ѵ�.
				// Arrow�� push_back �Ǿ����Ƿ� ���� �߰��� object�� ��������° �ִ�.

				m_objects[m_objects.size() - 1].SetArrowNumber(i);
				//std::cout << m_objects.size() - 1 << "��° Arrow�̰� �� Arrow�� " << i << "���� Character" << std::endl;
			}

			// ��2�� ĳ���� Arrow. 
			if (m_objects[i].type == OBJECT_CHARACTER && m_objects[i].GetTeam() == TEAM_2) {
				m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					SIZE_ARROW, 1.f, 1.f, 0.f, 1.f,
					(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
					SPEED_ARROW, OBJECT_ARROW, LIFE_ARROW, LIFETIME_ARROW, TEAM_2
				));
				// ĳ���Ϳ��� �߻��� Arrow�� �� ĳ������ ���� Arrow_num�� �־���Ѵ�.
				// Arrow�� push_back �Ǿ����Ƿ� ���� �߰��� object�� ��������° �ִ�.

				m_objects[m_objects.size() - 1].SetArrowNumber(i);
				//std::cout << m_objects.size() - 1 << "��° Arrow�̰� �� Arrow�� " << i << "���� Character" << std::endl;
			}

		}

		//std::cout << "�߻� !" << " ���� : " << number << std::endl;
		is_clocking = true;
	}


	// Team1 �� ��������
	if (team_1_clocking == false && m_objects.size() > 0) {
		// TEAM_1 ������ ������ ��������!
		int is_team1_alive = 0;
		for (int i = 0; i < 3; ++i) {
			// ������ �ϳ��� ��������� 
			if (m_objects[i].GetLife() > 0) {
				is_team1_alive++;
			}

		}
		if (is_team1_alive > 0) {
			std::cout << "ĳ���ͻ���" << std::endl;
			m_objects.push_back(Object(-250 + rand() % 500, rand() % 400, 0.f,
				SIZE_CHARACTER, 1.f, 1.f, 1.f, 1.f,
				// ������ ������� ����
				(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
				SPEED_CHARACTER, OBJECT_CHARACTER, LIFE_CHARACTER, LIFETIME_CHARACTER, TEAM_1
			));
			is_team1_alive = 0;
		}
		team_1_clocking = true;
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
				// ���� �޶�� �浹üũ ����
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

						// ĳ���Ϳ� ĳ������ ���
						// �浹�ص� ������ ����.
						if (m_objects[i].type == OBJECT_CHARACTER && m_objects[j].type == OBJECT_CHARACTER) {
							//std::cout << "ĳ���ͳ��� �浹" << std::endl;
							//m_objects.erase(m_objects.begin() + i);
							//m_objects.erase(m_objects.begin() + j);

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


						// �Ѿ��� �浹üũ �������.
						else if (m_objects[i].type == OBJECT_BUILDING && m_objects[j].type == OBJECT_BULLET) {
							// ĳ���� �浹��.
							m_objects[i].is_collide = true;
							m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

							// �Ѿ��� ������ �׳� erase
							m_objects[j].is_collide = true;
							m_objects.erase(m_objects.begin() + j);
						}

						else if (m_objects[i].type == OBJECT_BULLET && m_objects[j].type == OBJECT_BUILDING) {
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
}


void SceneMgr::SceneRender() {
	// ���׷��ֱ�
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
		// ������ ������쿡�� �׸���. 
		if (m_objects[i].GetLife() > 0) {
			// �������� �̹��� ����
			if (m_objects[i].type == OBJECT_BUILDING && m_objects[i].GetTeam() == TEAM_1) {
				renderer->DrawTexturedRect(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), m_objects[i].Get_R(), m_objects[i].Get_G(), m_objects[i].Get_B(), m_objects[i].Get_A(),
					texture_id_1, m_objects[i].draw_rank);

				// ���� ������ �־��ֱ�
				renderer->DrawSolidRectGauge(m_objects[i].Get_x(), m_objects[i].Get_y() - m_objects[i].Get_size() / 1.5, m_objects[i].Get_z(),
					m_objects[i].Get_size(), 5.f, 1.f, 0.f, 0.f,
					1.f, (float)m_objects[i].GetLife() / LIFE_BUILDING, m_objects[i].draw_rank);
			}
			else if (m_objects[i].type == OBJECT_BUILDING && m_objects[i].GetTeam() == TEAM_2) {
				renderer->DrawTexturedRect(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), m_objects[i].Get_R(), m_objects[i].Get_G(), m_objects[i].Get_B(), m_objects[i].Get_A(),
					texture_id_2, m_objects[i].draw_rank);

				// ���� ������ �־��ֱ�
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

				// ���� ������ �־��ֱ�
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


				// ���� ������ �־��ֱ�
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
	// TEAM_2�����Ѵ�.
	else {
		// ������������ ��ġ ����
		if (y > WINDOW_HEIGHT / 2) {
			if (team_2_character_clocking == false) {
				m_objects.push_back(Object(x - (WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2) - y,
					0.0f, SIZE_CHARACTER, 1.f, 1.f, 1.f, 1.f,
					// ������ ������� ����
					(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
					SPEED_CHARACTER, OBJECT_CHARACTER, LIFE_CHARACTER, LIFETIME_CHARACTER, TEAM_2));
				team_2_character_clocking = true;
			}
			else {
				std::cout << "��Ÿ��" << std::endl;
			}
		}
		else {
			std::cout << "������������ ��ġ����" << std::endl;
		}
	}
}