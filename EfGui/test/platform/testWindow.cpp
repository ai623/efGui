#include <string>
#include <cassert>
#include <wrl/client.h>
#include <EfGui/platform/windows/efGui.h>
#include <EfGui/platform/windows/EfDSBuffCreator.h>

#define ComPtr Microsoft::WRL::ComPtr

using namespace efgui;

struct MyWindow : EfWindow, EfLoop 
{
	~MyWindow() { _uninit(); }

	MyWindow() { _init(); }
	MyWindow(EfPainter& pt) :EfWindow(pt) { _init(); }

	virtual void whenDestroy() {
		_uninit();
	}
	
	virtual void whenMouseMove(EfPoint<long>& pos) {
		auto rect = getRect();
		mmousePos.x = pos.x;
		mmousePos.y = pos.y;
		whenPaint();
	}

	virtual void whenPaint() {
		auto& pt = getPainter();
		auto context = pt.getContext();
		auto rect = getRect();
		mclearColor[0] = (float)mmousePos.x / rect.width;
		mclearColor[1] = (float)mmousePos.y / rect.height;

		pt.clearTarget(*this, mclearColor);
		context->ClearDepthStencilView(mdsView.Get(), D3D11_CLEAR_DEPTH, 1., 0);
		std::wstring str = L"Hello,World!";

		present();
	}

private:
	EfPoint<long> mmousePos;
	std::array<float, 4> mclearColor{ 0,0,0,1 };
	D3D11_VIEWPORT mvp;
	EfDSBuffCreator mdsBuffCreator;
	ComPtr<ID3D11DepthStencilView> mdsView;

	void _init() 
	{
		auto& painter = getPainter();
		auto context = painter.getContext();
		auto rect = getRect();
		mvp = {0,0,(float)rect.width,(float)rect.height,0.,1.};
		auto targetView = getTargetView();
		context->RSSetViewports(1, &mvp);
		mdsView.Attach(mdsBuffCreator.create(getPainter(), *this));
		context->OMSetRenderTargets(1, &targetView, mdsView.Get());
	}
	void _uninit() { if (getWindowsNum() == 1) { efExec.quit(); } }
};



int efMain() {

	efGuiInit.enableDebugMode();
	auto re = efGuiInit();

	//EfPainter pt;
	MyWindow wnd;

	efExec();
	return 0;
}

#undef ComPtr