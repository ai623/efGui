#pragma once
#include <EfGui/common/efCommon.h>

#include <stdint.h>

#define UNICODE
#include <Windows.h>

#include "efCommon.h"
#include "efPainter.h"

#define _REL(p) _EfGui_Release_Comptr(p)

namespace efgui 
{
	namespace _efWindow
	{

		LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	}

	extern struct EfWindowInit efWindowInit;
	//public type 
	//destructor
	//constructor
	//checker
	//getter
	//setter
	//other methods

	//private:
	//inner type
	//private data
	//private methods
	//friend struct
	//friend func

	//common
	//...

	inline bool EfSuccess(EfResult re) { return re == 0; }
	inline bool EfFail(EfResult re) { return re; }

	struct EfWindowInit 
	{
		EfResult operator() ();
	};

	struct EfWindow 
	{
		//destructor
		virtual ~EfWindow() { _del(); }
		//constructor
		EfWindow(EfNoInit){}
		EfWindow(){ _init(); }
		//init
		bool init() { _del(); return _init(); }
		void uninit() { _uninit(); }
		//checker
		//getter
		EfPainter& getPainter() { return mpainter; }
		int getWindowsNum() const;
		//callback
		virtual void whenCreate() {}			//Caution: when construct, the son's virtual methods not initialized!
		virtual void whenDestroy() {}			//Caution: when destruct virtual methods have been destroyed!
		virtual void whenPaint() {}
		//virtual EfWindow* clone();			//TODO
	private:
		HWND mhWnd = NULL;
		IDXGISwapChain* mswapChain = nullptr;
		ID3D11Texture2D* mbackBuffer = nullptr;
		ID3D11RenderTargetView* mtargetView = nullptr;
		EfPainter mpainter = EfPainter(EfNoInit());

		bool _init() { EfPainter pt; return _init(pt, false, _efPainter::gsampleCount); }
		bool _init(EfPainter& painter, bool fullScreen, int sampleCount);
		void _uninit() { _del(); _reset(); }
		void _del() { if (mhWnd) { DestroyWindow(mhWnd); } _REL(mswapChain); _REL(mbackBuffer); _REL(mtargetView); mpainter.uninit();}
		void _reset() { mhWnd = NULL; mswapChain = nullptr; mbackBuffer = nullptr; mtargetView = nullptr; }
		void _copy(const EfWindow&) = delete;
		void _move(EfWindow&&) = delete;

		bool _initD3dComponents(bool fullScreen, int sampleCount);
		bool _initD3dComponents(DXGI_SWAP_CHAIN_DESC& desc);
	public:
		HWND getHWnd() const { return mhWnd; }

		friend LRESULT _efWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}