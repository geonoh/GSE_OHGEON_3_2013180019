#pragma once
#include "Object.h"
#include "Renderer.h"
#include "stdafx.h"
#include  "Sound.h"
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <vector>
using namespace std;

class SceneMgr
{
	Renderer* renderer;
	vector<Object> m_objects;

	Sound* m_sound;
	int bg_sound;
	int bullet_sound;
	int collision_sound;
	int big_collision_sound;

	int ai_lost_building = 0;
	int player_lost_building = 0;

public:
	bool is_clock_building = false;
	bool is_clocking = false;
	bool sprite_clocking = true;
	bool team_1_clocking = false;
	bool team_2_character_clocking = false;
	
	int who_is_win_scmgr = DRAW;
	// 첫 번째 총알 발사할때 생기는 에러 걸러주는 변수 
	bool first_error = true;
	bool shaking_effect = false;


	SceneMgr(float x,float y);
	~SceneMgr();
	void Update(float elapsed_time);
	void CollideCheck();
	void MouseInput(int x, int y);
	void SceneRender();
	void TitleRender();
	void AIWinRender();
	void PlayerWinRender();
};

