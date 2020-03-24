#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL\glew.h>

#include "GlobalValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"

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

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetEyePositionLocation();

	void SetDirectionalLight(DirectionalLight *dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount);
	void UseShader();
	void ClearShader();

private:
	int pointCountLight;
	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
		   uniformSpecularIntensity, uniformShininess;

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	}	uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	}	uniformPointLight[MAX_POINT_LIGHTS];

	int CompilerShader(const char* vertexCode, const char* fragmentCode);
	int AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
};

