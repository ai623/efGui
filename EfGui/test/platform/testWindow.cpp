#include <string>
#include <cassert>

#include <EfGui/platform/windows/efGui.h>

using namespace efgui;

struct MyPtControl : EfPainterControl
{
	~MyPtControl() { _del(); }
	virtual bool init(const EfPainter& pt, EfRenderTarget& target);
	virtual bool initPartial(const EfPainter& pt);
	virtual bool initPartial(EfRenderTarget& target);
	virtual void uninit() { _del(); _reset(); }
	virtual bool get(EfPainterControlDesc& desc);
	virtual bool backup(EfPainterControl& control);
	virtual bool set(EfPainter& painter, EfRenderTarget& target);

private:
	struct Vertex {

	};
	ID3D11InputLayout* minputLayout = nullptr;
	ID3D11Buffer* mctBuff = nullptr;
	ID3D11Buffer* midBuff = nullptr;
	ID3D11Buffer* mvtBuff = nullptr;
	ID3D11VertexShader* mvtShader = nullptr;
	ID3D11PixelShader* mpxShader = nullptr;
	ID3D11DepthStencilView* mdsView = nullptr;

	void _del() {
#define R(p) if(p) p ->Release();
		R(minputLayout)R(mctBuff)R(midBuff)R(mvtBuff)R(mvtShader)R(mpxShader)R(mdsView)
#undef R
	}
	void _reset() {
#define N(p) p=nullptr;
		N(minputLayout)N(mctBuff)N(midBuff)N(mvtBuff)N(mvtShader)N(mpxShader)N(mdsView)
#undef N
	}
};


struct MyWindow : EfWindow {
	~MyWindow() { _uninit(); }

	MyWindow(EfPainter& pt) :EfWindow(pt) { _init(); }

	virtual void whenDestroy() {
		_uninit();
	}

private:
	MyPtControl ptCtrl;

	void _init() 
	{
		ptCtrl.init(getPainter(),*this);
	}
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


//ID3D11InputLayout* minputLayout = nullptr;
//ID3D11Buffer* mctBuff = nullptr;
//ID3D11Buffer* midBuff = nullptr;
//ID3D11Buffer* mvtBuff = nullptr;
//ID3D11VertexShader* mvtShader = nullptr;
//ID3D11PixelShader* mpxShader = nullptr;
//ID3D11DepthStencilView* dsView = nullptr;
bool MyPtControl::init(const EfPainter& pt, EfRenderTarget& target)
{
	using std::string;
	using std::wstring;

	HRESULT hr;
	wstring dirPath = LR"(D:\Bill\project\visual_studio\mylib\EfGui\x64\Debug)";
	auto device = pt.getDevice();
#define ASS(hr)  assert(SUCCEEDED(hr))
	//init buffers
	{
		//init context buffer
		{
			D3D11_BUFFER_DESC desc;

		}
		//init index buffer
		{
			D3D11_BUFFER_DESC desc;
			
		}
		//init vertex buffer
		{
			D3D11_BUFFER_DESC desc;

		}

	}

	//init shaders
	{
		ID3DBlob* vs,* ps;
		wstring vsPath = dirPath + L"\\VertexShader.cso";
		wstring psPath = dirPath + L"\\PixelShader.cso";
		hr = D3DReadFileToBlob(vsPath.c_str(), &vs);ASS(hr);
		hr = D3DReadFileToBlob(psPath.c_str(), &ps);ASS(hr);

		//init layout
		D3D11_INPUT_ELEMENT_DESC desc[]{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 };
		hr = device->CreateInputLayout(desc, 1, vs->GetBufferPointer(), vs->GetBufferSize(), &minputLayout); ASS(hr);
		//init vertex shader
		hr = device->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), nullptr, &mvtShader); ASS(hr);
		//init pixel shader
		hr = device->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), nullptr, &mpxShader); ASS(hr);

		vs->Release();
		ps->Release();
	}

	//init depth stencil buffer view
	{
		ID3D11Texture2D* dsBuff;
		D3D11_TEXTURE2D_DESC desc;
		target.getTextureDesc(desc);
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		hr = device->CreateTexture2D(&desc, nullptr, &dsBuff); ASS(hr);
		hr = device->CreateDepthStencilView(dsBuff, nullptr, &mdsView); ASS(hr);

		dsBuff->Release();
	}


#undef ASS

	return true;
}

bool MyPtControl::initPartial(const EfPainter& pt)
{
	return false;
}

bool MyPtControl::initPartial(EfRenderTarget& target)
{
	return false;
}



bool MyPtControl::get(EfPainterControlDesc& desc)
{
	return false;
}

bool MyPtControl::backup(EfPainterControl& control)
{
	return false;
}

bool MyPtControl::set(EfPainter& painter, EfRenderTarget& target)
{
	auto context = painter.getContext();
	ID3D11RenderTargetView* tgViews[]{ target.getTargetView() };
	context->OMSetRenderTargets(1, tgViews, mdsView);
	return true;
}



