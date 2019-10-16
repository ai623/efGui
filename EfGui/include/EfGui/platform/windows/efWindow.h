#pragma once
#include <EfGui/common/efCommon.h>
#include <EfGui/common/efShape.h>

#include <stdint.h>

#define UNICODE
#include <Windows.h>

#include "efCommon.h"
#include "efPainter.h"
#include "efPainterManager.h"
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

	struct EfWindow : EfTexture2D, EfRenderTarget
	{
		//destructor
		virtual ~EfWindow() { _del(); }
		//constructor
		EfWindow(EfNoInit){}
		EfWindow(EfPainter& pt) { _init(pt); }
		EfWindow(){ _init(); }
		//init
		bool init() { _del(); return _init(); }
		bool init(EfPainter& pt) { _del(); return _init(pt); }
		void uninit() { _uninit(); }
		//checker
		bool isFullScreen() const;
		//getter
		EfRect<long> getRect() const;
		EfPainter& getPainter() { return mpainter; }
		int getWindowsNum() const;
		EfPoint<long> getMousePosition() const;
		//setter
		//void setPainter(const EfPainter& pt) { mpainter = pt; }		//Problem Here!!!
		//others
		void updateWindow()const { UpdateWindow(mhWnd); }
		void updateWindowLater() const { InvalidateRect(mhWnd, NULL, FALSE); }
		void present() { if(mswapChain) mswapChain->Present(0,0); }
		//callback
		virtual void whenCreate() {}			//Caution: when construct, the son's virtual methods not initialized!
		virtual void whenDestroy() {}			//Caution: when destruct virtual methods have been destroyed!
		virtual void whenPaint() {}
		virtual void whenSizeChange(EfRect<long>& rect) {}
		virtual void whenMouseMove(EfPoint<long>& pos) { }
		virtual void whenMouseWheel(WPARAM distance) {}
		virtual void whenKeyDown(WPARAM key) {}
		virtual void whenKeyUp(WPARAM key){}
		virtual void whenChar(WPARAM code) {}
		virtual void whenIME(WPARAM notify, LPARAM command){}
		//virtual EfWindow* clone();			//TODO
	private:
		HWND mhWnd = NULL;
		IDXGISwapChain* mswapChain = nullptr;
		ID3D11Texture2D* mbackBuffer = nullptr;
		ID3D11RenderTargetView* mtargetView = nullptr;
		EfPainter mpainter = EfPainter(EfNoInit());

		bool _init(bool fullScreen = false) { EfPainter pt; return _init(pt, fullScreen, _efPainter::gsampleCount); }
		bool _init(EfPainter& painter, bool fullScreen = false) { return _init(painter, fullScreen, _efPainter::gsampleCount); }
		bool _init(EfPainter& painter, bool fullScreen, int sampleCount);
		void _uninit() { _del(); _reset(); }
		void _del();
		void _reset() { mhWnd = NULL; mswapChain = nullptr; mbackBuffer = nullptr; mtargetView = nullptr; }
		void _copy(const EfWindow&) = delete;
		void _move(EfWindow&&) = delete;

		bool _initD3dComponents(bool fullScreen, int sampleCount);
		bool _initD3dComponents(DXGI_SWAP_CHAIN_DESC& desc);
	public:
		HWND getHWnd() const { return mhWnd; }
		IDXGISwapChain* getSwapChain() const { return mswapChain; }
		ID3D11Texture2D* getBackBuffer()const { return mbackBuffer; }
		ID3D11RenderTargetView* getTargetView() const { return mtargetView; }
		virtual void getTextureDesc(D3D11_TEXTURE2D_DESC& desc) const { if (mbackBuffer) { mbackBuffer->GetDesc(&desc); } }
		bool getSwapChainDesc(DXGI_SWAP_CHAIN_DESC& desc)const { if (mswapChain) { mswapChain->GetDesc(&desc); }return false; }

		friend LRESULT _efWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
	
	inline void efUpdateAllWindow() {
		InvalidateRect(NULL, NULL, FALSE);
	}
}