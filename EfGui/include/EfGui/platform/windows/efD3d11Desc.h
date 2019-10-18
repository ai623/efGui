#pragma once
#pragma once
#include <wrl/client.h>
#include <d3d11.h>
//#include <d3dcompiler.h>

namespace efgui
{
	namespace efd3d11
	{
		namespace efdesc
		{
			extern const D3D11_BUFFER_DESC gefVertexDesc;
			extern const D3D11_BUFFER_DESC gefIndexDesc;
			extern const D3D11_BUFFER_DESC gefConstantDesc;

			extern const D3D11_INPUT_ELEMENT_DESC gefElePOSITIONf2[];
			extern const D3D11_SAMPLER_DESC gefSamplerDesc;
		}
	}
}