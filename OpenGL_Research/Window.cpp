#include "Window.h"

#define SUCCESS return 0
#define ERROR return 1

Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(window, handleKeys);
	glfwSetCursorPosCallback(window, handleMouse);
}

GLfloat Window::getXChange()
{
	GLfloat change = xChange;
	xChange = 0;
	return change;
}

GLfloat Window::getYChange()
{
	GLfloat change = yChange;
	yChange = 0;
	return change;

}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			win->keys[key] = true;
		else if (action == GLFW_RELEASE)
			win->keys[key] = false;
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (win->mouseFirstMoved)
	{
		win->lastX = xPos;
		win->lastY = yPos;
		win->mouseFirstMoved = false;
	}

	win->xChange = xPos - win->lastX;
	win->yChange = win->lastY - yPos;

	win->lastX = xPos;
	win->lastY = yPos;
}

int Window::Initialize()
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

	window = glfwCreateWindow(width, height, "Test Window", NULL, NULL);

	if (!window)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		ERROR;
	}

	//Get Buffer size information
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(window);

	//Handle Key + Mouse Input
	createCallbacks();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	glfwSetWindowUserPointer(window, this);
}