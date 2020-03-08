#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

#define SUCCESS return 0
#define ERROR return 1

//Window dimenstions
Window window;
const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

#pragma region Shaders
static const char* vShader = "Shaders/Shader.vert";//Vertex Shader
static const char* fShader = "Shaders/Shader.frag";//VFragment Shader
#pragma endregion

void InitObjects()
{
	unsigned int indicies[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0, 
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, -0.5f,
		0.0f, -1.0f, 0.5f,
		1.0f, -1.0f, -0.5f,
		0.0f, 1.0f, 0.0f
	};

	Mesh* obj_1 = new Mesh();
	obj_1->CreateMesh(vertices, indicies, 12, 12);
	meshList.push_back(obj_1);
}

void InitShaders()
{
	Shader* shader_1 = new Shader();
	shader_1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader_1);
}

int main()
{
	window = Window(800, 600);
	window.Initialize();

	InitObjects();
	InitShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, window.GetBufferWidth() / window.GetBufferHeight(), 0.1f, 100.0f);

	//Loop until window closes
	while (!window.GetShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Get + Handle user input events
		glfwPollEvents();

		camera.keyControl(window.getKeys(), deltaTime);
		camera.MouseControl(window.getXChange(), window.getYChange());

		//Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLoction();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		meshList[0]->RenderMesh();
		glUseProgram(0);

		window.SwapBuffers();
	}

	SUCCESS;
}