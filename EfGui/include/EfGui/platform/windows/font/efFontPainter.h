#pragma once
#include <memory>
#include <vector>
#include <tuple>

#include <wrl/client.h>
#include <D2D1.h>

#include <EfGui/common/efdataType.h>
#include "../efSystem.h"
#include "../d3d10_1/efD3d10_1Painter.h"
#include "../d2d/efD2dWrapper.h"
#include "../efD3dWrapper.h"

#define ComPtr Microsoft::WRL::ComPtr

namespace efgui
{
	struct EfDWriteFontFamily
	{
		IDWriteFontFamily* getDWriteFontFamily() const { return mfamily.Get(); }
		std::wstring getFamilyName() const;
		bool hasInit() { return mfamily.Get(); }

	private:
		ComPtr<IDWriteFontFamily> mfamily;
		friend struct EfFontPainter;
	};


	struct EfFontPainter
	{
		EfFontPainter() { _init(); }

		void setFormat(ComPtr<IDWriteTextFormat> format) { mfontFormat = format; }
		void setSysFormat(const std::wstring& name, float fontSize);
		void setColor(float color[4]);
		void setFontSize(float size) { mfontSize = size; }
	
		UINT32 getSysFontNum() const;
		EfDWriteFontFamily getSysFontFamily(UINT32 index) const;
		EfDWriteFontFamily getSysFontFamily(const std::wstring& familyName) const;
		bool getSysFontFamilyIndex(const std::wstring& familyName, UINT32& index) const;
		IDWriteTextLayout* getTextLayout() const { return mtextLayout.Get(); }
		void getTextLayoutWidthHeight(float& width, float& height) const;

		void setTextLayout(const std::wstring& str);
		EfBuffer2D<EfArray<uint8_t,4> > draw();

	private:
		D2D1_COLOR_F mfontColor{ 0,0,0,1 };
		float mfontSize = 20;
		unsigned int mtextLength = 0;

		ComPtr<IDWriteTextFormat> mfontFormat;
		ComPtr<IDWriteTextLayout> mtextLayout;

		static efd3d10_1::EfD3d10_1Painter mpainter;
		static ComPtr<ID2D1Factory> md2dFactory;
		static ComPtr<IDWriteFactory> mdwriteFactory;
		static ComPtr<IDWriteFontCollection> msysFontCollection;

		bool _init();
	};




	inline void EfFontPainter::setColor(float color[4])
	{
		mfontColor.r = color[0];
		mfontColor.g = color[1];
		mfontColor.b = color[2];
		mfontColor.a = color[3];
	}


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

	inline void EfFontPainter::getTextLayoutWidthHeight(float& width, float& height) const
	{
		DWRITE_TEXT_METRICS metrics;
		mtextLayout->GetMetrics(&metrics);
		width = metrics.width;
		height = metrics.height;
	}

}

#undef ComPtr