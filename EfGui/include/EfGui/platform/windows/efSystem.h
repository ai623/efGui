#pragma once
#include <EfGui/common/efShape.h>

#include <Windows.h>

namespace efgui 
{
	inline EfRect<int> efGetSysResolution() {
		return EfRect<int>{GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
	}
}