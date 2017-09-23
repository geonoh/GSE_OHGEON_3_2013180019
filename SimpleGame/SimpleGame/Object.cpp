#include "stdafx.h"
#include "Object.h"


Object::Object()
{
}

Object::~Object()
{
}

Object::Object(float x, float y, float z, float size, float R, float G, float B, float A) : x(x), y(y), z(z), size(size), R(R), G(G), B(B), A(A) {
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