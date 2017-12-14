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
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//g_Renderer->DrawSolidRect(0, 0, 0, 100, 1, 0, 1, 1);


	// ������Ʈ�� �����Ӵ� 1ȸ. �� RendefScene�� ȣ��ɶ� �� ���̸� ��.
	DWORD current_time = timeGetTime();
	DWORD elapsed_time = current_time - previous_time;
	previous_time = current_time;

	// ������
	
	p_Scene->Update((float)elapsed_time);
	p_Scene->SceneRender();

	glutSwapBuffers();
}

void Idle(void)
{

	RenderScene();
}

// Ŭ���ϰ� ���� �� 
void MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN) {
		left_button_down = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

		// �� Ŭ�� �� ���� ĳ���� ������Ʈ ��ȯ
		if (left_button_down) {		// Ŭ����.
			std::cout << "x, y : " << x << ", " << y << " �ð� : " << cool_time_elapsed << std::endl;

			p_Scene->MouseInput(x, y);
		}
		left_button_down = false;
	}
	RenderScene();
}

void MotionInput(int x, int y) {		// ������ �ڵ� ����

}


void KeyInput(unsigned char key, int x, int y)
{
	if (key == 'q') {
		exit(1);
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

	glutMotionFunc(MotionInput);	// ������ �ڵ� ����
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop();

	delete p_Scene;
    return 0;
}	

