#include "RenderManager.h"
#include "ShaderHelper.h"
#include "EngineManager.h"
#include "ModelManager.h"
#include "EngineDefine.h"

#include "GameObject.h"
#include "Skybox.h"

#include "../Util/LogUtil.hpp"
#include "../FrameInfo.h"

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

void RenderManager::applyRenderSettings()
{
	glEnable(GL_DEPTH_TEST);

	onInitShadow();
}


void RenderManager::onInitShadow()
{
	glGenFramebuffers(1, &depthMapFBO);
	//generate depth map fbo
	
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, FrameInfo::ScreenWidth, FrameInfo::ScreenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//set default color when render out of the border

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}




void RenderManager::setModelMatrix(glm::mat4* matrix)
{
	modelMatrix = matrix;
}


void RenderManager::onStartRender()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear screen

	if (skyboxObject != nullptr)
	{
		skyboxObject->onRenderSkyBox();
	}


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
	else if (classObject->typeName == SKYBOX_TYPE_NAME && !skyboxObject)	//skybox won't render now
	{
		skyboxObject = new Skybox(gameObject);
		return;
	}

	renderDepthGraph(gameObject);

	GLuint shader = ShaderHelper::getInstance()->bindProgram("object", classObject->shaderName);

	Camera* camera = EngineManager::getInstance()->getCamera();

	glm::vec3 viewPos = camera->getPostion();

	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(viewPos));

	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(*modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(camera->getProjectionMatrix()));


	glUniform1f(glGetUniformLocation(shader, "material.shininess"), gameObject->getClassObject()->lightFloatData["shininess"]);

	if (skyboxObject)	//set skybox to all objects
	{
		glUniform1f(glGetUniformLocation(shader, "skybox"), skyboxObject->getSkyboxMapHandle());
	}

	sendLightInfo(shader);
	sendShadowInfo(shader, gameObject);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader, "material.shadow_map"), 0);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	ModelManager::getInstance()->RenderModel(classObject->modelName, shader, true, 1);	//true, 1
}

void RenderManager::renderDepthGraph(GameObject* gameObject)
{
	DeclareObject *gameObjectClassObject = gameObject->getClassObject();

	Transform transform = gameObject->getTransform();
	
	GLuint shader = ShaderHelper::getInstance()->bindProgram("object", "depth");
	GLfloat near_panel = 1.0f, far_panel = 100.0f;

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_panel, far_panel);
	glCullFace(GL_FRONT);	//cull front face when render depth texture
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	for (GameObject* dirLight : directionalLightGroup)
	{
		DeclareObject* lightClassObject = dirLight->getClassObject();

		glm::vec3 Front = transform.position + lightClassObject->lightVectorData["direction"] * 2.5f;

		glm::mat4 lightView = glm::lookAt(transform.position - lightClassObject->lightVectorData["direction"] * 2.5f , Front, glm::vec3(0.0, 1.0, 0.0)); // glm::lookAt(dirLight->getTransform().position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(*modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader, "lightSpaceMatrix"), 1, false, glm::value_ptr(lightSpaceMatrix));

		ModelManager::getInstance()->RenderModel(gameObjectClassObject->modelName, shader, false);
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}


void RenderManager::sendShadowInfo(unsigned int shader, GameObject* gameObject)
{
	int dirLightIndex = 0;

	char segmentName[32];

	GLfloat near_panel = 1.0f, far_panel = 100.0f;

	Transform transform = gameObject->getTransform();

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_panel, far_panel);

	for (GameObject* dirLight : directionalLightGroup)
	{
		DeclareObject* classObject = dirLight->getClassObject();
		
		glm::vec3 Front = transform.position + classObject->lightVectorData["direction"] * 2.5f;

		glm::mat4 lightView = glm::lookAt(transform.position - classObject->lightVectorData["direction"] * 2.5f, Front, glm::vec3(0.0, 1.0, 0.0));//glm::lookAt(dirLight->getTransform().position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		sprintf_s(segmentName, 32, "dirShadowMat[%d]", dirLightIndex++);
		glUniformMatrix4fv(glGetUniformLocation(shader, segmentName), 1, false, glm::value_ptr(lightSpaceMatrix));
	}

}

