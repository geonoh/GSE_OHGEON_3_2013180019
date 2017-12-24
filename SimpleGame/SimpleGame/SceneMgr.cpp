#include "stdafx.h"
#include "SceneMgr.h"

float start_time = 0.0f;
float start_time_building = 0.0f;
float start_time_team1 = 0.f;
float start_time_character_spawn = 0.f;
float start_time_sprite = 0.f;
GLuint texture_id_1 = 0;
GLuint texture_id_2 = 0;
GLuint texture_id_3 = 0;
GLuint texture_id_4 = 0;

GLuint texture_id_background = 0;
GLuint texture_id_sprite = 0;
GLuint texture_id_particle = 0;
GLuint texture_id_particle_player = 0;
GLuint texture_id_climate = 0;
GLuint texture_id_opening = 0;
GLuint texture_victory = 0;
GLuint texture_lose = 0;

int sprite_x = 0;
DWORD previous_time_particle = 0;
float climate_time = 0.f;

float shaking_effect_x = 0.f;
float shaking_effect_y = 0.f;
float shaking_time = 0.f;

bool is_wrong_pos = false;

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
	char particle_climate_path[] = "./Resource/snow.png";
	char particle_player_path[] = "./Resource/particle2.png";
	char open_image_path[] = "./Resource/opening1.png";

	char vic_image_path[] = "./Resource/victory.png";
	char lose_image_path[] = "./Resource/lose.png";

	texture_id_1 = renderer->CreatePngTexture(team_1_path);
	texture_id_2 = renderer->CreatePngTexture(team_2_path);
	texture_id_3 = renderer->CreatePngTexture(team_3_path);
	texture_id_4 = renderer->CreatePngTexture(team_4_path);
	texture_id_background = renderer->CreatePngTexture(back_path);
	texture_id_sprite = renderer->CreatePngTexture(sprite_path);
	texture_id_particle = renderer->CreatePngTexture(particle_path);
	texture_id_climate = renderer->CreatePngTexture(particle_climate_path);
	texture_id_particle_player = renderer->CreatePngTexture(particle_player_path);
	texture_id_opening = renderer->CreatePngTexture(open_image_path);

	texture_victory = renderer->CreatePngTexture(vic_image_path);
	texture_lose = renderer->CreatePngTexture(lose_image_path);

	char music_file_path[] = "./Resource/bgm_.mp3";
	char bullet_file_path[] = "./Resource/bullet.mp3";
	char collision_sound_path[] = "./Resource/fast_collision.mp3";
	char big_collision_sound_path[] = "./Resource/big_collision.mp3";

	m_sound = new Sound();
	bg_sound = m_sound->CreateSound(music_file_path);
	bullet_sound = m_sound->CreateSound(bullet_file_path);
	collision_sound = m_sound->CreateSound(collision_sound_path);
	big_collision_sound = m_sound->CreateSound(big_collision_sound_path);
	m_sound->PlaySoundW(bg_sound, true, 0.5f);
}


SceneMgr::~SceneMgr()
{
	m_objects.clear();
	m_sound->DeleteSound(bg_sound);

	delete renderer;
}

