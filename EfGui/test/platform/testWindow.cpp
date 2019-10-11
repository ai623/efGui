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
	auto re = efGuiInit();
	
	MyWindow wnd;
	EfPainter pt;

	efExec();
	return 0;
}