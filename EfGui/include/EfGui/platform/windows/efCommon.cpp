#include <Windows.h>
#include "efCommon.h"


namespace efgui 
{
	namespace _innerUsed 
	{

	}
	std::wstring getCurrentPath()
	{
		{
			std::wstring path;
			auto size = GetCurrentDirectoryW(0, nullptr);
			auto buff = new wchar_t[size + 1];
			GetCurrentDirectoryW(size, buff);
			buff[size] = L'\0';
			path = buff;
			delete buff;
			return path;
		}
	}
}
