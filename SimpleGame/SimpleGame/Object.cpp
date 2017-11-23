#include "stdafx.h"
#include "Object.h"
#include <iostream>
using namespace std;


Object::Object()
{
}

Object::~Object()
{
}

Object::Object(float x, float y, float z, float size, float R, float G, float B, float A, float v_x, float v_y, float v_z, float i_speed, int type,int life,float life_time,int team)
	: x(x), y(y), z(z), size(size), R(R), G(G), B(B), A(A), v_x(v_x), v_y(v_y), v_z(v_z), speed(i_speed), type(type), life(life), life_time(life_time), team(team)
{
	if (type == OBJECT_BUILDING)
		draw_rank = DRAWRANK_BUILDING;
	else if (type == OBJECT_CHARACTER)
		draw_rank = DRAWRANK_CHARACTER;
	else if (type == OBJECT_BULLET)
		draw_rank = DRAWRANK_BULLET;
	else if (type == OBJECT_ARROW)
		draw_rank = DRAWRANK_ARROW;

}

float Object::Get_x() {
	return x;
}
float Object::Get_y() {
	return y;
}
float Object::Get_z() {
	return z;
}
float Object::Get_size() {
	return size;
}
float Object::Get_R() {
	return R;
}
float Object::Get_G() {
	return G;
}
float Object::Get_B() {
	return B;
}
float Object::Get_A() {
	return A;
}

void Object::Update(float time) {
	// 벽에 충돌되면 해당 좌표축 방향벡터 *(-1)
	if (type == OBJECT_BUILDING || type == OBJECT_CHARACTER) {
		if (x >= (WINDOW_WIDTH / 2) || x <= -(WINDOW_WIDTH / 2))
			v_x = v_x*(-1);
		if (y >= (WINDOW_HEIGHT / 2) || y <= -(WINDOW_HEIGHT / 2))
			v_y = v_y*(-1);
	}

	// Updated_Position = Prev + 속도 벡터 * 시간
	x += v_x * time * speed;
	y += v_y * time * speed;

}

void Object::SetLife(int num) {
	life = num;
}
void Object::LostLife(int num) {
	life -= num;
}

int Object::GetLife() {
	return life;
}

float Object::GetLifeTime() {
	return life_time;
}
void Object::SetLifeTime(float input_life_time) {
	life_time = input_life_time;
}


void Object::SetArrowNumber(int set_arrow_number) {
	arrow_number = set_arrow_number;
}

int Object::GetArrowNumber() {
	return arrow_number;
}


int Object::GetTeam() {
	return team;
}