#pragma once
#include <wrl/client.h>
#include <D2D1.h>

#include "../efCommon.h"

#define ComPtr Microsoft::WRL::ComPtr
#define ReCatch(re) efErrorCatcher.setHResult(re)
namespace efgui
{
	namespace efd2d
	{
		struct EfD2dPainter
		{

		private:
			
		};
	}
}
#undef ReCatch
#undef ComPtr