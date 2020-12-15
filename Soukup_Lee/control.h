#pragma once
#include <iostream>
#include <iomanip>
#include "windows.h"
#include <conio.h>
#include "glut.h"
#include "glaux.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glaux.lib")

double w1 = 600.0, h1 = 600.0;
float vx = 0.0, vy = 0.0, vz = 0.0;
double Zoom = 50.0;
double a, b;


void Read_kb(unsigned char key, int, int)				// zoom and control
{
	if (key == 's')
		vx -= 2.0;
	if (key == 'w')
		vx += 2.0;
	if (key == 'a')
		vy -= 2.0;
	if (key == 'd')
		vy += 2.0;
	if (key == '1')
		vz -= 2.0;
	if (key == '3')
		vz += 2.0;
	if (key == '+')
		Zoom -= 2.0;
	if (key == '-')
		Zoom += 2.0;
	if (key == 27)
		exit(0);
	glutPostRedisplay();
}

void  ReadSpec(int key, int, int)						//
{
	if (key == GLUT_KEY_UP)
		b += 1.0;
	if (key == GLUT_KEY_DOWN)
		b -= 1.0;
	if (key == GLUT_KEY_RIGHT)
		a += 1.0;
	if (key == GLUT_KEY_LEFT)
		a -= 1.0;
	glutPostRedisplay();
}

void reshape(int w, int h)								//window
{
	w1 = w;
	h1 = h;
	glutPostRedisplay();
}