#include <EfGui/platform/windows/efGui.h>

using namespace efgui;
struct MyWindow : EfWindow {
	~MyWindow() { _uninit(); }
	void whenDestroy() {
		_uninit();
	}

private:
	void _init() {}
	void _uninit() { if (getWindowsNum() == 1) { efExec.quit(); } }
};

int efMain() {

	efGuiInit.enableDebugMode();
	auto re = efGuiInit();
	
	MyWindow wnd;

	EfPainter pt1;
	EfPainter pt2;
	for(int i =0; i<1000;i++)
	{
		pt1 = pt2;
		pt2 = pt2;
		pt2 = pt1;
	}


	efExec();
	return 0;
}