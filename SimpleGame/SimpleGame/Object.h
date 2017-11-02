#pragma once

class Object
{
private:
	float x, y, z;
	float size;
	bool is_size_up;
	float R, G, B, A;
	// �ǽ�3�� �߰� (�ӵ� ����)
	float v_x;
	float v_y;
	float v_z;
	float speed;

	// �ǽ� 5 �߰�
	float life;
public:
	Object(float x, float y, float z, float size, float R, float G, float B, float A);
	Object(float x, float y, float z, float size, float R, float G, float B, float A, float v_x, float v_y, float v_z, float i_speed);

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

	// �浹�ϰ� �Ǹ� SetRed
	void SetRed();
	bool is_collide;
	void SetWhite();

	void SetLife(float num);
	float GetLife();

	void Set_velocity(float x, float y, float z, float i_speed);
	void Update(float time);

	~Object();

};

