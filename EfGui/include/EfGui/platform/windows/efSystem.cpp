#include "efSystem.h"

namespace efgui
{
	namespace efsystem
	{
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

		std::wstring getLocaleName()
		{
			// GetSystemDefaultLangID();
			wchar_t localeName[LOCALE_NAME_MAX_LENGTH];
			
			GetSystemDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);
			std::wstring re(localeName);
			return re;
		}
	}
}