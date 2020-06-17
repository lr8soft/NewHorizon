#pragma once
#ifndef _SHADER_MANAGER_
#define _SHADER_MANAGER_
#include <map>
#include <GL/glcorearb.h>
class ShaderHelper {
private:
	static ShaderHelper* pInstance;

	std::map<std::string, GLuint> shaderGroup;

	ShaderHelper();
public:
	~ShaderHelper();

	//Example: bindProgram("object", "test") --> assets\Shader\object\test.vert, DO NOT DELETE SHADER MANUALLY
	GLuint bindProgram(const std::string& folderName, const std::string& programName);
	//Example: deleteProgram("object", "test") --> remove the instance of assets\Shader\object\test.vert
	bool deleteProgram(const std::string& folderName, const std::string& programName);

	static ShaderHelper * getInstance();

};

#endif