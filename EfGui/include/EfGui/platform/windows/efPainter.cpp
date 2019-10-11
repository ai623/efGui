#include "efPainter.h"

namespace efgui 
{
	EfPainterInit efPainterInit;
	namespace _efPainter {
		const D3D_FEATURE_LEVEL glevelsWant[] = { D3D_FEATURE_LEVEL_11_0 };
		IDXGIFactory* gdxgiFactory = nullptr;
		bool gmultiThread = false;
		bool gdebugMode = false;
	}

	EfResult EfPainterInit::operator()(bool debugMode, bool multiThread)
	{
		HRESULT hr;
		hr = CreateDXGIFactory(IID_PPV_ARGS(&_efPainter::gdxgiFactory));
		if (FAILED(hr)) {
			_EfGui_Debug_Warning_Msg_Code("EfPainterInit: Fail to create IDXGIFactory", hr);
			return -1;
		}
		return 0;
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

	bool EfPainter::_init(IDXGIAdapter* adapter, bool debugMode)
	{
		HRESULT hr;
		_del();

		UINT flags{};
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

}
