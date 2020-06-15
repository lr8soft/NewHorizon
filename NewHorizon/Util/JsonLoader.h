#pragma once
#ifndef _JSON_LOADER_
#define _JSON_LOADER_
#include <memory>
#include "../ThirdParty/json/json.h"
class JsonLoader {
private:
	static std::string* loadInfoFromFile(const std::string& path);
public:
	static std::shared_ptr<Json::Value> getJsonFromFile(const std::string& path);
};

#endif