void SceneMgr::Update(float elapsed_time) {
	float elapsed_time_in_sec = elapsed_time / 1000.f;
	if (player_lost_building >= 3) {
		cout << "AI승" << endl;
		who_is_win_scmgr = TEAM_1;
	}

	if (ai_lost_building >= 3) {
		cout << "플레이어승" << endl;
		who_is_win_scmgr = TEAM_2;
	}

	if (sprite_clocking) {
		start_time_sprite += elapsed_time_in_sec;
		if (start_time_sprite >= SPRITE_TIME) {
			start_time_sprite = 0.f;
			sprite_clocking = false;
		}
	}
	if (sprite_clocking == false) {
		sprite_x++;
		if (sprite_x > 9)
			sprite_x = 0;
		sprite_clocking = true;
	}


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
		if (start_time_character_spawn >= PLAYER_COOLTIME) {	// 플레이어 캐릭터 생성 쿨타임 적용
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
	// 화면 흔들리는 효과 
	if (shaking_effect) {
		shaking_time += elapsed_time_in_sec;
		if (shaking_time >= SHAKING_TIME) {
			shaking_effect_x = 0.f;
			shaking_effect_y = 0.f;
			shaking_effect = false;
			renderer->SetSceneTransform(0.f, 0.f, 1.f, 1.f);
			shaking_time = 0.f;
		}
		else if (0.15f <= shaking_time) {
			renderer->SetSceneTransform(-10.f, 10.f, 1.f, 1.f);
		}
		else if (0.1f <= shaking_time) {
			renderer->SetSceneTransform(10.f, -10.f, 1.f, 1.f);
		}
		else if (0.05f <= shaking_time) {
			renderer->SetSceneTransform(-10.f, -10.f, 1.f, 1.f);
		}
		else if (0.f <= shaking_time) {
			renderer->SetSceneTransform(10.f, 10.f, 1.f, 1.f);
		}
	}


	// 각 팀별 빌딩의 bullet 발사 부분
	if (is_clock_building == false && m_objects.size() > 0) {
		bool following_shoot_ai = true;
		bool following_shoot_player = false;

		if (first_error == false) {
			// TEAM_1 빌딩이 생명이 있을때만!
			for (int i = 0; i < 3; ++i) {
				if (m_objects[i].GetLife() > 0 && m_objects[i].type == OBJECT_BUILDING) {
					for (int j = 0; j < m_objects.size() - 1; ++j) {
						// 적의 캐릭터가 있을때
						if (m_objects[j].type == OBJECT_CHARACTER && m_objects[j].GetTeam() == TEAM_2 && following_shoot_ai == true) {
							cout << "캐릭터 방향으로 총알 쏘기" << endl;
							// 적캐릭터(즉, 플레이어)의 방향으로 총알 발사 
							m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
								SIZE_BULLET, 1.f, 0.f, 0.f, 1.f,
								// 적 플레이어의 방향으로 총알 발사하기
								//(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
								(float)(m_objects[j].Get_x() - m_objects[i].Get_x())
								/ (float)sqrt(pow(m_objects[j].Get_x() - m_objects[i].Get_x(), 2.f) + pow(m_objects[j].Get_y() - m_objects[i].Get_y(), 2.f))
								, (float)(m_objects[j].Get_y() - m_objects[i].Get_y())
								/ sqrt(pow(m_objects[j].Get_x() - m_objects[i].Get_x(), 2.f) + pow(m_objects[j].Get_y() - m_objects[i].Get_y(), 2.f))
								, 0.0f,
								SPEED_BULLET, OBJECT_BULLET, LIFE_BULLET, LIFETIME_BULLET, TEAM_1
							));

							m_sound->PlaySoundW(bullet_sound, false, 0.1f);
							following_shoot_ai = false;
						}
						// 만약 생성된 캐릭터가 하나도 없는 경우에는 그냥 랜덤한 방향으로 보내주자.
						else if (j == m_objects.size() - 1 - 1 && following_shoot_ai == true) {
							cout << "player 캐릭터가 없으므로 랜덤하게 발사" << endl;
							m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
								SIZE_BULLET, 1.f, 0.f, 0.f, 1.f,
								// 랜덤 방향으로 총알 발사하기
								(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
								SPEED_BULLET, OBJECT_BULLET, LIFE_BULLET, LIFETIME_BULLET, TEAM_1
							));
							m_sound->PlaySoundW(bullet_sound, false, 0.1f);
							following_shoot_ai = false;
						}
					}
				}
				following_shoot_ai = true;
			}

			// TEAM_2 빌딩이 생명이 있을때만!
			for (int i = 3; i < 6; ++i) {
				if (m_objects[i].GetLife() > 0 && m_objects[i].type == OBJECT_BUILDING) {
					for (int j = 0; j < m_objects.size() - 1; ++j) {
						// 적의 캐릭터가 있을때
						if (m_objects[j].type == OBJECT_CHARACTER && m_objects[j].GetTeam() == TEAM_1 && following_shoot_player == true) {
							cout << "캐릭터 방향으로 총알 쏘기" << endl;
							// 적캐릭터(즉, 플레이어)의 방향으로 총알 발사 
							m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
								SIZE_BULLET, 0.f, 0.f, 1.f, 1.f,
								// 적 플레이어의 방향으로 총알 발사하기
								//(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
								(float)(m_objects[j].Get_x() - m_objects[i].Get_x())
								/ (float)sqrt(pow(m_objects[j].Get_x() - m_objects[i].Get_x(), 2.f) + pow(m_objects[j].Get_y() - m_objects[i].Get_y(), 2.f))
								, (float)(m_objects[j].Get_y() - m_objects[i].Get_y())
								/ sqrt(pow(m_objects[j].Get_x() - m_objects[i].Get_x(), 2.f) + pow(m_objects[j].Get_y() - m_objects[i].Get_y(), 2.f))
								, 0.0f,
								SPEED_BULLET, OBJECT_BULLET, LIFE_BULLET, LIFETIME_BULLET, TEAM_2
							));

							m_sound->PlaySoundW(bullet_sound, false, 0.1f);
							following_shoot_player = false;
						}
						// 만약 생성된 캐릭터가 하나도 없는 경우에는 그냥 랜덤한 방향으로 보내주자.
						else if (j == m_objects.size() - 1 - 1 && following_shoot_player == true) {
							cout << "적 캐릭터가 없으므로 랜덤하게 발사" << endl;
							m_objects.push_back(Object(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
								SIZE_BULLET, 1.f, 0.f, 0.f, 1.f,
								// 랜덤 방향으로 총알 발사하기
								(float)(rand() % 250 - rand() % 250) / 250, (float)(rand() % 250 - rand() % 250) / 250, 0.0f,
								SPEED_BULLET, OBJECT_BULLET, LIFE_BULLET, LIFETIME_BULLET, TEAM_2
							));
							m_sound->PlaySoundW(bullet_sound, false, 0.1f);
							following_shoot_player = false;
						}
					}
				}
				following_shoot_player = true;
			}
		}
		first_error = false;
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
			}

		}

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
			if (m_objects[i].type == OBJECT_BUILDING && m_objects[i].GetTeam() == TEAM_1) {
				ai_lost_building++;
			}
			else if (m_objects[i].type == OBJECT_BUILDING && m_objects[i].GetTeam() == TEAM_2) {
				player_lost_building++;
			}
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

							m_sound->PlaySoundW(big_collision_sound, false, 0.5f);

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
							m_sound->PlaySoundW(big_collision_sound, false, 0.5f);
							shaking_effect = true;
						}

						// 총알의 충돌체크 해줘야함.
						else if (m_objects[i].type == OBJECT_CHARACTER && m_objects[j].type == OBJECT_BULLET) {
							// 캐릭터 충돌함.
							m_objects[i].is_collide = true;
							m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

							// 총알은 닿으면 그냥 erase
							m_objects[j].is_collide = true;
							m_objects.erase(m_objects.begin() + j);
							m_sound->PlaySoundW(collision_sound, false, 0.5f);

						}

						else if (m_objects[i].type == OBJECT_BULLET && m_objects[j].type == OBJECT_CHARACTER) {
							// 캐릭터 충돌함.
							m_objects[j].is_collide = true;
							m_objects[j].SetLife(m_objects[j].GetLife() - m_objects[i].GetLife());

							// 총알은 닿으면 그냥 erase
							m_objects[i].is_collide = true;
							m_objects.erase(m_objects.begin() + i);
							m_sound->PlaySoundW(collision_sound, false, 0.5f);

						}


						// 총알의 충돌체크 해줘야함.
						else if (m_objects[i].type == OBJECT_BUILDING && m_objects[j].type == OBJECT_BULLET) {
							// 캐릭터 충돌함.
							m_objects[i].is_collide = true;
							m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

							// 총알은 닿으면 그냥 erase
							m_objects[j].is_collide = true;
							m_objects.erase(m_objects.begin() + j);
							m_sound->PlaySoundW(collision_sound, false, 0.5f);

						}

						else if (m_objects[i].type == OBJECT_BULLET && m_objects[j].type == OBJECT_BUILDING) {
							// 캐릭터 충돌함.
							m_objects[j].is_collide = true;
							m_objects[j].SetLife(m_objects[j].GetLife() - m_objects[i].GetLife());

							// 총알은 닿으면 그냥 erase
							m_objects[i].is_collide = true;
							m_objects.erase(m_objects.begin() + i);
							m_sound->PlaySoundW(collision_sound, false, 0.5f);

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
								m_sound->PlaySoundW(collision_sound, false, 0.5f);

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
								m_sound->PlaySoundW(collision_sound, false, 0.5f);

							}
						}

						// Arrow와 빌딩
						else if (m_objects[i].type == OBJECT_BUILDING && m_objects[j].type == OBJECT_ARROW) {
							// 캐릭터 충돌함.
							m_objects[i].is_collide = true;
							m_objects[i].SetLife(m_objects[i].GetLife() - m_objects[j].GetLife());

							// Arrow는 닿으면 그냥 erase
							m_objects[j].is_collide = true;
							m_objects.erase(m_objects.begin() + j);
							std::cout << " : 보스체력 ::" << m_objects[i].GetLife() << std::endl;
							m_sound->PlaySoundW(collision_sound, false, 0.5f);


						}
						else if (m_objects[i].type == OBJECT_ARROW && m_objects[j].type == OBJECT_BUILDING) {
							// 캐릭터 충돌함.
							m_objects[j].is_collide = true;
							m_objects[j].SetLife(m_objects[j].GetLife() - m_objects[i].GetLife());

							// Arrow은 닿으면 그냥 erase
							m_objects[i].is_collide = true;
							m_objects.erase(m_objects.begin() + i);
							std::cout << " : 보스체력 ::" << m_objects[j].GetLife() << std::endl;
							m_sound->PlaySoundW(collision_sound, false, 0.5f);

						}
					}
				}
			}
		}
	}
}
void SceneMgr::AIWinRender() {
	renderer->DrawTexturedRect(0.f, 0.f, 0.f, 800.f, 1.f, 1.f, 1.f, 1.f
		, texture_lose, DRAWRANK_BACKGROUND);

}
void SceneMgr::PlayerWinRender() {
	renderer->DrawTexturedRect(0.f, 0.f, 0.f, 800.f, 1.f, 1.f, 1.f, 1.f
		, texture_victory, DRAWRANK_BACKGROUND);

}
void SceneMgr::TitleRender() {
	renderer->DrawTexturedRect(0.f, 0.f, 0.f, 800.f, 1.f, 1.f, 1.f, 1.f
		, texture_id_opening, DRAWRANK_BACKGROUND);
}

