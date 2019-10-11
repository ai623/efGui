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
			adapter,														//�Կ�,nullptrʹ��Ĭ���Կ�
			driverType,										//Ӳ��Ⱦ
			NULL,															//���ģ�飬���ʹ�����ʵ�ֵĻ�
			flags,	//�Ƿ�debug�͵��߳�
			_efPainter::glevelsWant,														//��Ҫ��d3d11�汾���Ǹ����飬�������α��������֧������������
			ARRAYSIZE(_efPainter::glevelsWant),											//��Ҫ��d3d11�汾����
			D3D11_SDK_VERSION,												//�����Ǹ�ֵ
			&mdevice,														//
			&mlevel,														//�õ���d3d11�汾
			&mcontext														//
		);
		if (FAILED(hr)) {
			_EfGui_Debug_Warning_Msg_Code("EfPainter: _init(): Fail to call D3D11CreateDevice", hr);
			return false;
		}

		return true;
	}

}
