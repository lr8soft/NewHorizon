#pragma once
#ifndef _I_COMPONENT_H_
#define _I_COMPONENT_H_
#include <any>
#include <map>
#include <string>
class  IComponent {
protected:
	std::map<std::string, std::any> infoMap;
public:
	void setData(std::string key, std::any data)
	{
		infoMap.insert(std::make_pair(key, data));
	}

	std::any getData(std::string key)
	{
		return infoMap.find(key)->second;
	}

	virtual void onComponenAssetsInit() {};
	virtual void onComponenRenderInit(){}
	virtual void onComponentUpdate() {};
	virtual void onComponentRender() {};
	virtual void onComponentRelease() {};
};

#endif