void SceneMgr::SceneRender() {
	// 배경그려주기
	renderer->DrawTexturedRect(0.f, 0.f, 0.f,
		1000.f, 1.f, 1.f, 1.f, 1.f,
		texture_id_background, DRAWRANK_BACKGROUND);

	// 적 Character 쿨타임
	glColor3f(1.f, 0.f, 0.f);
	renderer->DrawText(-220.f, 180.f, GLUT_BITMAP_TIMES_ROMAN_24, 0.f, 0.f, 0.f, "COOL TIME");
	renderer->DrawSolidRectGauge(-100.f, 185.f, 0.f, 80, 4, 1.f, 0.f, 0.f, 0.8f, start_time_team1 / ENEMY_GENTIME, DRAWRANK_BUILDING);

	// 아군 Character 쿨타임
	glColor3f(0.f, 0.f, 1.f);
	renderer->DrawText(-220.f, -180.f, GLUT_BITMAP_TIMES_ROMAN_24, 0.f, 0.f, 0.f, "COOL TIME");
	renderer->DrawSolidRectGauge(-100.f, -175.f, 0.f, 80, 4, 0.f, 0.f, 1.f, 0.8f, start_time_character_spawn / PLAYER_COOLTIME, DRAWRANK_BUILDING);

	// Bullet 쿨타임
	glColor3f(1.f, 0.f, 1.f);
	renderer->DrawText(-50.f, 10.f, GLUT_BITMAP_TIMES_ROMAN_24, 0.f, 0.f, 0.f, "Bullet COOL TIME");
	renderer->DrawSolidRectGauge(0.f, 0.f, 0.f, 80, 4, 1.f, 0.f, 1.f, 0.8f, start_time_building / BULLET_GENTIME, DRAWRANK_BUILDING);


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
				renderer->DrawTexturedRectSeq(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), 1.f, 0.f, 0.f, 1.f,
					texture_id_sprite,
					sprite_x, 0, 14, 8, DRAWRANK_CHARACTER);

				// 빌딩 게이지 넣어주기
				renderer->DrawSolidRectGauge(m_objects[i].Get_x(), m_objects[i].Get_y() + m_objects[i].Get_size(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), 5.f, 1.f, 0.f, 0.f,
					1.f, (float)m_objects[i].GetLife() / LIFE_CHARACTER, m_objects[i].draw_rank);
			}
			else if (m_objects[i].type == OBJECT_CHARACTER && m_objects[i].GetTeam() == TEAM_2) {
				renderer->DrawTexturedRectSeq(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), 0.f, 0.f, 1.f, 1.f,
					texture_id_sprite,
					sprite_x, 3, 14, 8, DRAWRANK_CHARACTER);


				// 빌딩 게이지 넣어주기
				renderer->DrawSolidRectGauge(m_objects[i].Get_x(), m_objects[i].Get_y() + m_objects[i].Get_size(), m_objects[i].Get_z(),
					m_objects[i].Get_size(), 5.f, 0.f, 0.f, 1.f,
					1.f, (float)m_objects[i].GetLife() / LIFE_CHARACTER, m_objects[i].draw_rank);

			}
			else if (m_objects[i].type == OBJECT_BULLET) {
				if (m_objects[i].GetTeam() == TEAM_1) {
					renderer->DrawSolidRect(m_objects[i].Get_x(),
						m_objects[i].Get_y(), m_objects[i].Get_z(),
						m_objects[i].Get_size(), m_objects[i].Get_R(),
						m_objects[i].Get_G(), m_objects[i].Get_B(),
						m_objects[i].Get_A(), m_objects[i].draw_rank);
					renderer->DrawParticle(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
						10, 1.f, 1.f, 1.f, 1.f,
						-1.f * m_objects[i].Get_Vx() / sqrt(m_objects[i].Get_Vx()*m_objects[i].Get_Vx() + m_objects[i].Get_Vy()*m_objects[i].Get_Vy()),
						-1.f * m_objects[i].Get_Vy() / sqrt(m_objects[i].Get_Vx()*m_objects[i].Get_Vx() + m_objects[i].Get_Vy()*m_objects[i].Get_Vy()),
						texture_id_particle, m_objects[i].bullet_particle_time, DRAWRANK_PARTICLE);
				}
				else if (m_objects[i].GetTeam() == TEAM_2) {
					renderer->DrawSolidRect(m_objects[i].Get_x(),
						m_objects[i].Get_y(), m_objects[i].Get_z(),
						m_objects[i].Get_size(), m_objects[i].Get_R(),
						m_objects[i].Get_G(), m_objects[i].Get_B(),
						m_objects[i].Get_A(), m_objects[i].draw_rank);
					renderer->DrawParticle(m_objects[i].Get_x(), m_objects[i].Get_y(), m_objects[i].Get_z(),
						10, 1.f, 1.f, 1.f, 1.f,
						-1.f * m_objects[i].Get_Vx() / sqrt(m_objects[i].Get_Vx()*m_objects[i].Get_Vx() + m_objects[i].Get_Vy()*m_objects[i].Get_Vy()),
						-1.f * m_objects[i].Get_Vy() / sqrt(m_objects[i].Get_Vx()*m_objects[i].Get_Vx() + m_objects[i].Get_Vy()*m_objects[i].Get_Vy()),
						texture_id_particle_player, m_objects[i].bullet_particle_time, DRAWRANK_PARTICLE);
				}
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
	climate_time += 0.005;
	renderer->DrawParticleClimate(0, 0, 0, 5.f, 1, 1, 1, 1, -0.1, -0.1, texture_id_climate,
		climate_time, 0.01);

	if (is_wrong_pos) {
		glColor3f(0.f, 0.f, 0.f);
		renderer->DrawText(-180.f, -25.f, GLUT_BITMAP_TIMES_ROMAN_24, 0.f, 0.f, 0.f, "You can't build there!");
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
				is_wrong_pos = false;
			}
			else {
				std::cout << "쿨타임" << std::endl;
			}
		}
		else {
			is_wrong_pos = true;
			std::cout << "남쪽진영에만 배치가능" << std::endl;
		}
	}
}