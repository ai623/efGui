#include "efD3d10_1Painter.h"

namespace efgui
{
	namespace efd3d10_1
	{
		bool EfD3d10_1Painter::_init(IDXGIAdapter* adapter, bool multiThread, bool debug)
		{
			HRESULT hr;
			D3D10_DRIVER_TYPE driverType;
			UINT flag = D3D10_CREATE_DEVICE_BGRA_SUPPORT;

			if (adapter) {
				driverType = D3D10_DRIVER_TYPE_NULL;
			}
			else {
				driverType = D3D10_DRIVER_TYPE_HARDWARE;
			}

			if (!multiThread) {
				flag |= D3D10_CREATE_DEVICE_SINGLETHREADED;
			}
			if (debug) {
				flag |= D3D10_CREATE_DEVICE_DEBUG;
			}


			hr = D3D10CreateDevice1(
				adapter,
				driverType,
				NULL,
				flag,
				D3D10_FEATURE_LEVEL_10_1,
				D3D10_1_SDK_VERSION,
				&mdevice
			);
			if (FAILED(hr)) return false;
			return true;
		}
	}
}