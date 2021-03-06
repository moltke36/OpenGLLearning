// Quizz01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define GLEW_STATIC
// 链接静态库，必需先定义GLEW_STATIC  

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ogldev_math_3d.h"
#include <fstream>
#include <string>

// 链接glew32s.lib库
#pragma comment( lib, "glew32s.lib" )  

const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

// Create a VertexBufferObject
GLuint VBO;

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

static void DrawArrayBuffer()
{
	// Enable Vertex buffer attribute
	//glEnableVertexArrayAttrib(VBO, 0);
	glEnableVertexAttribArray(0);

	// Bind GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Tell Pipline how to render
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Start to Draw
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Disable Vertex attribute
	glDisableVertexAttribArray(0);

}

static void RenderScene()
{
	// Clean Color Cache
	glClear(GL_COLOR_BUFFER_BIT);

	// Switch the Front and Back Cache
	//glutSwapBuffers();

	// Setup Background color
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Draw Array
	DrawArrayBuffer();

	// Switch Render Cache
	glutSwapBuffers();
}

static void ExitWindow(unsigned char KeyInput, int x, int y)
{
	// Detected Input Key ASCII
	if (KeyInput == 27)
	{
		// Exit Windows
		glutExit();
	}
}

static void CreateVertexBuff()
{

	// Create a Vector3f Point Buffer
	Vector3f Vertices[3];

	// Put the Vertices
	Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
	Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
	Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

	// Create VertexBuffer
	glGenBuffers(sizeof(Vertices),&VBO);

	// Bind the VertexBuff to Array Buff
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Bind Vertics Data to Buff
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// Create Shader
	GLuint ShaderObj = glCreateShader(ShaderType);

	// Specify source code
	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);

	// Compile Shader
	glCompileShader(ShaderObj);

	// Check Compile Error
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s' \n", ShaderType, InfoLog);
		exit(1);
	}

	// Attach Shader object to Shader Program
	glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShader()
{
	// Set up Program object
	GLuint ShaderProgram = glCreateProgram();

	// Check Shader Program creating
	if (ShaderProgram == 0)
	{
		fprintf(stderr, "Error Creating Shader Program\n");
		exit(1);
	}

	// Create Shader Source Text
	std::string vs, fs;

	// Load the shader file
	if (!ReadFile(pVSFileName, vs))
	{
		fprintf(stderr, "Error Loading Vertex Shader\n");
		exit(1);
	}
	if (!ReadFile(pFSFileName, fs))
	{
		fprintf(stderr, "Error Loading Fragment Shader\n");
		exit(1);
	}
	

	// Create Shaders
	AddShader(ShaderProgram,vs.c_str(),GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(),GL_FRAGMENT_SHADER);

	// Link Shader Program
	glLinkProgram(ShaderProgram);

	// Check Linking Status
	GLint Success;
	GLchar ErrorLog[1024] = { 0 };
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success==0)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	// Validate Shader Program
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error Validating shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	// Link Shader Program Into Pipeline
	glUseProgram(ShaderProgram);
}

int main(int argcp, char **argv)
{
	// Initialize Glut
	glutInit(&argcp, argv);

	// Setup DisplayMode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// WindowsSetting
	glutInitWindowSize(480, 480);
	glutInitWindowSize(200, 200);
	glutCreateWindow("Quizz 01");

	// Start Render
	glutDisplayFunc(RenderScene);

	// Start Glew
	GLenum Error = glewInit();

	// Check Glew Init Succeedly
	if (Error != GLEW_OK)
	{
		fprintf(stderr, "%s\n", glewGetErrorString(Error));
		return -1;
	}
	
	// Create Vertex Buffer
	CreateVertexBuff();

	// Compile Shaders
	CompileShader();

	// Setup KeyBoard Call Function
	glutKeyboardFunc(ExitWindow);

	glutIdleFunc(RenderScene);



	glutMainLoop();

    return 0;
}

