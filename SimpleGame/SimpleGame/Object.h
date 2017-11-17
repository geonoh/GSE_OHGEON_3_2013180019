#pragma once

class Object
{
private:
	float x, y, z;
	float size;
	bool is_size_up;
	float R, G, B, A;

	// 속도 백터 관련
	float v_x;
	float v_y;
	float v_z;
	float speed;

	int life;
	float life_time;

	int arrow_number = 0;


	int team;

public:

	// 충돌관련 함수
	bool is_collide;

	// 오브젝트 타입
	int type;

	Object();
	Object(float x, float y, float z, float size, float R, float G, float B, float A, 
		float v_x, float v_y, float v_z, float i_speed, int type, int life,float life_time,int team);
	//// 에로우 전용
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


	// type이 Arrow인 경우에는 Arrow의 number을 set해줘야함
	void SetArrowNumber(int set_arrow_number);
	int GetArrowNumber();


	int GetTeam();

	~Object();

};

