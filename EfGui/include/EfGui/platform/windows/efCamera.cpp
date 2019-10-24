#include "efCamera.h"

namespace efgui
{
	void EfCamera2D::getCamera2DInfo(EfCamera2DInfo& info) const
	{
		info.ratioX = mWidth / 2;
		info.ratioY = mHeight / 2;
		info.biasX = -mmidX/info.ratioX;
		info.biasY = -mmidY/info.ratioY;
	}

	void EfDpiCamera2D::getCamera2DInfo(EfCamera2DInfo& info) const
	{
		info.ratioX = mWidth * mdpi / 2;
		info.ratioY = mHeight * mdpi / 2;
		info.biasX = -mmidX * mdpi / info.ratioX;
		info.biasY = -mmidY * mdpi / info.ratioY;
	}
}