
// opengltest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#define NDEBUG   // use Release Lib not Debug Lib
#include <GL/freeglut.h>  //freeGLUT窗口库
#include <math.h>  // Math Lib


int ScreenSize[2] = { 960,720 };

void RenderScene()
{
	//// Clean Color Cache
	//glClear(GL_COLOR_BUFFER_BIT);
	//
	//// Swith the Cache
	//glutSwapBuffers();
}

void ExitWindow(unsigned char KeyInput, int x, int y)
{
	// Detected Input Key ASCII
	if (KeyInput == 27)
	{
		// Exit Windows
		glutExit();
	}
}

void AnimatedColor()
{

	// Set up timer
	float u_time = glutGet(GLUT_ELAPSED_TIME)/1000.f;

	//int ScreenSize = glutGet(GLPoistion)

	// Clean Color Cache
	glClear(GL_COLOR_BUFFER_BIT);

	// Swith the Cache
	glutSwapBuffers();

	// Color After Clean Cache
	glClearColor(0.5+0.5*cos(0.5*u_time), 0.5+0.5*cos(0.5*u_time+2), 0.5+0.5*cos(0.5*u_time+4), 1.0f);

}

int main(int argc, char ** argv)
{
	// InitializeGlut
	glutInit(&argc, argv);

	// DisplayMode, Double-Cache, RGBA
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// WindowsSetting
	glutInitWindowSize(ScreenSize[0], ScreenSize[1]);   // WindowsSize
	glutInitWindowPosition(100,100); //WindowsPosition
	glutCreateWindow("Tutorial 01"); // WindowsTitle

	// Start Render
	glutDisplayFunc(RenderScene);

	// Setup Idle Function
	glutIdleFunc(AnimatedColor);

	// Setup KeyBoard Call Function
	glutKeyboardFunc(ExitWindow);

	// Call GLUT Internal Loop
	glutMainLoop();

	return 0;

	
}