void RenderManager::sendLightInfo(unsigned int shader)
{
	glm::vec3 lightInfo;
	lightInfo.x = directionalLightGroup.size();
	lightInfo.y = pointLightGroup.size();
	lightInfo.z = flashLightGroup.size();
	glUniform3fv(glGetUniformLocation(shader, "lightCount"), 1, glm::value_ptr(lightInfo));

	char segmentName[32];
	int pointLightIndex = 0;

	for (GameObject* pointLight : pointLightGroup)
	{
		sprintf_s(segmentName, 32, "pointLights[%d].position", pointLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(pointLight->getTransform().position));

		std::string info = segmentName;
		
		DeclareObject* classObject = pointLight->getClassObject();

		sprintf_s(segmentName, 32, "pointLights[%d].ambient", pointLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(classObject->lightVectorData["ambient"]));

		sprintf_s(segmentName, 32, "pointLights[%d].diffuse", pointLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(classObject->lightVectorData["diffuse"]));

		sprintf_s(segmentName, 32, "pointLights[%d].specular", pointLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(classObject->lightVectorData["specular"]));

		sprintf_s(segmentName, 32, "pointLights[%d].constant", pointLightIndex);
		glUniform1f(glGetUniformLocation(shader, segmentName), classObject->lightFloatData["constant"]);

		sprintf_s(segmentName, 32, "pointLights[%d].linear", pointLightIndex);
		glUniform1f(glGetUniformLocation(shader, segmentName), classObject->lightFloatData["linear"]);

		sprintf_s(segmentName, 32, "pointLights[%d].quadratic", pointLightIndex);
		glUniform1f(glGetUniformLocation(shader, segmentName), classObject->lightFloatData["quadratic"]);

		pointLightIndex++;
	}

	int dirLightIndex = 0;
	for (GameObject* dirLight : directionalLightGroup)
	{

		DeclareObject* classObject = dirLight->getClassObject();

		sprintf_s(segmentName, 32, "dirLight[%d].direction", dirLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(classObject->lightVectorData["direction"]));

		sprintf_s(segmentName, 32, "dirLight[%d].ambient", dirLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(classObject->lightVectorData["ambient"]));

		sprintf_s(segmentName, 32, "dirLight[%d].diffuse", dirLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(classObject->lightVectorData["diffuse"]));

		sprintf_s(segmentName, 32, "dirLight[%d].specular", dirLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(classObject->lightVectorData["specular"]));

		dirLightIndex++;
	}


	int spotLightIndex = 0;
	for (GameObject* flashLight : flashLightGroup)
	{
		sprintf_s(segmentName, 32, "spotLight[%d].position", spotLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(flashLight->getTransform().position));


		DeclareObject* classObject = flashLight->getClassObject();

		sprintf_s(segmentName, 32, "spotLight[%d].direction", spotLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(classObject->lightVectorData["direction"]));

		sprintf_s(segmentName, 32, "spotLight[%d].cutOff", spotLightIndex);
		glUniform1f(glGetUniformLocation(shader, segmentName), classObject->lightFloatData["cutOff"]);

		sprintf_s(segmentName, 32, "spotLight[%d].outerCutOff", spotLightIndex);
		glUniform1f(glGetUniformLocation(shader, segmentName), classObject->lightFloatData["outerCutOff"]);

		sprintf_s(segmentName, 32, "spotLight[%d].constant", spotLightIndex);
		glUniform1f(glGetUniformLocation(shader, segmentName), classObject->lightFloatData["constant"]);

		sprintf_s(segmentName, 32, "spotLight[%d].linear", spotLightIndex);
		glUniform1f(glGetUniformLocation(shader, segmentName), classObject->lightFloatData["linear"]);

		sprintf_s(segmentName, 32, "spotLight[%d].quadratic", spotLightIndex);
		glUniform1f(glGetUniformLocation(shader, segmentName), classObject->lightFloatData["quadratic"]);

		sprintf_s(segmentName, 32, "spotLight[%d].ambient", spotLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(classObject->lightVectorData["ambient"]));

		sprintf_s(segmentName, 32, "spotLight[%d].diffuse", spotLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(classObject->lightVectorData["diffuse"]));

		sprintf_s(segmentName, 32, "spotLight[%d].specular", spotLightIndex);
		glUniform3fv(glGetUniformLocation(shader, segmentName), 1, glm::value_ptr(classObject->lightVectorData["specular"]));

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
