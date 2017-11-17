#pragma once

class Object
{
private:
	float x, y, z;
	float size;
	bool is_size_up;
	float R, G, B, A;

	// �ӵ� ���� ����
	float v_x;
	float v_y;
	float v_z;
	float speed;

	int life;
	float life_time;

	int arrow_number = 0;


	int team;

public:

	// �浹���� �Լ�
	bool is_collide;

	// ������Ʈ Ÿ��
	int type;

	Object();
	Object(float x, float y, float z, float size, float R, float G, float B, float A, 
		float v_x, float v_y, float v_z, float i_speed, int type, int life,float life_time,int team);
	//// ���ο� ����
	//Object(float x, float y, float z, float size, float R, float G, float B, float A,
	//	float v_x, float v_y, float v_z, float i_speed, int type, int life, float life_time, int arrow_num,int team);

	float Get_x();
	float Get_y();
	float Get_z();
	float Get_size();
	float Get_R();
	float Get_G();
	float Get_B();
	float Get_A();

	float GetLifeTime();
	void SetLifeTime(float input_life_time);

	void SetLife(int num);
	void LostLife(int num);
	int GetLife();

	void Update(float time);


	// type�� Arrow�� ��쿡�� Arrow�� number�� set�������
	void SetArrowNumber(int set_arrow_number);
	int GetArrowNumber();


	int GetTeam();

	~Object();

};

