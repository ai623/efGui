#include <cmath>


#include "efFontPainter.h"


#define ComPtr Microsoft::WRL::ComPtr

namespace efgui
{
	efd3d10_1::EfD3d10_1Painter EfFontPainter::mpainter = efd3d10_1::EfD3d10_1Painter(EfNoInit());
	ComPtr<ID2D1Factory> EfFontPainter::md2dFactory;
	ComPtr<IDWriteFactory> EfFontPainter::mdwriteFactory;
	ComPtr<IDWriteFontCollection> EfFontPainter::msysFontCollection;


	std::wstring EfDWriteFontFamily::getFamilyName() const
	{
#define Error_Check if(FAILED(hr)) return std::wstring();
		// GetSystemDefaultLangID();
		HRESULT hr;
		ComPtr<IDWriteLocalizedStrings> locStr;
		BOOL exists = FALSE;
		UINT32 index;
		UINT32 strLength;

		auto localeName = efsystem::getLocaleName();
		hr = mfamily->GetFamilyNames(&locStr); Error_Check
		locStr->FindLocaleName(localeName.c_str(), &index, &exists);
		if (!exists) {
			locStr->FindLocaleName(L"en-us", &index, &exists);
			if (!exists)index = 0; 
		}
		hr = locStr->GetStringLength(index, &strLength);Error_Check
		
		auto strRe = new wchar_t[strLength + 1];
		hr = locStr->GetString(index, strRe, strLength + 1);Error_Check
		std::wstring familyName(strRe);
		delete[] strRe;
		//delete[] strRe;
		return familyName;
#undef Error_Check
	}

	bool EfFontPainter::_init()
	{
		HRESULT hr;
		if (!mpainter.hasInit()) {
			if (mpainter.init() == false) {
				return false;
			}
		}
		if (md2dFactory.Get() == nullptr) {
			if(_innerUsed::gmultiThread)
				md2dFactory = efd2d::createD2D1FactoryCom(D2D1_FACTORY_TYPE_MULTI_THREADED);
			else {
				md2dFactory = efd2d::createD2D1FactoryCom(D2D1_FACTORY_TYPE_SINGLE_THREADED);
			}
			if (md2dFactory.Get() == nullptr)
				return false;
		}

		if (mdwriteFactory.Get() == nullptr) {
			mdwriteFactory = efd2d::createDWriteFactory();
			if (!mdwriteFactory) return false;
		}

		if (msysFontCollection.Get() == nullptr) {
			hr = mdwriteFactory->GetSystemFontCollection(&msysFontCollection, TRUE);
			if (FAILED(hr)) return false;
		}

		//
		//auto device = mpainter.getDevice();
		//ComPtr<ID3D10Texture2D> texture;
		//D3D10_TEXTURE2D_DESC desc;
		//D3D10_SUBRESOURCE_DATA data{};
		//
		//hr = device->CreateTexture2D(&desc, &data, &texture);
		//if (FAILED(hr)) {
		//	return false;
		//}

		return true;
	}

	void EfFontPainter::setTextLayout(const std::wstring& str)
	{
#define Check_Error if(FAILED(hr)) return;
		HRESULT hr;
		mtextLength = str.length();
		hr = mdwriteFactory->CreateTextLayout(str.c_str(), str.length(), mfontFormat.Get(), 8192, 8192, &mtextLayout); Check_Error;

		return;
#undef Check_Error
	}

	EfBuffer2D<EfArray<uint8_t, 4> > EfFontPainter::draw()
	{
#define Check_Error if(FAILED(hr)) return EfBuffer2D<EfArray<uint8_t, 4> > ()
		HRESULT hr;

		UINT textureWidth;
		UINT textureHeight;
		float dpi =(float) GetDpiForSystem();
		{
			DWRITE_TEXT_RANGE range;
			range.startPosition = 0;
			range.length = mtextLength;
			mtextLayout->SetFontSize(mfontSize, range);

			DWRITE_TEXT_METRICS metrics;
			mtextLayout->GetMetrics(&metrics);
			textureWidth = (UINT) std::ceil(metrics.width / 96 *dpi);
			textureHeight = (UINT)std::ceil(metrics.height / 96 *dpi);
		}

		ComPtr<ID3D10Texture2D> targetTexture;
		ComPtr<ID3D10Texture2D> stageTexture;
		auto device = mpainter.getDevice();
		{
			D3D10_TEXTURE2D_DESC desc;
			
			desc.Width = textureWidth;
			desc.Height = textureHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc = { 1,0 };
			desc.Usage = D3D10_USAGE_DEFAULT;
			desc.BindFlags = D3D10_BIND_RENDER_TARGET;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			hr = device->CreateTexture2D(&desc, nullptr, &targetTexture); Check_Error;
			
			desc.Usage = D3D10_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
			hr = device->CreateTexture2D(&desc, nullptr, &stageTexture); Check_Error;

			//clear buffer
			{
				ComPtr<ID3D10RenderTargetView> view;
				device->CreateRenderTargetView(targetTexture.Get(), nullptr, &view);
				FLOAT color[4];
				color[0] = 1 - mfontColor.r;
				color[1] = 1 - mfontColor.g;
				color[2] = 1 - mfontColor.b;
				color[3] = 0;
				device->ClearRenderTargetView(view.Get(), color);
			}
		}
		
		ComPtr<IDXGISurface> surface;
		hr = targetTexture.As(&surface); Check_Error;

		ComPtr<ID2D1RenderTarget> target;
		{
			D2D1_RENDER_TARGET_PROPERTIES props{};
			props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
			props.pixelFormat = { DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED };
			props.dpiX = dpi;
			props.dpiY = dpi;
			hr = md2dFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), props, &target); Check_Error;
		}

		ComPtr<ID2D1SolidColorBrush> brush;
		hr = target->CreateSolidColorBrush(mfontColor, &brush);
		//hr = target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);

		target->BeginDraw();
		target->DrawTextLayout(
			{ 0,0 },
			mtextLayout.Get(),
			brush.Get()
		);
		target->EndDraw();

		EfBuffer2D<EfArray<uint8_t, 4> >  re(textureHeight,textureWidth);

		{
			D3D10_MAPPED_TEXTURE2D mapText;
			UINT subResource = D3D10CalcSubresource(0, 0, 1);

			device->CopyResource(stageTexture.Get(), targetTexture.Get());
			hr = stageTexture->Map(
				subResource,
				D3D10_MAP_READ,
				0,
				&mapText
			); Check_Error;

			uint8_t* data = (uint8_t*)mapText.pData;
			auto rowSize = textureWidth * 4;
			auto dst = re.data;

			for (unsigned int i = 0; i < textureHeight; i++) {
				std::memcpy(dst, data, rowSize);
				data += mapText.RowPitch;
				dst += textureWidth;
			}
			stageTexture->Unmap(subResource);
		}
		return re;
#undef Check_Error
	}

	void EfFontPainter::setSysFormat(const std::wstring& name, float fontSize)
	{
		HRESULT hr;
		hr = mdwriteFactory->CreateTextFormat(
			name.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT_MEDIUM,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontSize,
			efsystem::getLocaleName().c_str(),
			&mfontFormat
		);
	}





}


#undef ComPtr