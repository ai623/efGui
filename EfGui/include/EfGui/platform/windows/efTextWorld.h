#pragma once
#include <string>

#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>

#include "efCamera.h"
#include "efPainter.h"
#include "d3d10_1/efD3d10_1Painter.h"
#include "font/efFontPainter.h"

#define string std::string
#define wstring std::wstring
#define ComPtr Microsoft::WRL::ComPtr

namespace efgui
{

	struct EfTextWorld
	{
		EfDpiCamera2D camera;

		EfTextWorld() { _init(); }

		void setPainter(EfPainter& pt) { mpainter = pt; }

		bool setSysFontFormat(const wstring& format);
		void setFontSize(float size) { mfontSize = size; }
		void setText(const wstring& text) { mtext = text; }
		void setText(wstring&& text) { mtext = text; }
		//void setLineWrapMode(bool enable);

	private:
		wstring mtext;
		float mfontSize = 16;

		EfPainter mpainter = EfPainter(EfNoInit());

		ComPtr<IDWriteFactory> mdwriteFactory;
		ComPtr<IDWriteTextFormat> mtextFormat;
		ComPtr<IDWriteTextLayout> mtextLayout;

		bool _init();
	};
}


namespace efgui
{
}

#undef string
#undef wstring
#undef ComPtr