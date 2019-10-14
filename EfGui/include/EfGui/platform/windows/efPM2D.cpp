#include <d3dcompiler.h>
#include <string>

#include "efPM2D.h"


#define ComPtr Microsoft::WRL::ComPtr

namespace efgui
{
	void EfPMPolygon::setData(EfRect<float>& rect)
	{

	}

	bool EfPMPolygon::_init(const EfPainter& pt, const D3D11_VIEWPORT& vp, bool shared)
	{
		mpainter = pt;
		mShared = shared;
		auto device = mpainter.getDevice();

		ComPtr<ID3DBlob> vFile, pFile;

		if (shared) {
			//TODO
			return false;
		}
		else {
			
		}

		return true;
	}
}

#undef ComPtr