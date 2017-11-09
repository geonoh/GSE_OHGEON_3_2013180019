#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define MOVE_SPEED			0.01f
#define MAX_OBJECTS_COUNT	10
#define OBJECT_BUILDING		0
#define OBJECT_CHARACTER	1
#define OBJECT_BULLET		2
#define OBJECT_ARROW		3

#define SPEED_BULLET		600
#define SPEED_CHARACTER		300
#define SPEED_ARROW			100


#define LIFETIME_BULLET		8
#define LIFETIME_CHARACTER	20
#define LIFETIME_ARROW		3
#define LIFETIME_ULTIMATE	-1
