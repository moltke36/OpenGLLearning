

#include "stdafx.h"
#define GLEW_STATIC  
// 链接静态库，必需先定义GLEW_STATIC  

#include <GL/glew.h>  
#include <GL/glut.h>  

#include <stdio.h>
#include <string>

#include "ogldev_math_3d.h"
#include "Pipeline.h"
#include <assert.h>

#include <iostream>
#include <fstream>
#pragma comment( lib, "glew32s.lib" )  

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

// 定义要读取的顶点着色器脚本和片断着色器脚本的文件名，作为文件读取路径（这样的话shader.vs和shader.fs文件要放到工程的根目录下，保证下面定义的是这两个文件的文件路径）
const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

GLuint VBO;
// Create a Index buffer object
GLuint IBO;
// 中间变量
GLuint gScaleLocation;
GLuint gWorldLocation;

PersProjInfo gPersProjInfo;


/**
* 读取文件
*/

bool ReadFile(const char* pFileName, std::string& outFile)
{
	std::ifstream f(pFileName);

	bool ret = false;

	if (f.is_open()) {
		printf("open file %s \n", pFileName);
		std::string line;
		while (std::getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		ret = true;
	}
	else {
		printf("can't open file %s", pFileName);
	}

	return ret;
}



/**
* 渲染回调函数
*/
static void RenderScenceCB() {
	// 清空颜色缓存
	glClear(GL_COLOR_BUFFER_BIT);

	// Declare a Scale Value
	static float Scale = 0.0f;

	// Evevry Time Call it, will change the value
	Scale += 0.001f;

	Matrix4f World;

	// Translation
	//World.m[0][0] = 1.0f; World.m[0][1] = 0.0f; World.m[0][2] = 0.0f; World.m[0][3] = sinf(Scale);
	//World.m[1][0] = 0.0f; World.m[1][1] = 1.0f; World.m[1][2] = 0.0f; World.m[1][3] = 0.0f;
	//World.m[2][0] = 0.0f; World.m[2][1] = 0.0f; World.m[2][2] = 1.0f; World.m[2][3] = 0.0f;
	//World.m[3][0] = 0.0f; World.m[3][1] = 0.0f; World.m[3][2] = 0.0f; World.m[3][3] = 1.0f;

	// Rotation
	//World.m[0][0] = 1.0f;		 World.m[0][1] = 0.0f;			World.m[0][2] = 0.0f;				World.m[0][3] = 0.0f;
	//World.m[1][0] = 0.0f;		 World.m[1][1] = cosf(Scale);	World.m[1][2] = -sinf(Scale);		World.m[1][3] = 0.0f;
	//World.m[2][0] = 0.0f;		 World.m[2][1] = sinf(Scale);   World.m[2][2] = cosf(Scale);		World.m[2][3] = 0.0f;
	//World.m[3][0] = 0.0f;        World.m[3][1] = 0.0f;			World.m[3][2] = 0.0f;				World.m[3][3] = 1.0f;

	// Scaling
	//World.m[0][0] = sinf(Scale); World.m[0][1] = 0.0f;        World.m[0][2] = 0.0f;        World.m[0][3] = 0.0f;
	//World.m[1][0] = 0.0f;        World.m[1][1] = sinf(Scale); World.m[1][2] = 0.0f;        World.m[1][3] = 0.0f;
	//World.m[2][0] = 0.0f;        World.m[2][1] = 0.0f;        World.m[2][2] = sinf(Scale); World.m[2][3] = 0.0f;
	//World.m[3][0] = 0.0f;        World.m[3][1] = 0.0f;        World.m[3][2] = 0.0f;        World.m[3][3] = 1.0f;

	Pipeline p;
	//p.Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
	//p.WorldPos(sinf(Scale), 0.0f, 0.0f);
	//p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);

	p.Rotate(0.0f, Scale*50.0, 0.0f);
	p.WorldPos(0.0f, 0.0f, 5.0f);
	p.Scale(0.5, 0.5, 0.5);
	// 初始化透视变换配置参数
	p.SetPerspectiveProj(25.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 1000.0f);
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());

	//glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);

	// 将值传递给shader
	glUniform1f(gScaleLocation, sinf(Scale));

	// 开启顶点属性
	glEnableVertexAttribArray(0);
	// 绑定GL_ARRAY_BUFFER缓冲器
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 告诉管线怎样解析bufer中的数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// 绑定GL_ELEMENT_ARRAY_BUFFER缓冲器
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// 开始绘制几何图形(绘制ELEMENT) 
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// 开始绘制几何图形(绘制一个点) 
	glDrawArrays(GL_TRIANGLES, 0,3);

	//  禁用顶点数据
	glDisableVertexAttribArray(0);

	// 交换前后缓存
	glutSwapBuffers();

	glutPostRedisplay();
}


