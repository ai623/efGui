#include <EfGui/platform/windows/efGui.h>

using namespace efgui;
struct MyWindow : EfWindow {
	~MyWindow() { _uninit(); }

	MyWindow(EfPainter& pt):EfWindow(pt){}

	virtual void whenDestroy() {
		_uninit();
	}

private:
	void _init() {}
	void _uninit() { if (getWindowsNum() == 1) { efExec.quit(); } }
};

int efMain() {

	efGuiInit.enableDebugMode();
	auto re = efGuiInit();

	EfPainter pt;
	MyWindow wnd(pt);

	efExec();
	return 0;
}