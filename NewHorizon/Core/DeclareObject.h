#pragma once
#ifndef _DECLARE_OBJECT_H_
#define _DECLARE_OBJECT_H_
#include <string>
struct DeclareObject {
	std::string assetName;
	std::string shaderName;
	std::string modelName;
	std::string scriptName;
	std::string typeName;
	std::string scriptNameSpace;
	//sizeof(string)=28 or 40
};

#endif