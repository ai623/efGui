#include "efCamera.h"

namespace efgui
{
	void EfCamera2D::getCamera2DInfo(EfCamera2DInfo& info)
	{
		info.ratioX = mWidth / 2;
		info.ratioY = mHeight / 2;
		info.biasX = -mmidX/info.ratioX;
		info.biasY = -mmidY/info.ratioY;
	}
}