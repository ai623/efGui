#pragma once
#include <string>
#include <EfGui/common/efShape.h>

#include <Windows.h>

namespace efgui 
{
	namespace efsystem
	{
		std::wstring getCurrentPath();
		std::wstring getLocaleName();

		inline EfRect<int> getSysResolution() {
			return EfRect<int>{GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
		}
	}

}