static void InitializeGlutCallbacks()
{
	glutIdleFunc(RenderScenceCB);
	glutDisplayFunc(RenderScenceCB);
}

/**
* 创建Index Buffer
*/
static void CreateIndexBuffer()
{
	unsigned int Indices[] = { 0, 1, 2,
		0, 2, 3,
		2, 3, 4,
		2, 4, 5,
		1,2,5,
		1,5,6,
		5,6,7,
		5,4,7,
		0,4,7,
		0,4,3,
		1,7,0,
		1,7,6};
	glGenBuffers(1, &IBO);
	// 绑定GL_ELEMENT_ARRAY_BUFFER缓冲器
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// 绑定顶点数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);


}

/**
* 创建顶点缓冲器
*/
static void CreateVertexBuffer()
{
	// 创建含有一个顶点的顶点数组
	Vector3f Vertices[8];
	// 将点置于屏幕中央
	Vertices[0] = Vector3f(-1.0f, 1.0f, -1.0f);
	Vertices[1] = Vector3f(1.0f, 1.0f, -1.0f);
	Vertices[2] = Vector3f(1.0f, 1.0f, 1.0f);
	Vertices[3] = Vector3f(-1.0f, 1.0f, 1.0f);
	Vertices[4] = Vector3f(-1.0f, -1.0f, 1.0f);
	Vertices[5] = Vector3f(1.0f, -1.0f, 1.0f);
	Vertices[6] = Vector3f(1.0f, -1.0f, -1.0f);
	Vertices[7] = Vector3f(-1.0f, -1.0f, -1.0f);


	// 创建缓冲器
	glGenBuffers(8, &VBO);
	// 绑定GL_ARRAY_BUFFER缓冲器
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 绑定顶点数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

/**
* 创建Shader
*/
static void AddShader(GLuint ShaderProgram, const char*pShaderText, GLenum ShaderType)
{
	// 根据shader类型参数定义两个shader对象
	GLuint ShaderObj = glCreateShader(ShaderType);
	// 检查是否定义成功
	if (ShaderObj == 0) 
	{
		fprintf(stderr, "Error create shader type %d\n", ShaderType);
		exit(0);
	}

	// 定义shader的代码源
	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Length[1];
	Length[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Length); // ShaderObj对象，元素个数，pchar指针，每个数组长度
	glCompileShader(ShaderObj);// 编译shader对象


	// 检查和shader相关的错误
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}


	// 将编译好的shader对象绑定到program object程序对象上
	glAttachShader(ShaderProgram, ShaderObj);
}


// 编译着色器函数
static void CompileShaders()
{
	// 创建着色器程序
	GLuint ShaderProgram = glCreateProgram();
	// 检查是否创建成功
	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// 存储着色器文本的字符串缓冲
	std::string vs, fs;

	// 分别读取着色器文件中的文本到字符串缓冲区
	if (!ReadFile(pVSFileName, vs)) {
		exit(1);
	};
	if (!ReadFile(pFSFileName, fs)) {
		exit(1);
	};

	//std::cout << "Vertex Shader: " << vs << "\n";
	//std::cout << "Fragment Shader: " << fs << "\n";

	// 添加顶点着色器和片段着色器
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	// 链接shader着色器程序，并检查程序相关错误
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	// 检查验证在当前的管线状态程序是否可以被执行
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	// 设置到管线声明中来使用上面成功建立的shader程序
	glUseProgram(ShaderProgram);

	// 查询获取一致变量的位置
	gScaleLocation = glGetUniformLocation(ShaderProgram, "gScale");
	// 检查错误
	assert(gScaleLocation != 0xFFFFFFFF);
}




int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("第一个demo");


	
	// 开始渲染
	InitializeGlutCallbacks();
	glutPostRedisplay();
	

	GLenum err = glewInit();  // 前面运行了glut*的一系列函数，已经获得了opengl的context，所以这里不会出错，读者可以试试在main的开始就调用这句会怎样  
	if (err != GLEW_OK) {
		fprintf(stderr, "%s\n", glewGetErrorString(err));
		return  -1;
	}

	// 创建顶点缓冲器
	CreateVertexBuffer();

	//  创建Index(目录)缓冲器
	CreateIndexBuffer();

	// 编译着色器
	CompileShaders();


	glutMainLoop();
	return 0;
}
