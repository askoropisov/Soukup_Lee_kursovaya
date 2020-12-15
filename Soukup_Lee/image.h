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
 
using namespace std;

int newDRP[100][100];
char Lboard[100][100], Sboard[100][100];

void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, w1, h1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, w1 / h1, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(a, b, -Zoom);
    glRotatef(vx, 1, 0, 0);
    glRotatef(vy, 0, 1, 0);
    glRotatef(vz, 0, 0, 1);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            switch (newDRP[i][j])
            {

            case 0: glBegin(GL_QUADS);							//пустые клетки
                glColor3f(1, 1, 1);
                glVertex3f(2 * j + 1 - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j - 16.5, -2 * i - 1 + 16.5, 2);
                glVertex3f(2 * j + 1 - 16.5, -2 * i - 1 + 16.5, 2);
                glEnd();
                break;
            case 999: glBegin(GL_QUADS);						//препятствия
                glColor3f(1, 0, 0);
                glVertex3f(2 * j + 1 - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j - 16.5, -2 * i - 1 + 16.5, 2);
                glVertex3f(2 * j + 1 - 16.5, -2 * i - 1 + 16.5, 2);
                glEnd();
                break;
            case -1: glBegin(GL_QUADS);							//начало (точка А)
                glColor3f(0, 1, 0);
                glVertex3f(2 * j + 1 - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j - 16.5, -2 * i - 1 + 16.5, 2);
                glVertex3f(2 * j + 1 - 16.5, -2 * i - 1 + 16.5, 2);
                glEnd();

                glBegin(GL_TRIANGLES);
                glColor3f(0, 1, 0);
                glVertex3f(2 * j + 1 - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j + 0.5 - 16.5, -2 * i + 16, 3);
                glEnd();

                glBegin(GL_TRIANGLES);
                glColor3f(0, 1, 0);
                glVertex3f(2 * j - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j - 16.5, -2 * i - 1 + 16.5, 2);
                glVertex3f(2 * j + 0.5 - 16.5, -2 * i + 16, 3);
                glEnd();

                glBegin(GL_TRIANGLES);
                glColor3f(0, 1, 0);
                glVertex3f(2 * j - 16.5, -2 * i - 1 + 16.5, 2);
                glVertex3f(2 * j + 1 - 16.5, -2 * i - 1 + 16.5, 2);
                glVertex3f(2 * j + 0.5 - 16.5, -2 * i + 16, 3);
                glEnd();

                glBegin(GL_TRIANGLES);
                glColor3f(0, 1, 0);
                glVertex3f(2 * j + 1 - 16.5, -2 * i - 1 + 16.5, 2);
                glVertex3f(2 * j + 1 - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j + 0.5 - 16.5, -2 * i + 16, 3);
                glEnd();
                break;
            case -2: glBegin(GL_QUADS);							//конец (точка Б)
                glColor3f(0, 0, 1);
                glVertex3f(2 * j + 1 - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j - 16.5, -2 * i - 1 + 16.5, 2);
                glVertex3f(2 * j + 1 - 16.5, -2 * i - 1 + 16.5, 2);
                glEnd();


                glBegin(GL_TRIANGLES);
                glColor3f(0, 0, 1);
                glVertex3f(2 * j + 1 - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j + 0.5 - 16.5, -2 * i + 16, 3);
                glEnd();

                glBegin(GL_TRIANGLES);
                glColor3f(0, 0, 1);
                glVertex3f(2 * j - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j - 16.5, -2 * i - 1 + 16.5, 2);
                glVertex3f(2 * j + 0.5 - 16.5, -2 * i + 16, 3);
                glEnd();

                glBegin(GL_TRIANGLES);
                glColor3f(0, 0, 1);
                glVertex3f(2 * j - 16.5, -2 * i - 1 + 16.5, 2);
                glVertex3f(2 * j + 1 - 16.5, -2 * i - 1 + 16.5, 2);
                glVertex3f(2 * j + 0.5 - 16.5, -2 * i + 16, 3);
                glEnd();

                glBegin(GL_TRIANGLES);
                glColor3f(0, 0, 1);
                glVertex3f(2 * j + 1 - 16.5, -2 * i - 1 + 16.5, 2);
                glVertex3f(2 * j + 1 - 16.5, -2 * i + 16.5, 2);
                glVertex3f(2 * j + 0.5 - 16.5, -2 * i + 16, 3);
                glEnd();
                break;
            case 777: glColor3f(1, 1, 0.5);						//трассировка
                glPushMatrix();
                glTranslatef(2 * j - 16, -2 * i + 16, 3);
                glutSolidCube(1);
                glPopMatrix();
                break;
            }
        }
    }
    glutSwapBuffers();
}


void print_termenal_and_barrier()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cout << setw(2) << Lboard[i][j];
        }
        cout << endl;
    }
}

void print_DRP()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cout << setw(2) << Sboard[i][j];
        }
        cout << endl;
    }
}

void Print_newDRP(char Sboard[100][100])									//заполняем матрицу символами
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (Sboard[i][j] == '0') newDRP[i][j] = 0;
            if (Sboard[i][j] == 'S') newDRP[i][j] = -1;
            if (Sboard[i][j] == 'T') newDRP[i][j] = -2;
            if (Sboard[i][j] == 's') newDRP[i][j] = -1;
            if (Sboard[i][j] == 't') newDRP[i][j] = -2;
            if (Sboard[i][j] == '#') newDRP[i][j] = 999;
            if (Sboard[i][j] == '*') newDRP[i][j] = 777;
        }
    }
}