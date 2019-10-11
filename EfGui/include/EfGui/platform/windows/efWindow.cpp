#include "efWindow.h"
#include "efMain.h"

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
namespace efgui {
	namespace _innerUsed 
	{
		const wchar_t gwcName[] = L"BaseWindow";
		int gwndNum = 0;
	}

	EfWindowInit efWindowInit;

	int EfWindow::getWindowsNum() const
	{
		return _innerUsed::gwndNum;
	}

	bool EfWindow::_init()
	{
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
		if (mhWnd) {
			return false;
		}
		return true;
	}

	EfResult EfWindowInit::operator()()
	{
		WNDCLASSW wc{};
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = _innerUsed::ghInstance;
		wc.lpszClassName = _innerUsed::gwcName;

		if (!RegisterClassW(&wc)) {
			return -1;
		}
		return 0;
	}

}


LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	using efgui::EfWindow;
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
		//wnd->mhWnd = hWnd;
		efgui::_innerUsed::gwndNum++;
		wnd->whenCreate();
		return 0;
	}
	case WM_DESTROY:
	{
		wnd->whenDestroy();
		//wnd->releaseAll();
		wnd->setHWndNull();
		efgui::_innerUsed::gwndNum--;
		return 0;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}