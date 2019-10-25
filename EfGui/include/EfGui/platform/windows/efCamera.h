#pragma once
#include <Windows.h>

#include <EfGui/common/efGCObj.h>

namespace efgui
{
	struct EfCamera2DInfo
	{
		float ratioX;		
		float ratioY;
		float biasX;			//after scale the coordinates
		float biasY;			//after scale the coordinates
	};

	struct EfCamera3DInfo
	{

	};

	struct IEfCamera2D //: EfGCObj
	{
		virtual ~IEfCamera2D() {}
		virtual void getCamera2DInfo(EfCamera2DInfo&) const = 0;
	};

	struct IEfCamera3D //: EfGCObj
	{
		virtual ~IEfCamera3D() {}
		virtual void getCamera3DInfo(EfCamera3DInfo&) const = 0;
	};

	struct EfCamera2D : IEfCamera2D
	{
		virtual void getCamera2DInfo(EfCamera2DInfo& info) const;
		void setWidthHeight(float width, float height);
		void setCenterPos(float x, float y);
		void setLTPos(float x, float y);					//Depends on the current width and height!
	protected:
		float mmidX = 0;
		float mmidY = 0;
		float mWidth = 1;
		float mHeight = 1;
	};

	struct EfDpiCamera2D : EfCamera2D
	{
		virtual void getCamera2DInfo(EfCamera2DInfo& info) const;
	protected:
		float mdpi = (float) GetDpiForSystem();
	};

}

namespace efgui
{
	inline void EfCamera2D::setLTPos(float x, float y)
	{
		mmidX = x + mWidth / 2;
		mmidY = y - mHeight / 2;
	}
	
	inline void EfCamera2D::setCenterPos(float x, float y)
	{
		mmidX = x;
		mmidY = y;
	}

	inline void EfCamera2D::setWidthHeight(float width, float height)
	{
		mWidth = width;
		mHeight = height;
	}

}