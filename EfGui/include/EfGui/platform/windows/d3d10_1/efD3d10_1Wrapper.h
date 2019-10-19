#pragma once
#include <d3d10_1.h>
#include <wrl/client.h>

#include "../efCommon.h"

#define ComPtr Microsoft::WRL::ComPtr
#define ReCatch(re) efErrorCatcher.setHResult(re)


namespace efgui
{
	namespace efd3d10_1
	{

	}
}

#undef ReCatch
#undef ComPtr