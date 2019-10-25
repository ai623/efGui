#pragma once
#include <wrl/client.h>
#include <D2D1.h>
#include <dwrite.h>

#include "../efCommon.h"

#define ComPtr Microsoft::WRL::ComPtr
#define ReCatch(re) efErrorCatcher.setHResult(re)
namespace efgui
{
	namespace efd2d
	{
		inline ComPtr<ID2D1Factory> createD2D1FactoryCom(D2D1_FACTORY_TYPE type) {
			ComPtr<ID2D1Factory> factory;
			HRESULT hr;
			hr = D2D1CreateFactory(type, IID_PPV_ARGS(&factory));
			ReCatch(hr);
			return factory;
		}

		
		inline ComPtr<IDWriteFactory> createDWriteFactoryCom() {
			ComPtr<IDWriteFactory> factory;
			HRESULT hr;
			hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),&factory);
			ReCatch(hr);
			return factory;
		}
	}
}
#undef ReCatch
#undef ComPtr