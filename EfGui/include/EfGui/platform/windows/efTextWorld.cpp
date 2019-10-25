#include "efTextWorld.h"
#include "d2d/efD2dWrapper.h"

#define string std::string
#define wstring std::wstring
#define ComPtr Microsoft::WRL::ComPtr

namespace efgui
{

	bool EfTextWorld::setSysFontFormat(const wstring& name)
	{
		HRESULT hr;
		ComPtr<IDWriteTextFormat> format;
		hr = mdwriteFactory->CreateTextFormat
		(
			name.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT_MEDIUM,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			mfontSize,
			efsystem::getLocaleName().c_str(),
			&mtextFormat
		);
		if (FAILED(hr)) { return false; }
		mtextFormat = format;

		return true;
	}

	bool EfTextWorld::_init()
	{
		HRESULT hr;
		EfDWriteFontCollection sysFontCollection;
		auto family = sysFontCollection.getFontFamily(0);
		if (!family.hasInit()) return false;
		wstring familyName = family.getFamilyName();
		mdwriteFactory = efd2d::createDWriteFactoryCom();
		if (mdwriteFactory.Get() == nullptr) return false;
		hr = mdwriteFactory->CreateTextFormat
		(
			familyName.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT_MEDIUM,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			mfontSize,
			efsystem::getLocaleName().c_str(), 
			&mtextFormat
		);
		if (FAILED(hr)) { return false; }
		return true;
	}
}


