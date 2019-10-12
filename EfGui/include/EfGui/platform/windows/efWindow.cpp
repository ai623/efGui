#include "efWindow.h"
#include "efMain.h"
#include "efSystem.h"


namespace efgui {

	namespace _efWindow 
	{
		const wchar_t gwcName[] = L"BaseWindow";
		int gwndNum = 0;

		LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			//using efgui::EfWindow;
			//using efgui::_efWindow::gwndNum;
			EfWindow* wnd = (EfWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			switch (msg)
			{
			case WM_PAINT:
			{
				wnd->whenPaint();
				break;
			}
			case WM_CREATE:
			{
				wnd = (EfWindow*)(((CREATESTRUCT*)lParam)->lpCreateParams);
				SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)wnd);
				wnd->mhWnd = hWnd;
				gwndNum++;
				wnd->whenCreate();
				return 0;
			}
			case WM_DESTROY:
			{
				wnd->whenDestroy();
				//wnd->releaseAll();
				wnd->mhWnd = NULL;
				gwndNum--;
				return 0;
			}
			default:
				break;
			}

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	EfWindowInit efWindowInit;

	int EfWindow::getWindowsNum() const
	{
		return _efWindow::gwndNum;
	}

	bool EfWindow::_init(EfPainter& painter, bool fullScreen, int sampleCount)
	{
		using namespace _efWindow;
		using namespace _innerUsed;
		mhWnd = CreateWindowExW(
			0,                              // Optional window styles.
			gwcName,                     // Window class
			L"",    // Window text
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,            // Window style

			// Size and position
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

			NULL,       // Parent window    
			NULL,       // Menu
			ghInstance,  // Instance handle
			this        // Additional application data
		);
		if (!mhWnd) {
			return false;
		}
		mpainter = painter;
		if (_initD3dComponents(fullScreen, sampleCount)) {
			InvalidateRect(mhWnd, NULL, FALSE);	//post WM_PAINT message
			return true;
		}
		else {
			mpainter.uninit();
			return false;
		}
	}

	void EfWindow::_del()
	{
		if (mhWnd) { DestroyWindow(mhWnd); } 
		if (mswapChain) {
			DXGI_SWAP_CHAIN_DESC desc;
			mswapChain->GetDesc(&desc);
			if (desc.Windowed == FALSE) {
				mswapChain->SetFullscreenState(FALSE, NULL);
			}
			mswapChain->Release();
		}
		_REL(mbackBuffer);
		_REL(mtargetView);
		mpainter.uninit();
	}

	bool EfWindow::_initD3dComponents(bool fullScreen, int sampleCount)
	{
		HRESULT hr;

		DXGI_SWAP_CHAIN_DESC scDesc;
		auto& bfDesc = scDesc.BufferDesc;
		if (fullScreen) {
			auto rect = efGetSysResolution();
			bfDesc.Width = rect.width;
			bfDesc.Height = rect.height;
			scDesc.Windowed = FALSE;
		}
		else {
			bfDesc.Width = 0;
			bfDesc.Height = 0;
			scDesc.Windowed = TRUE;
		}
		bfDesc.RefreshRate = DXGI_RATIONAL{60,1};
		bfDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bfDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bfDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scDesc.SampleDesc.Count = sampleCount;
		if (sampleCount != 1) {
			UINT qualityLevleNum;
			hr = mpainter.getDevice()->CheckMultisampleQualityLevels(bfDesc.Format, sampleCount, &qualityLevleNum);
			if (FAILED(hr)) {
				_EfGui_Debug_Warning_Msg_Code("EfWindow: Fail to get multi-sample quality.", hr);
				scDesc.SampleDesc.Quality = 0;
			}
			else {
				scDesc.SampleDesc.Quality = qualityLevleNum - 1;
			}
		}
		else {
			scDesc.SampleDesc.Quality = 0;
		}
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.BufferCount = 1;
		scDesc.OutputWindow = mhWnd;
		scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scDesc.Flags = 0;

		return _initD3dComponents(scDesc);
	}

	bool EfWindow::_initD3dComponents(DXGI_SWAP_CHAIN_DESC& desc)
	{
		HRESULT hr;
		auto factory = mpainter.createIDXGIFactory();
		if (factory == nullptr) {
			return false;
		}
		hr = factory->CreateSwapChain(mpainter.getDevice(), &desc, &mswapChain);
		if (FAILED(hr)) { _EfGui_Debug_Warning_Msg_Code("EfWindow: Fail to create SwapChain", hr); factory->Release(); return false; }
		hr = mswapChain->GetBuffer(0, IID_PPV_ARGS(&mbackBuffer));
		if (FAILED(hr)) {_EfGui_Debug_Warning_Msg_Code("EfWindow: Fail to get back buffer.", hr); factory->Release(); mswapChain->Release(); return false;}
		hr = mpainter.getDevice()->CreateRenderTargetView(mbackBuffer, nullptr, &mtargetView);
		if (FAILED(hr)) { _EfGui_Debug_Warning_Msg_Code("EfWindow: Fail to create render target view.", hr); factory->Release(); mswapChain->Release(); mbackBuffer->Release(); return false; }
		factory->Release();
		return true;
	}

	EfResult EfWindowInit::operator()()
	{
		using efgui::_efWindow::WndProc;
		WNDCLASSW wc{};
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = _innerUsed::ghInstance;
		wc.lpszClassName = _efWindow::gwcName;

		if (!RegisterClassW(&wc)) {
			_EfGui_Debug_Warning_Msg("Fail to Initialized");
			return -1;
		}
		return 0;
	}

}

