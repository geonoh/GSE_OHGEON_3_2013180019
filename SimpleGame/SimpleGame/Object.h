#pragma once

class Object
{
private:
	float x, y, z;
	float size;
	bool is_size_up;
	float R, G, B, A;
	// 실습3에 추가 (속도 벡터)
	float v_x;
	float v_y;
	float v_z;
	float speed;
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

	void SetRed();


	void Set_velocity(float x, float y, float z, float i_speed);
	void Update(float time);

	~Object();

};

