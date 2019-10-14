#pragma once
#include <wrl/client.h>
#include <EfGui/common/efShape.h>
#include "efPainterManager.h"


#define ComPtr Microsoft::WRL::ComPtr

namespace efgui
{
	struct EfPMPolygon
	{
		virtual bool init(const EfPainter& pt, const D3D11_VIEWPORT& vp, bool shared = false) { return _init(pt, vp, shared); }
		virtual void setData(EfRect<float>& rect);
	private:
		EfPainter mpainter;
		ComPtr<ID3D11Buffer> mcBuff;
		ComPtr<ID3D11Buffer> mvBuff;
		ComPtr<ID3D11Buffer> miBuff;
		ComPtr<ID3D11VertexShader> mvShader;
		ComPtr<ID3D11PixelShader> mpShader;

		bool mShared;

		bool _init(const EfPainter& pt, const D3D11_VIEWPORT& vp, bool shared = false);
	};
}

#undef ComPtr