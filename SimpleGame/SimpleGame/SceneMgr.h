#pragma once
#include "Object.h"
#include "Renderer.h"
#include "stdafx.h"
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <vector>
using namespace std;

class SceneMgr
{
	Renderer* renderer;
	vector<Object> m_objects;
public:
	bool is_shooting = false;
	bool is_clocking = true;
	SceneMgr(float x,float y);
	~SceneMgr();
	void Update();
	void CollideCheck();
	void MouseInput(int x, int y, int object_type);
	void SceneRender();
};

