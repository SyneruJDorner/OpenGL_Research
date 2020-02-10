#pragma once
#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();
	~Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialize();

	inline GLfloat GetBufferWidth() { return bufferWidth; }
	inline GLfloat GetBufferHeight() { return bufferHeight; }
	inline bool GetShouldClose() { return glfwWindowShouldClose(window); }
	inline void SwapBuffers() { glfwSwapBuffers(window); }

private:
	GLFWwindow* window;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

