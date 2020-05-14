#include "Camera.h"

Camera::Camera()
{

}

Camera::Camera(glm::vec3 startPos, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startPosSpeed, GLfloat startRotSpeed)
{
	position = startPos;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	forward = glm::vec3(0.0f, 0.0f, -1.0f);
	moveSpeed = startPosSpeed;
	turnSpeed = startRotSpeed;

	Update();
}

Camera::~Camera()
{

}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += forward * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= forward * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
		pitch = 89.0f;

	if (pitch < -89.0f)
		pitch = -89.0f;

	Update();
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}

glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(forward);
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + forward, up);
}

void Camera::Update()
{
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(forward);

	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}