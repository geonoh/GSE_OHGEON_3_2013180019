#pragma once
#include "Object.h"
#include "Renderer.h"
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <vector>
using namespace std;

#define MAX_OBJECTS_COUNT 100

#define OBJECT_BUILDING		0
#define OBJECT_CHARACTER	1
#define OBJECT_BULLET		2
#define OBJECT_ARROW		3



class SceneMgr
{
	Renderer* renderer;
	vector<Object> m_objects;
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

