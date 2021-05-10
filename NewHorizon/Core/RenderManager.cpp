#include "RenderManager.h"
#include "ShaderHelper.h"
#include "EngineManager.h"
#include "ModelManager.h"

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

void RenderManager::onRender(DeclareObject* classObject)
{
	GLuint shader = ShaderHelper::getInstance()->bindProgram("object", classObject->shaderName);

	Camera* camera = EngineManager::getInstance()->getCamera();

	glm::vec3 viewPos = camera->getPostion();

	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(*modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(camera->getProjectionMatrix()));

	//light info
	//glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
	//glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
	//glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
	glUniform1f(glGetUniformLocation(shader, "material.shininess"), 32);

	glUniform3fv(glGetUniformLocation(shader, "light.direction"), 1, glm::value_ptr(glm::vec3(-3, -3, -3)));
	//glUniform3fv(glGetUniformLocation(shader, "light.position"), 1, glm::value_ptr(lightPos));
	glUniform3fv(glGetUniformLocation(shader, "light.ambient"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
	glUniform3fv(glGetUniformLocation(shader, "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
	glUniform3fv(glGetUniformLocation(shader, "light.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	//glUniform3fv(glGetUniformLocation(shader, "ambientColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	//glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(glm::vec3(3, 3, 3)));//a lightpot in (2, 2, 2)
	//glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));//

	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(viewPos));

	ModelManager::getInstance()->RenderModel(classObject->modelName, shader);
}
