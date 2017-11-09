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
	// ���� �浹�Ǹ� �ش� ��ǥ�� ���⺤�� *(-1)
	if (x > 250 || x < -250)
		v_x = v_x*(-1);
	if (y > 250 || y < -250)
		v_y = v_y*(-1);

	// Updated_Position = Prev + �ӵ� ���� * �ð�
	x += v_x*time;
	y += v_y*time;

	// ����������Ʈ�� 0.5�ʸ��� �Ѿ��� �߻��ؾ��Ѵ�.
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
