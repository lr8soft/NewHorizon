#include "ShaderManager.h"
#include <GL3/gl3w.h>
#include "../Util/ShaderReader.h"

ShaderManager* ShaderManager::pInstance = nullptr;
ShaderManager::ShaderManager(){}
GLuint ShaderManager::bindProgram(const std::string& folderName, const std::string & programName)
{
	auto programRealName = folderName + "." + programName;
	GLuint programHandle = 0;
	if (shaderGroup.find(programRealName) != shaderGroup.end())//Find shader
	{
		programHandle = shaderGroup[programRealName];
		glUseProgram(programHandle);
	}
	else {//try init shader
		xc_ogl::ShaderReader shader;
		auto programPath = folderName  + "/" + programName;
		shader.loadFromFile(("assets/Shader/" + programPath + ".vert").c_str(), GL_VERTEX_SHADER);
		shader.loadFromFile(("assets/Shader/" + programPath + ".frag").c_str(), GL_FRAGMENT_SHADER);
		shader.linkAllShader();

		programHandle = shader.getProgramHandle();
		shaderGroup.insert(std::make_pair(programRealName, programHandle));
		glUseProgram(programHandle);
	}
	return programHandle;
}

bool ShaderManager::deleteProgram(const std::string& folderName, const std::string & programName)
{
	std::string programRealName = folderName + "." + programName;
	auto targetIter = shaderGroup.find(programRealName);
	if (targetIter != shaderGroup.end())
	{
		glDeleteProgram(targetIter->second);
		shaderGroup.erase(targetIter);
		return true;
	}
	return false;
}

ShaderManager * ShaderManager::getInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new ShaderManager;
	}
	return pInstance;
}

ShaderManager::~ShaderManager()
{
	for (auto iter = shaderGroup.begin(); iter != shaderGroup.end(); ++iter)
	{
		glDeleteProgram(iter->second);
	}
	shaderGroup.clear();
}