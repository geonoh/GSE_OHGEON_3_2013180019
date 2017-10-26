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

Object::Object(float x, float y, float z, float size, float R, float G, float B, float A) : x(x), y(y), z(z), size(size), R(R), G(G), B(B), A(A) {
	//life = 10;
}

void Object::Setter(float in_x, float in_y, float in_z, float in_size, float in_R, float in_G, float in_B, float in_A) {
	x = in_x;
	y = in_y;
	z = in_z;
	size = in_size;
	R = in_R;
	G = in_G;
	B = in_B;
	A = in_A;
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

void Object::Set_velocity(float x, float y, float z, float i_speed) {
	v_x = x;
	v_y = y;
	v_z = z;
	speed = i_speed;
}

void Object::Update(float time) {
	// prev pos + direction*time
	Sleep(time);

	//printf("%f\n", life);
	if (life <= 0) {
		printf("���\n");
		Setter(-200, -200, -200, 0, 0, 0, 0, 0);
	}


	if (x > 250 || x < -250) 
		v_x = v_x*(-1);
	if (y > 250 || y < -250) 
		v_y = v_y*(-1);

	x += v_x*speed;
	y += v_y*speed;
	//if (size > 15)
	//	is_size_up = false;
	//else if (size < 5)
	//	is_size_up = true;


	//if (is_size_up)
	//	size += 0.1;
	//else
	//	size -= 0.1;

	life -= 0.1;
}

void Object::SetRed() {
	R = 1.0f;
	G = 0.0f;
	B = 0.0f;
	A = 0.0f;
}

void Object::SetLife(float num) {
	life = num;
}

float Object::GetLife() {
	return life;
}