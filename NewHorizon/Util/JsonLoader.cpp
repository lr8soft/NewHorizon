#include "JsonLoader.h"
#include <string>
#include<fstream>
#include<sstream>
#include <iostream>
std::string* JsonLoader::loadInfoFromFile(const std::string & path)
{
	std::ifstream io(path);
	if (!io) {
		std::cerr << "[ERROR]Fail to load json " << path << std::endl;
	}
	else {
		std::string* str = new std::string;

		std::string tempInfo;
		bool is_first = true;
		while (std::getline(io, tempInfo)) {
			if (is_first)
				is_first = false;
			else
				(*str) += '\n';
			(*str) += tempInfo;
		}
		(*str) += '\0';
		io.close();

		return str;
	}
	return nullptr;
}
std::shared_ptr<Json::Value> JsonLoader::getJsonFromFile(const std::string & path)
{

	std::shared_ptr<Json::Value> pJson(new Json::Value);

	std::ifstream io(path);
	if (io.is_open())
	{
		JSONCPP_STRING errs;
		Json::CharReaderBuilder rbuilder;
		bool parse_ok = Json::parseFromStream(rbuilder, io, pJson.get(), &errs);
		if (!parse_ok)
		{
			std::cerr << "[ERROR]Fail to parse json file " << path << std::endl;
		}
	}
	else {
		std::cerr << "[ERROR]Fail to load json " << path << std::endl;
	}
	io.close();
	return pJson;
}
