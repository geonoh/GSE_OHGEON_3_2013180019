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
	bool shake = false;


	Sound* m_sound;
	int bg_sound;
	int bullet_sound;

public:
	bool is_clock_building = false;
	bool is_clocking = false;

	bool team_1_clocking = false;
	bool team_2_character_clocking = false;

	float team_2_timer = 0.f;



	SceneMgr(float x,float y);
	~SceneMgr();
	void Update(float elapsed_time);
	void CollideCheck();
	void MouseInput(int x, int y);
	void SceneRender();
};

