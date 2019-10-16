#pragma once
#include <string>

#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "efDebug.h"


#define ComPtr Microsoft::WRL::ComPtr
#define wstring std::wstring
#define ReCatch(re) efErrorCatcher.setHResult(re)
namespace efgui
{
	namespace efd3d11
	{
		namespace efwrapper
		{
			inline ID3DBlob* readFileToBlob(const wstring& path) {
				ID3DBlob* blob;
				ReCatch(D3DReadFileToBlob(path.c_str(), &blob));
				return blob;
			}
		}


	}
}

#undef ReCatch
#undef wstring
#undef ComPtr