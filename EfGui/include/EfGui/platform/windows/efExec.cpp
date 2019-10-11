#include "efExec.h"

#include <Windows.h>

namespace efgui 
{
	int EfExec::operator()()
	{
		MSG msg = { };
		while (msg.message != WM_QUIT)
		{
			if (_isEnableDefaultRun) {
				// If there are Window messages then process them.
				if (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessageW(&msg);
				}
				else
				{
					if (defaultRun) {
						defaultRun();
					}
				}
			}
			else {
				GetMessageW(&msg, NULL, 0, 0);
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
		return (int)msg.wParam;
	}

	void efgui::EfExec::quit()
	{
		PostQuitMessage(0);
	}

	EfExec efExec;
}
