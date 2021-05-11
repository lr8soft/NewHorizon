#include <vector>
#include <string>

#include "Skybox.h"

#include "EngineManager.h"
#include "GameObject.h"
#include "ShaderHelper.h"

#include "../Util/ImageLoader.h"

#include <GL3/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Skybox::Skybox(GameObject *object): gameObject(object)
{
	DeclareObject* classObject = object->getClassObject();

	std::vector<std::string> cubeInfo;
	cubeInfo.push_back(classObject->declareStrData["right"]);
	cubeInfo.push_back(classObject->declareStrData["left"]);
	cubeInfo.push_back(classObject->declareStrData["top"]);
	cubeInfo.push_back(classObject->declareStrData["bottom"]);
	cubeInfo.push_back(classObject->declareStrData["front"]);
	cubeInfo.push_back(classObject->declareStrData["back"]);

	skyboxMap = xc_ogl::ImageLoader::loadCubeMap(cubeInfo);

	GLuint vbo;

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &vbo);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	glBindVertexArray(0);
}

Skybox::~Skybox()
{
	//free texture
}

GameObject * Skybox::getGameObject()
{
	return gameObject;
}

void Skybox::onRenderSkyBox()
{
	glDepthMask(GL_FALSE);

	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxMap);
	GLuint shader = ShaderHelper::getInstance()->bindProgram("object", gameObject->getClassObject()->shaderName);

	Camera* camera = EngineManager::getInstance()->getCamera();

	glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera->getViewMatrix()));

	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(camera->getProjectionMatrix()));

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
}

unsigned int Skybox::getSkyboxMapHandle()
{
	return skyboxMap;
}
