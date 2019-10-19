#pragma once

#include <d3d10_1.h>
#include <wrl/client.h>

#include "../efCommon.h"

#define ComPtr Microsoft::WRL::ComPtr
#define ReCatch(re) efErrorCatcher.setHResult(re)


namespace efgui
{
	namespace efd3d10_1
	{
		struct EfD3d10_1Painter
		{
			EfD3d10_1Painter(EfNoInit) {}
			EfD3d10_1Painter() {init(); }

			//EfD3d10_1Painter(const EfD3d10_1Painter&) = default;
			

			bool init() { return _init(nullptr, _innerUsed::gmultiThread, _innerUsed::gdebugMode); }
			bool hasInit() { return mdevice.Get(); }

		private:
			bool _init(IDXGIAdapter* adapter, bool multiThread, bool debug);
			ComPtr<ID3D10Device1> mdevice;

		public:
			ID3D10Device1* getDevice()const;
			ComPtr<ID3D10Device1> getDeviceCom()const;
		};




		inline ID3D10Device1* EfD3d10_1Painter::getDevice() const
		{
			return mdevice.Get();
		}

		inline ComPtr<ID3D10Device1> EfD3d10_1Painter::getDeviceCom() const
		{
			return mdevice;
		}
	}
}

#undef ReCatch
#undef ComPtr