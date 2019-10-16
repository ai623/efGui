#include "efDebug.h"
#include <Windows.h>

namespace efgui {
	EfErrorCatcher efErrorCatcher;

	void EfDebug::operator()(std::string str)
	{
		MessageBoxA(NULL, str.c_str(), "Message", MB_OK);
	}

	void EfDebug::error(std::string str)
	{
		MessageBoxA(NULL, str.c_str(), NULL, MB_OK);
		exit(-1);
	}

	void EfDebug::error(std::string str, uint64_t errorCode)
	{
		MessageBoxA(NULL, (str + ", Error code: " + std::to_string(errorCode)).c_str(), NULL, MB_OK);
		exit(-1);
	}

	void efDebugMsg(const std::string& msg)
	{
		MessageBoxA(NULL, msg.c_str(), "Message", MB_OK);
	}
	void efDebugError(const std::string& msg)
	{
		MessageBoxA(NULL, msg.c_str(), NULL, MB_OK);
		exit(-1);
	}
	void efDebugError(const std::string& msg, uint64_t errorCode)
	{
		MessageBoxA(NULL, (msg + ", Error code: " + std::to_string(errorCode)).c_str(), NULL, MB_OK);
		exit(-1);
	}
}
