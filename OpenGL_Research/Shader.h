#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL\glew.h>

#define SUCCESS return 0
#define ERROR return 1

class Shader
{
public:
	Shader();
	~Shader();

	void CreateFormString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLoction();
	GLuint GetModelLocation();
	GLuint GetViewLocation();

	void UseShader();
	void ClearShader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView;

	int CompilerShader(const char* vertexCode, const char* fragmentCode);
	int AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
};

