#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define MAX_OBJECTS_COUNT	10
#define OBJECT_BUILDING		0
#define OBJECT_CHARACTER	1
#define OBJECT_BULLET		2
#define OBJECT_ARROW		3

#define SPEED_BULLET		400
#define SPEED_CHARACTER		300
#define SPEED_ARROW			100

// life
#define LIFE_BUILDING		500
#define LIFE_CHARACTER		100
#define LIFE_BULLET			50
#define LIFE_ARROW			100

// Size
#define SIZE_BUILDING		100.f
#define SIZE_CHARACTER		30.f
#define SIZE_BULLET			4.f
#define SIZE_ARROW			4.f


#define LIFETIME_BULLET		20
#define LIFETIME_CHARACTER	20
#define LIFETIME_ARROW		3
#define LIFETIME_ULTIMATE	-1

#define WINDOW_WIDTH	500
#define WINDOW_HEIGHT	800

#define TEAM_1			0
#define TEAM_2			1
#define DRAW			-1

// time관련
#define ENEMY_GENTIME		5
#define PLAYER_COOLTIME		3
#define ARROW_GENTIME		3
#define BULLET_GENTIME		2
#define SPRITE_TIME			0.1
// effect 관련
#define SHAKING_TIME		0.2

// depth test
#define DRAWRANK_BUILDING	0.1
#define DRAWRANK_PARTICLE	0.1
#define DRAWRANK_CHARACTER	0.2
#define DRAWRANK_BULLET		0.3
#define DRAWRANK_ARROW		0.3
#define DRAWRANK_BACKGROUND	0.9

