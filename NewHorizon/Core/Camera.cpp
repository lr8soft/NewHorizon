#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../HorizonFrame.h"
#include "../Util/LogUtil.hpp"
void Camera::processInput(float deltaTime)
{
	float cameraSpeed = 2.5f * deltaTime;
	GLFWwindow* window = HorizonFrame::getInstance()->getScreen();
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		cameraSpeed *= 2;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
		
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	
	LogUtil::printInfo("x:"+ std::to_string(cameraPos.x) + " y:"+ std::to_string(cameraPos.y) + " z:" + std::to_string(cameraPos.z));
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
