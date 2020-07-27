#pragma once
#ifndef _HTTP_CONNECT_H_
#define _HTTP_CONNECT_H_
#include <iostream>
#include <sstream>
class HttpConnect {
private:
	std::stringstream DBG;
public:
	HttpConnect();
	~HttpConnect();

	void socketHttp(std::string host, std::string request);

	void postData(std::string host, std::string path, std::string post_content);
	void getData(std::string host, std::string path, std::string get_content);

	std::stringstream& getDebugInfo();
};


#endif