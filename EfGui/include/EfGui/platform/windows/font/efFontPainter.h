#pragma once
#include <wrl/client.h>
#include <D2D1.h>

#include "../efSystem.h"
#include "../d3d10_1/efD3d10_1Painter.h"
#include "../d2d/efD2dWrapper.h"
#include "../efD3dWrapper.h"

#define ComPtr Microsoft::WRL::ComPtr

namespace efgui
{
	struct EfDWriteFontFamily
	{
		IDWriteFontFamily* getRawPtr() const { return mfamily.Get(); }
		std::wstring getFamilyName() const;
		bool hasInit() { return mfamily.Get(); }

	private:
		ComPtr<IDWriteFontFamily> mfamily;
		friend struct EfFontPainter;
	};


	struct EfFontPainter
	{
		EfFontPainter() { _init(); }

		void setFontSize(unsigned int size) { mfontSize = size; }

		UINT32 getSysFontNum() const;
		EfDWriteFontFamily getSysFontFamily(UINT32 index) const;
		EfDWriteFontFamily getSysFontFamily(const std::wstring& familyName) const;
		bool getSysFontFamilyIndex(const std::wstring& familyName, UINT32& index) const;

	private:
		unsigned int mfontSize = 0;

		static efd3d10_1::EfD3d10_1Painter mPainter;
		static ComPtr<ID2D1Factory> md2dFactory;
		static ComPtr<IDWriteFactory> mwriteFactory;
		static ComPtr<IDWriteFontCollection> msysFontCollection;

		bool _init();
	};









	inline UINT32 EfFontPainter::getSysFontNum() const
	{
		return msysFontCollection->GetFontFamilyCount();
	}

	inline EfDWriteFontFamily EfFontPainter::getSysFontFamily(UINT32 index) const
	{
		//HRESULT hr;
		EfDWriteFontFamily fontFamily;
		msysFontCollection->GetFontFamily(index, &fontFamily.mfamily);
		return fontFamily;
	}
	inline EfDWriteFontFamily EfFontPainter::getSysFontFamily(const std::wstring& familyName)const
	{
		BOOL exists;
		UINT32 index;
		msysFontCollection->FindFamilyName(familyName.c_str(), &index, &exists);
		if (exists) {
			EfDWriteFontFamily fontFamily;
			msysFontCollection->GetFontFamily(index, &fontFamily.mfamily);
			return fontFamily;
		}
		return EfDWriteFontFamily();
	}

	inline bool EfFontPainter::getSysFontFamilyIndex(const std::wstring& familyName, UINT32& index)const
	{
		BOOL exists;
		msysFontCollection->FindFamilyName(familyName.c_str(), &index, &exists);
		return exists;
	}
}

#undef ComPtr