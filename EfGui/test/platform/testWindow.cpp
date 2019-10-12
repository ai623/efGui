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
	int count = 0;

	MyWindow wnd;


	efExec();
	return 0;
}