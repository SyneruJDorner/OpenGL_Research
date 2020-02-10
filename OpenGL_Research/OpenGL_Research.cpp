#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SUCCESS return 0
#define ERROR return 1

//Window dimenstions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

#pragma region Shaders
//Vertex Shader
static const char* vShader = "											\n\
#version 330															\n\
																		\n\
layout (location = 0) in vec3 pos;										\n\
																		\n\
out vec4 vCol;															\n\
																		\n\
uniform mat4 model;														\n\
																		\n\
void main()																\n\
{																		\n\
	gl_Position = model * vec4(pos, 1.0);								\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);							\n\
}																		\n\
";

//VFragment Shader
static const char* fShader = "											\n\
#version 330															\n\
																		\n\
in vec4 vCol;															\n\
																		\n\
out vec4 colour;														\n\
																		\n\
void main()																\n\
{																		\n\
	colour = vCol;														\n\
}																		\n\
";
#pragma endregion

void CreateTriangle()
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

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);

	#pragma region Error handle the shader compiling!
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		ERROR;
	}
	#pragma endregion

	glAttachShader(program, shader);

	SUCCESS;
}

int CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Error creating shader program!\n");
		ERROR;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	#pragma region Error handle the shader compiling!
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		ERROR;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		ERROR;
	}
	#pragma endregion

	uniformModel = glGetUniformLocation(shader, "model");
}

int main()
{
	//Init GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialization failed!");
		glfwTerminate();
		ERROR;
	}

	//Setup GLFW window properties
	//OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Prevent old version code!
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow forward compatibility!
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

	if (!window)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		ERROR;
	}

	//Get Buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(window);

	//Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed!");
		glfwDestroyWindow(window);
		glfwTerminate();
		ERROR;
	}

	glEnable(GL_DEPTH_TEST);

	//Setup Viewport Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	//Loop until window closes
	while (!glfwWindowShouldClose(window))
	{
		//Get + Handle user input events
		glfwPollEvents();

		if (direction)
			triOffset += triIncrement;
		else
			triOffset -= triIncrement;

		if (abs(triOffset) >= triMaxOffset)
			direction = !direction;

		curAngle += 1.0f;
		if (curAngle >= 360)
			curAngle -= 360;

		if (sizeDirection)
			curSize += 0.001f;
		else
			curSize -= 0.001f;

		if (curSize >= maxSize || curSize <= minSize)
			sizeDirection = !sizeDirection;

		//Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
	}

	SUCCESS;
}