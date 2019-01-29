

#include "stdafx.h"
#define GLEW_STATIC  
// 链接静态库，必需先定义GLEW_STATIC  

#include <GL/glew.h>  
#include <GL/glut.h>  
#include <stdio.h>  

#pragma comment( lib, "glew32.lib" )  

void  init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void  drawLine() {
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_LINE_STIPPLE);
	GLushort  patn = 0xFAFA;
	glLineStipple(3, patn);
	glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_LINE_LOOP);
	glVertex2i(10, 10);
	glVertex2f(100.0, 75.3);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(70, 80);
	glEnd();

	glFlush();
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(400, 300);
	glutCreateWindow("第一个demo");

	GLenum err = glewInit();  // 前面运行了glut*的一系列函数，已经获得了opengl的context，所以这里不会出错，读者可以试试在main的开始就调用这句会怎样  
	if (err != GLEW_OK) {
		fprintf(stderr, "%s\n", glewGetErrorString(err));
		return  -1;
	}

	init();
	glutDisplayFunc(drawLine);
	glutMainLoop();
	return 0;
}
