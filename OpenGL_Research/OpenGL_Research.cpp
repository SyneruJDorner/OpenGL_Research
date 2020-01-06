#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SUCCESS return 0
#define ERROR return 1

//Window dimenstions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

#pragma region Shaders
//Vertex Shader
static const char* vShader = "							\n\
#version 330											\n\
														\n\
layout (location = 0) in vec3 pos;						\n\
														\n\
void main()												\n\
{														\n\
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);		\n\
}														\n\
";

//VFragment Shader
static const char* fShader = "							\n\
#version 330											\n\
														\n\
out vec4 colour;										\n\
														\n\
void main()												\n\
{														\n\
	colour = vec4(1.0, 0.0, 0.0, 1.0);					\n\
}														\n\
";
#pragma endregion

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

	//Setup Viewport Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	//Loop until window closes
	while (!glfwWindowShouldClose(window))
	{
		//Get + Handle user input events
		glfwPollEvents();

		//Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
	}

	SUCCESS;
}