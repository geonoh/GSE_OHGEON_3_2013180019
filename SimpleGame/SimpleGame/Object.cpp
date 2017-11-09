#include "stdafx.h"
#include "Object.h"
#include <iostream>
using namespace std;


Object::Object()
{
	//life = 10;
}

Object::~Object()
{
}

Object::Object(float x, float y, float z, float size, float R, float G, float B, float A, float v_x, float v_y, float v_z, float i_speed, int type,int life,float life_time)
	: x(x), y(y), z(z), size(size), R(R), G(G), B(B), A(A), v_x(v_x), v_y(v_y), v_z(v_z), speed(i_speed), type(type), life(life), life_time(life_time)
{
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
	if (x > 250 || x < -250)
		v_x = v_x*(-1);
	if (y > 250 || y < -250)
		v_y = v_y*(-1);

	// Updated_Position = Prev + 속도 벡터 * 시간
	x += v_x*time;
	y += v_y*time;

	// 빌딩오브젝트는 0.5초마다 총알을 발사해야한다.
}

void Object::SetLife(int num) {
	life = num;
}
void Object::LostLife(int num) {
	life -= num;
}

float Object::GetLife() {
	return life;
}

float Object::GetLifeTime() {
	return life_time;
}
void Object::SetLifeTime(float input_life_time) {
	life_time = input_life_time;
}
