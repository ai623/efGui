#include "efMain.h"

namespace efgui {
	namespace _innerUsed {
		HINSTANCE ghInstance;
		PWSTR gpCmdLine;
		int gnCmdShow;
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) 
{
	efgui::_innerUsed::ghInstance = hInstance;
	efgui::_innerUsed::gpCmdLine = pCmdLine;
	efgui::_innerUsed::gnCmdShow = nCmdShow;

	auto re = efMain();
	return 0;
}