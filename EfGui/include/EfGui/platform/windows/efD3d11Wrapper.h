#pragma once
#include <string>
#include <type_traits>
#include <stdint.h>

#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "efCommon.h"


#define ComPtr Microsoft::WRL::ComPtr
#define wstring std::wstring
#define ReCatch(re) efErrorCatcher.setHResult(re)

namespace efgui
{
	namespace efd3d11
	{
		inline ID3DBlob* readFileToBlob(const wstring& path) {
			HRESULT hr;
			ID3DBlob* blob;
			hr = D3DReadFileToBlob(path.c_str(), &blob); ReCatch(hr);
			return blob;
		}

		template<typename T>
		inline constexpr DXGI_FORMAT getDxgiFormat(unsigned int num) {
			using namespace std;
			static_assert(is_same<T, float>, "Invalid Type");
			if (is_same<T, float>) {
				switch (num)
				{
				case 1:
					return DXGI_FORMAT_R32_FLOAT;
				case 2:
					return DXGI_FORMAT_R32G32_FLOAT;
				case 3:
					return DXGI_FORMAT_R32G32B32_FLOAT;
				case 4:
					return DXGI_FORMAT_R32G32B32A32_FLOAT;
				default:
					return DXGI_FORMAT_UNKNOWN;
				}
			}
		}


	}
}

#undef ReCatch
#undef wstring
#undef ComPtr