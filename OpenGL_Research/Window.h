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
	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	inline void SwapBuffers() { glfwSwapBuffers(window); }

private:
	GLFWwindow* window;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
	bool keys[1024];
	GLfloat lastX, lastY, xChange, yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

