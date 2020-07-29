#pragma once
#ifndef _HTTP_CONNECT_H_
#define _HTTP_CONNECT_H_

#include <iostream>
class HttpConnect {
private:

	static size_t writeData(void * buffer, size_t size, size_t membercount, void * userp);
public:
	static bool haveGlobalInit;
	static void releaseGlobalEnov();

	std::string readFromUrl(const std::string url);

	HttpConnect();

	
};

#endif