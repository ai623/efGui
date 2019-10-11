#pragma once
#include <d3d11.h>


namespace efgui 
{
	struct EfPainter 
	{
		ID3D11Device* mdevice = nullptr;
		ID3D11DeviceContext* mcontext = nullptr;
	};
}