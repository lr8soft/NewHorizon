#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <glm/glm.hpp>
class Camera {
private:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -2.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
public:
	void processInput(float deltaTime);
	glm::mat4 getViewMatrix();
};

#endif