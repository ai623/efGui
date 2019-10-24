#pragma once
#include <string>

#include <wrl/client.h>

#include "efCamera.h"

#define string std::string
#define wstring std::wstring
#define ComPtr Microsoft::WRL::ComPtr

namespace efgui
{

	struct EfTextWorld
	{
		void setCamera(EfGCPtr<IEfCamera2D> camera);
		void setCamera(IEfCamera2D& camera);
		IEfCamera2D& getCamera();

		static bool initialize();
	private:
		wstring mtext;
		EfGCPtr<IEfCamera2D> mcamera;
	};
}


namespace efgui
{
	inline void EfTextWorld::setCamera(EfGCPtr<IEfCamera2D> camera)
	{
		mcamera = camera;
	}

	inline void EfTextWorld::setCamera(IEfCamera2D& camera)
	{
		camera.addRef();
		mcamera = &camera;
	}

	inline IEfCamera2D& EfTextWorld::getCamera() 
	{
		return *mcamera.get();
	}
}

#undef string
#undef wstring
#undef ComPtr