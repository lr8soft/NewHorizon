#pragma once
#ifndef _DECLARE_OBJECT_MANAGER_H_
#define _DECLARE_OBJECT_MANAGER_H_
#include "DeclareObject.h"
#include <map>
class DeclareObjectManager {
private:
	static DeclareObjectManager* pInstance;

	std::map<std::string, DeclareObject*> declareObjectGroup;
public:
	static DeclareObjectManager* getInstance();

	DeclareObject* LoadDeclareObject(const std::string& name, const std::string& path);
	DeclareObject* GetDeclareObject(const std::string& name);

};

#endif