#include "efFontPainter.h"


#define ComPtr Microsoft::WRL::ComPtr

namespace efgui
{
	efd3d10_1::EfD3d10_1Painter EfFontPainter::mPainter = efd3d10_1::EfD3d10_1Painter(EfNoInit());
	ComPtr<ID2D1Factory> EfFontPainter::md2dFactory;
	ComPtr<IDWriteFactory> EfFontPainter::mwriteFactory;
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
		if (!mPainter.hasInit()) {
			if (mPainter.init() == false) {
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

		if (mwriteFactory.Get() == nullptr) {
			mwriteFactory = efd2d::createDWriteFactory();
			if (!mwriteFactory) return false;
		}

		if (msysFontCollection.Get() == nullptr) {
			hr = mwriteFactory->GetSystemFontCollection(&msysFontCollection, TRUE);
			if (FAILED(hr)) return false;
		}

		//
		//auto device = mPainter.getDevice();
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

}


#undef ComPtr