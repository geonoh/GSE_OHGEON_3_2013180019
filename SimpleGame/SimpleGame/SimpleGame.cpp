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

Renderer *g_Renderer = NULL;
Object* p_Obejct = new Object(0.0f, 0.0f, 0.0f, 100.0f, 0.0f, 1.0f, 1.0f, 1.0f);

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	// Renderer Test	(위치x, 위치y, 위치z, 가로 세로 싸이즈, R, G, B, A)
	//g_Renderer->DrawSolidRect(0, 0, 0, 100, 1, 0, 1, 1);

	// 업데이트 한번 해주고 렌더링을 해주자. 
	// 즉 여기에서 업데이트를 한 번 해보자.
	//std::cout << p_Obejct->Get_x() << std::endl;
	p_Obejct->Set_velocity(1.f, 1.f, 0.f, 0.05f);
	p_Obejct->Update(0.f);

	//std::cout << p_Obejct->Get_x() << ", " << p_Obejct->Get_y() << ", " << p_Obejct->Get_z() << std::endl;
	g_Renderer->DrawSolidRect(p_Obejct->Get_x(), 
		p_Obejct->Get_y(), p_Obejct->Get_z(), p_Obejct->Get_size(), 
		p_Obejct->Get_R(), p_Obejct->Get_G(), p_Obejct->Get_B(), p_Obejct->Get_A());

	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN) {
		std::cout << "push_down : ";
		std::cout << x << ", " << y << std::endl;
		p_Obejct->Setter(x - 250.f, 250.f - y, p_Obejct->Get_z(), 
			p_Obejct->Get_size(), p_Obejct->Get_R(), 
			p_Obejct->Get_G(), p_Obejct->Get_B(), p_Obejct->Get_A());
	}
	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	if (key == 'q')
		exit(0);
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
	glutInitWindowSize(500, 500);
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

	// Initialize Renderer
	g_Renderer = new Renderer(500, 500);
	if (!g_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop();

	delete g_Renderer;
	delete p_Obejct;

    return 0;
}

