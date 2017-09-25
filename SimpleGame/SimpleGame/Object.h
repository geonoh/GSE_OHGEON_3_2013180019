#pragma once

class Object
{
private:
	float x, y, z;
	float size;
	float R, G, B, A;
public:
	Object(float x, float y, float z, float size, float R, float G, float B, float A);
	Object();

	void Setter(float x, float y, float z, float size, float R, float G, float B, float A);
	float Get_x();
	float Get_y();
	float Get_z();
	float Get_size();
	float Get_R();
	float Get_G();
	float Get_B();
	float Get_A();

	~Object();

};

