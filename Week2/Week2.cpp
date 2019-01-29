

#include "stdafx.h"
#define GLEW_STATIC  
// 链接静态库，必需先定义GLEW_STATIC  

#include <GL/glew.h>  
#include <GL/glut.h>  
#include <stdio.h>
#include "ogldev_math_3d.h"

#pragma comment( lib, "glew32s.lib" )  

GLuint VBO;

/**
* 渲染回调函数
*/
static void RenderScenceCB() {
	// 清空颜色缓存
	glClear(GL_COLOR_BUFFER_BIT);

	// 开启顶点属性
	glEnableVertexAttribArray(0);
	// 绑定GL_ARRAY_BUFFER缓冲器
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 告诉管线怎样解析bufer中的数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	// 开始绘制几何图形(绘制一个点) 
	glDrawArrays(GL_POINTS, 0, 1);
	glDrawArrays(GL_POINTS, 1, 1);
	glDrawArrays(GL_POINTS, 2, 1);


	//  禁用顶点数据
	glDisableVertexAttribArray(0);

	// 交换前后缓存
	glutSwapBuffers();
}


/**
* 创建顶点缓冲器
*/
static void CreateVertexBuffer()
{
	// 创建含有一个顶点的顶点数组
	Vector3f Vertices[3];
	// 将点置于屏幕中央
	Vertices[0] = Vector3f(0.1f, 0.1f, 0.1);
	Vertices[1] = Vector3f(0.2f, 0.2f, 0.2f);
	Vertices[2] = Vector3f(.3f, 0.3f, 0.3f);

	// 创建缓冲器
	glGenBuffers(3, &VBO);
	// 绑定GL_ARRAY_BUFFER缓冲器
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 绑定顶点数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}


int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(400, 300);
	glutCreateWindow("第一个demo");

	// 开始渲染
	glutDisplayFunc(RenderScenceCB);
	
	GLenum err = glewInit();  // 前面运行了glut*的一系列函数，已经获得了opengl的context，所以这里不会出错，读者可以试试在main的开始就调用这句会怎样  
	if (err != GLEW_OK) {
		fprintf(stderr, "%s\n", glewGetErrorString(err));
		return  -1;
	}

	// 创建顶点缓冲器
	CreateVertexBuffer();

	glutMainLoop();
	return 0;
}
