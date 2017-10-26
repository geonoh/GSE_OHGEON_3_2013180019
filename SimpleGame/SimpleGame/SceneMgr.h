#pragma once
#include "Object.h"
#include "Renderer.h"
#include <time.h>
#include <stdlib.h>
#include <Windows.h>

#define MAX_OBJECTS_COUNT 10

class SceneMgr
{
	Renderer* renderer;
	Object* m_objects[MAX_OBJECTS_COUNT];
	//Object* user_obejct;
public:
	SceneMgr(float x,float y);
	~SceneMgr();
	void Update();
	void CollideCheck();
	void MouseInput(int x, int y, int num);
	void SceneRender(float x, float y, float z, float size,
		float r, float g, float b, float a);
	void SceneRender();
};

