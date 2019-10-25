#pragma once
#include "efPainter.h"

namespace efgui {
	struct EfTexture2D
	{
		virtual void getTextureDesc(D3D11_TEXTURE2D_DESC& desc) const = 0;
	};


	struct EfPainterManagerInfo
	{
		bool hasSetRenderTarget = false;
	};

	//Manage painting data
	//Manage drawing painting data 
	//Manage stencil buffer
	//Do not manage rendertarget
	//Do not manage viewport
	//Do not manage depth buffer
	//draw() method may also set painter
	struct IEfPainterManager
	{
		virtual ~IEfPainterManager() {}

		//initiate data but not set painter context
		//shared: share resource between different painters
		virtual bool init(const EfPainter& pt, bool shared = false) = 0;

		//release data
		virtual void uninit() = 0;

		//for nonshared manager, it will recreate resources
		virtual void setPainter(EfPainter& pt) = 0;

		//setCamera()

		//Get dscription about what it does about the painter
		virtual void get(EfPainterManagerInfo& desc)const = 0;

		//Get Infomation read by users.
		virtual std::string getInfo() const = 0;

		//backup data for recover painter context
		virtual void backupContext(IEfPainterManager& manager) const = 0;

		//create texture need for binding
		//virtual ID3D11DepthStencilView* create(const EfTexture2D& text) const = 0;

		virtual void setContext() = 0;

		virtual void draw() = 0;
	};
}

