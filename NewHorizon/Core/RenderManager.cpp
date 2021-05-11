#include "RenderManager.h"
#include "ShaderHelper.h"
#include "EngineManager.h"
#include "ModelManager.h"
#include "EngineDefine.h"

#include "GameObject.h"

#include "../Util/LogUtil.hpp"

#include <GL3/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

RenderManager* RenderManager::pInstance = nullptr;



RenderManager * RenderManager::getInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new RenderManager;
	}
	return pInstance;
}

void RenderManager::setModelMatrix(glm::mat4* matrix)
{
	modelMatrix = matrix;
}
void RenderManager::onRender(GameObject* gameObject)
{
	DeclareObject* classObject = gameObject->getClassObject();

	//update light info
	if (classObject->typeName == DIRECTIONAL_LIGHT_TYPE_NAME && findObject(directionalLightGroup, gameObject))
	{
		directionalLightGroup.push_back(gameObject);

	}
	else if (classObject->typeName == POINT_LIGHT_TYPE_NAME && findObject(pointLightGroup, gameObject))
	{
		pointLightGroup.push_back(gameObject);
	}
	else if (classObject->typeName == FLASH_LIGHT_TYPE_NAME && findObject(flashLightGroup, gameObject))
	{
		flashLightGroup.push_back(gameObject);
	}

	GLuint shader = ShaderHelper::getInstance()->bindProgram("object", classObject->shaderName);

	Camera* camera = EngineManager::getInstance()->getCamera();

	glm::vec3 viewPos = camera->getPostion();

	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(viewPos));

	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(*modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(camera->getProjectionMatrix()));


	glUniform1f(glGetUniformLocation(shader, "material.shininess"), gameObject->getClassObject()->lightData["shininess"]);


	sendLightInfo(shader);

	ModelManager::getInstance()->RenderModel(classObject->modelName, shader);
}

void RenderManager::sendLightInfo(unsigned int shader)
{
	glm::vec3 lightInfo;
	lightInfo.x = directionalLightGroup.size();
	lightInfo.y = pointLightGroup.size();
	lightInfo.z = flashLightGroup.size();
	glUniform3fv(glGetUniformLocation(shader, "lightCount"), 1, glm::value_ptr(lightInfo));

	char shaderParamter[32];
	int pointLightIndex = 0;

	for (GameObject* pointLight : pointLightGroup)
	{
		sprintf_s(shaderParamter, 32, "pointLights[%d].position", pointLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(pointLight->getTransform().position));

		std::string info = shaderParamter;
		
		DeclareObject* classObject = pointLight->getClassObject();

		sprintf_s(shaderParamter, 32, "pointLights[%d].ambient", pointLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(classObject->lightVectorData["ambient"]));

		sprintf_s(shaderParamter, 32, "pointLights[%d].diffuse", pointLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(classObject->lightVectorData["diffuse"]));

		sprintf_s(shaderParamter, 32, "pointLights[%d].specular", pointLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(classObject->lightVectorData["specular"]));

		sprintf_s(shaderParamter, 32, "pointLights[%d].constant", pointLightIndex);
		glUniform1f(glGetUniformLocation(shader, shaderParamter), classObject->lightData["constant"]);

		sprintf_s(shaderParamter, 32, "pointLights[%d].linear", pointLightIndex);
		glUniform1f(glGetUniformLocation(shader, shaderParamter), classObject->lightData["linear"]);

		sprintf_s(shaderParamter, 32, "pointLights[%d].quadratic", pointLightIndex);
		glUniform1f(glGetUniformLocation(shader, shaderParamter), classObject->lightData["quadratic"]);

		pointLightIndex++;
	}

	int dirLightIndex = 0;
	for (GameObject* dirLight : directionalLightGroup)
	{

		DeclareObject* classObject = dirLight->getClassObject();

		sprintf_s(shaderParamter, 32, "dirLight[%d].direction", dirLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(classObject->lightVectorData["direction"]));

		sprintf_s(shaderParamter, 32, "dirLight[%d].ambient", dirLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(classObject->lightVectorData["ambient"]));

		sprintf_s(shaderParamter, 32, "dirLight[%d].diffuse", dirLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(classObject->lightVectorData["diffuse"]));

		sprintf_s(shaderParamter, 32, "dirLight[%d].specular", dirLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(classObject->lightVectorData["specular"]));

		dirLightIndex++;
	}


	int spotLightIndex = 0;
	for (GameObject* flashLight : flashLightGroup)
	{
		sprintf_s(shaderParamter, 32, "spotLight[%d].position", spotLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(flashLight->getTransform().position));


		DeclareObject* classObject = flashLight->getClassObject();

		sprintf_s(shaderParamter, 32, "spotLight[%d].direction", spotLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(classObject->lightVectorData["direction"]));

		sprintf_s(shaderParamter, 32, "spotLight[%d].cutOff", spotLightIndex);
		glUniform1f(glGetUniformLocation(shader, shaderParamter), classObject->lightData["cutOff"]);

		sprintf_s(shaderParamter, 32, "spotLight[%d].outerCutOff", spotLightIndex);
		glUniform1f(glGetUniformLocation(shader, shaderParamter), classObject->lightData["outerCutOff"]);

		sprintf_s(shaderParamter, 32, "spotLight[%d].constant", spotLightIndex);
		glUniform1f(glGetUniformLocation(shader, shaderParamter), classObject->lightData["constant"]);

		sprintf_s(shaderParamter, 32, "spotLight[%d].linear", spotLightIndex);
		glUniform1f(glGetUniformLocation(shader, shaderParamter), classObject->lightData["linear"]);

		sprintf_s(shaderParamter, 32, "spotLight[%d].quadratic", spotLightIndex);
		glUniform1f(glGetUniformLocation(shader, shaderParamter), classObject->lightData["quadratic"]);

		sprintf_s(shaderParamter, 32, "spotLight[%d].ambient", spotLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(classObject->lightVectorData["ambient"]));

		sprintf_s(shaderParamter, 32, "spotLight[%d].diffuse", spotLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(classObject->lightVectorData["diffuse"]));

		sprintf_s(shaderParamter, 32, "spotLight[%d].specular", spotLightIndex);
		glUniform3fv(glGetUniformLocation(shader, shaderParamter), 1, glm::value_ptr(classObject->lightVectorData["specular"]));

		spotLightIndex++;
	}
}

void RenderManager::onFinishRender()
{
	modelMatrix = nullptr;
	directionalLightGroup.clear();
	pointLightGroup.clear();
	flashLightGroup.clear();
}
