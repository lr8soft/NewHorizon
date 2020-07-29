#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../HorizonFrame.h"

#include "../Util/LogUtil.hpp"
void Camera::processInput(float deltaTime)
{
	GLFWwindow* window = HorizonFrame::getInstance()->getScreen();
	float velocity = MovementSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Position += Front * velocity;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Position -= Front * velocity;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Position -= Right * velocity;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Position += Right * velocity;
}

void Camera::processMouse(float xpos, float ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;


	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	updateCameraVectors();
}

void Camera::processScroll(float yoffset)
{
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

glm::vec3 Camera::getFront()
{
	return Front;
}

glm::vec3 Camera::getPostion()
{
	return Position;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(glm::radians(Zoom), (float)FrameInfo::ScreenWidth / (float)FrameInfo::ScreenHeight, 0.1f, 100.0f);
}


void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}