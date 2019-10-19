#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#include "efCommon.h"

#define ComPtr Microsoft::WRL::ComPtr
#define ReCatch(re) efErrorCatcher.setHResult(re)

namespace efgui
{
	namespace efd3d
	{
		inline IDXGISurface* createDXGISurface(ID3D11Resource* buff)
		{
			HRESULT hr;
			IDXGISurface* surface;
			hr = buff->QueryInterface(IID_PPV_ARGS(&surface));
			if (FAILED(hr))return nullptr;
			return surface;
		}
	}
}

#undef ReCatch
#undef ComPtr