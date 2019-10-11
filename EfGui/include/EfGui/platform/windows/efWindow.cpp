#include "efWindow.h"
#include "efMain.h"

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
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

	bool EfWindow::_init()
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
		if (mhWnd) {
			return false;
		}
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

