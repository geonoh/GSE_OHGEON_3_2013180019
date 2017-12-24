/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"
#include "Object.h"
#include "SceneMgr.h"



SceneMgr* p_Scene = nullptr;
int click_count = 0;

bool left_button_down = false;

DWORD previous_time = 0;

float cool_time_elapsed = 0;
bool is_start = false;
int who_is_win = DRAW;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if (is_start) {
		if (who_is_win == DRAW)
			p_Scene->SceneRender();
		if (who_is_win == TEAM_1) {
			p_Scene->AIWinRender();
		}
		if (who_is_win == TEAM_2) {
			p_Scene->PlayerWinRender();
		}
	}
	else {
		p_Scene->TitleRender();
	}
	glutSwapBuffers();
}

void Idle(void)
{
	// 업데이트는 프레임당 1회. 즉 RendefScene이 호출될때 한 번이면 됨.
	if (is_start) {
		if (who_is_win == DRAW) {
			DWORD current_time = timeGetTime();
			DWORD elapsed_time = current_time - previous_time;
			previous_time = current_time;

			p_Scene->Update((float)elapsed_time);
		}
		who_is_win = p_Scene->who_is_win_scmgr;
	}
	RenderScene();
}

// 클릭하고 땠을 때 
void MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN) {
		left_button_down = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		// 좌 클릭 후 떼면 캐릭터 오브젝트 소환
		if (left_button_down) {		// 클릭됨.
			p_Scene->MouseInput(x, y);
		}
		left_button_down = false;
	}
	RenderScene();
}

void MotionInput(int x, int y) {		// 교수님 코드 참조

}


void KeyInput(unsigned char key, int x, int y)
{
	if (key == 'q') {
		exit(1);
	}
	if (key == 's' || key == 'S') {
		is_start = true;
	}
	std::cout << "test" << std::endl;
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	p_Scene = new SceneMgr(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMotionFunc(MotionInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop();

	delete p_Scene;
    return 0;
}	

