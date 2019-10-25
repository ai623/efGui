#include "efPainter.h"
#include "efWindow.h"


#define _RPN(p)		_EfGui_Release_Comptr_To_Null(p)
#define _REL(p)		_EfGui_Release_Comptr(p)
#define _CPP(x,p)	_EfGui_Copy_Comptr(x,p)
#define _MVP(x,p)	_EfGui_Move_Comptr(x,p)


namespace efgui 
{
	EfPainterInit efPainterInit;
	namespace _efPainter {
		const D3D_FEATURE_LEVEL glevelsWant[] = { D3D_FEATURE_LEVEL_11_0 };
		IDXGIFactory* gdxgiFactory = nullptr;
		bool gmultiThread = false;
		bool gdebugMode = false;
		UINT gsampleCount = 1;
	}

	EfResult EfPainterInit::operator()(bool debugMode, bool multiThread,UINT sampleCount)
	{
		using namespace _efPainter;
		HRESULT hr;
		hr = CreateDXGIFactory(IID_PPV_ARGS(&gdxgiFactory));
		if (FAILED(hr)) {
			_EfGui_Debug_Warning_Msg_Code("EfPainterInit: Fail to create IDXGIFactory", hr);
			return -1;
		}
		gdebugMode = debugMode;
		gmultiThread = multiThread;
		gsampleCount = sampleCount;
		
		return 0;
	}

	void EfPainterInit::uninit()
	{
		using _efPainter::gdxgiFactory;
		_EfGui_Release_Comptr_To_Null(gdxgiFactory);
	}

	bool EfAdapter::_init(UINT index)
	{
		using _efPainter::gdxgiFactory;
		HRESULT hr;
		hr = gdxgiFactory->EnumAdapters(index, &madapter);
		if (FAILED(hr)) {
			_EfGui_Debug_Warning_Msg_Code("EfPainterInit: Fail to create IDXGIAdapter", hr);
			return false;
		}
		return true;
	}

	void EfPainter::setRenderTarget(EfWindow& wnd)
	{
		auto view = wnd.getTargetView();  mcontext->OMSetRenderTargets(1, &view, nullptr); 
	}

	bool EfPainter::_init(IDXGIAdapter* adapter, bool debugMode)
	{
		HRESULT hr;
		
		UINT flags = 0;
		flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;			//for supporting d2d
		if (!_efPainter::gmultiThread) {
			flags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
		}

		if (debugMode) {
			flags |= D3D11_CREATE_DEVICE_DEBUG;
		}

		D3D_DRIVER_TYPE driverType;
		if (adapter == nullptr) {
			driverType = D3D_DRIVER_TYPE_HARDWARE;
		}
		else {
			driverType = D3D_DRIVER_TYPE_UNKNOWN;
		}

		hr = D3D11CreateDevice(
			adapter,														//显卡,nullptr使用默认显卡
			driverType,										//硬渲染
			NULL,															//软件模块，如果使用软件实现的话
			flags,	//是否debug和单线程
			_efPainter::glevelsWant,														//想要的d3d11版本，是个数组，将会依次遍历，如果支持则立即返回
			ARRAYSIZE(_efPainter::glevelsWant),											//想要的d3d11版本数量
			D3D11_SDK_VERSION,												//必须是该值
			&mdevice,														//
			&mlevel,														//得到的d3d11版本
			&mcontext														//
		);
		if (FAILED(hr)) {
			_EfGui_Debug_Warning_Msg_Code("EfPainter: _init(): Fail to call D3D11CreateDevice", hr);
			return false;
		}

		return true;
	}


	IDXGIDevice* EfPainter::createDXGIDevice() const
	{
		HRESULT hr;
		IDXGIDevice* dxgiDevice;
		hr = mdevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
		if (FAILED(hr))return nullptr;
		return dxgiDevice;
	}

	IDXGIAdapter* EfPainter::createDXGIAdpter() const
	{
		HRESULT hr;
		IDXGIDevice* dxgiDevice;
		IDXGIAdapter* adapter;

		hr = mdevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
		if (FAILED(hr)) { _EfGui_Debug_Warning_Msg_Code("EfPainter: Fail to createDXGIFactory", hr); _REL(dxgiDevice); return nullptr; }
		hr = dxgiDevice->GetParent(IID_PPV_ARGS(&adapter));
		if (FAILED(hr)) { _EfGui_Debug_Warning_Msg_Code("EfPainter: Fail to createDXGIFactory", hr); _REL(dxgiDevice); _REL(adapter); return nullptr; }
		return adapter;
	}

	IDXGIFactory* EfPainter::createDXGIFactory() const
	{
		HRESULT hr;
		IDXGIDevice* dxgiDevice;
		IDXGIAdapter* adapter;
		IDXGIFactory* factory;

		hr = mdevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
		if (FAILED(hr)) { _EfGui_Debug_Warning_Msg_Code("EfPainter: Fail to createDXGIFactory", hr); _REL(dxgiDevice); return nullptr; }
		hr = dxgiDevice->GetParent(IID_PPV_ARGS(&adapter));
		if (FAILED(hr)) { _EfGui_Debug_Warning_Msg_Code("EfPainter: Fail to createDXGIFactory", hr); _REL(dxgiDevice); _REL(adapter); return nullptr; }
		hr = adapter->GetParent(IID_PPV_ARGS(&factory));
		if (FAILED(hr)) { _EfGui_Debug_Warning_Msg_Code("EfPainter: Fail to createDXGIFactory", hr); _REL(dxgiDevice); _REL(adapter);  _REL(factory); return nullptr; }
		dxgiDevice->Release();
		adapter->Release();
		return factory;
	}

	ID3D11Texture2D* EfPainter::createDSTextureForWnd(const EfWindow& wnd) const
	{
		ID3D11Texture2D* dsBuff;
		HRESULT hr;
		D3D11_TEXTURE2D_DESC desc;
		auto backBuff = wnd.getBackBuffer();
		backBuff->GetDesc(&desc);

		desc.MipLevels = 1;
		desc.ArraySize = 1;

		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		hr = mdevice->CreateTexture2D(&desc, nullptr, &dsBuff);
		if (FAILED(hr))_EfGui_Debug_Warning_Msg_Code("EfPainter: Fail to createDSTextureForWnd", hr);
		return dsBuff;
	}



	bool EfAdapterOutput::_init(IDXGIAdapter* apt, UINT index)
	{
		HRESULT hr;
		hr = apt->EnumOutputs(index, &moutput);
		if (FAILED(hr)) { _EfGui_Debug_Warning_Msg_Code("EfAdapterOutput: Fail to init", hr); return false; }
		return true;
	}


}


#undef	_RPN
#undef	_REL
#undef	_CPP
#undef	_MVP