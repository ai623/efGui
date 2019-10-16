#include <string>
#include <cassert>
#include <wrl/client.h>
#include <DirectXMath.h>

#include <EfGui/platform/windows/efGui.h>

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

		context->ClearRenderTargetView(getTargetView(), mclearColor);
		context->ClearDepthStencilView(mdsView.Get(), D3D11_CLEAR_DEPTH, 1., 0);

		present();
	}

	virtual void whenChar(WPARAM code) {

	}
	virtual void whenIME(WPARAM notify, LPARAM command)
	{
		if (0&&notify == WM_IME_STARTCOMPOSITION) {
			auto imm = ImmGetContext(getHWnd());
			COMPOSITIONFORM  form;
			form.dwStyle = CFS_POINT;
			form.ptCurrentPos.x = 0;
			form.ptCurrentPos.y = 0;
			ImmSetCompositionWindow(imm,&form);
			ImmReleaseContext(getHWnd(), imm);
		}

	}
private:
	EfPoint<long> mmousePos;
	float mclearColor[4]{ 0,0,0,1 };
	D3D11_VIEWPORT mvp;

	ComPtr<ID3D11Texture2D> mdsBuff;
	ComPtr<ID3D11DepthStencilView> mdsView;

	void _init() 
	{
		auto& painter = getPainter();
		auto context = painter.getContext();
		auto device = painter.getDevice();
		auto rect = getRect();
		auto targetView = getTargetView();

		mvp = {0,0,(float)rect.width,(float)rect.height,0.,1.};
		context->RSSetViewports(1, &mvp);

		mdsBuff.Attach(painter.createDSTextureForWnd(*this));
		device->CreateDepthStencilView(mdsBuff.Get(), nullptr, &mdsView);

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