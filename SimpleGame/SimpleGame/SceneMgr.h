#pragma once
#include "Object.h"
#define MAX_OBJECTS_COUNT 50

class SceneMgr
{
public:
	Object* m_objects[MAX_OBJECTS_COUNT];
	SceneMgr();
	~SceneMgr();
	void Update();
	void CollideCheck();
};

