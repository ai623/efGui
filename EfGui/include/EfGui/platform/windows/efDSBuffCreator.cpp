#include "efDSBuffCreator.h"

#define ComPtr Microsoft::WRL::ComPtr
namespace efgui
{
	ID3D11DepthStencilView* EfDSBuffCreator::create(const EfPainter& pt, const EfTexture2D& text) const
	{
		HRESULT hr;
		D3D11_TEXTURE2D_DESC desc;
		auto device = pt.getDevice();
	
		//using Microsoft::WRL::ComPtr;
		ComPtr<ID3D11Texture2D> dsBuff;
		ID3D11DepthStencilView* dsView;

		text.getTextureDesc(desc);
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		hr = device->CreateTexture2D(&desc, nullptr, &dsBuff);
		if (FAILED(hr)) { return nullptr; }//TODO
		hr = device->CreateDepthStencilView(dsBuff.Get(), nullptr, &dsView);
		//TODO

	
		return dsView;
	}

}
#undef ComPtr