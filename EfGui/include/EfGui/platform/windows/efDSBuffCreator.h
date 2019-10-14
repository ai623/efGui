#pragma once
#include <wrl/client.h>
#include "efPainterManager.h"


namespace efgui
{
	struct EfDSBuffCreator
	{
		virtual ID3D11DepthStencilView* create(const EfPainter& pt, const EfTexture2D& text) const;
	};
}

