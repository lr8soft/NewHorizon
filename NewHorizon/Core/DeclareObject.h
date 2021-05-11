#pragma once
#ifndef _DECLARE_OBJECT_H_
#define _DECLARE_OBJECT_H_

#include <glm/glm.hpp>
#include <map>
#include <string>
struct DeclareObject {
	std::string assetName;
	std::string shaderName;
	std::string modelName;
	std::string scriptName;
	std::string typeName;
	std::string scriptNameSpace;

	std::map<std::string, float> lightData;
	std::map<std::string, glm::vec3> lightVectorData;
	//sizeof(string)=28 or 40
};

#endif