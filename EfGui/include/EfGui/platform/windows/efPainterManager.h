#pragma once
#include "efPainter.h"

namespace efgui {
	struct EfTexture2D
	{
		virtual void getTextureDesc(D3D11_TEXTURE2D_DESC& desc) const = 0;
	};

	struct EfRenderTarget
	{
		//virtual ID3D11RenderTargetView* createTargetView() const = 0;
		//virtual void getTextureDesc(D3D11_TEXTURE2D_DESC& desc) const = 0;
		virtual ID3D11RenderTargetView* getTargetView()const = 0;
	};

	struct EfDepthStencilCreator
	{
		virtual ID3D11DepthStencilView* create(const EfPainter& pt, const D3D11_VIEWPORT& text) const = 0;
	};

	struct EfPainterManagerDesc
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
	struct EfPainterManager
	{
		virtual ~EfPainterManager() {}

		//initiate data but not set painter context
		//shared: share resource between different painters
		virtual bool init(const EfPainter& pt, bool shared = false) = 0;

		//release data
		virtual void uninit() = 0;

		//for nonshared manager, it will recreate resources
		virtual bool changePainter(const EfPainter& pt) = 0;

		virtual bool changeViewport(const D3D11_VIEWPORT& vp) = 0;

		//Get dscription about what it does about the painter
		virtual bool get(EfPainterManagerDesc& desc)const = 0;

		//Get Infomation read by users.
		virtual std::string getInfo() const = 0;

		//backup data for recover painter context
		virtual bool backup(EfPainterManager& manager) const = 0;

		//set painter
		virtual bool set(EfPainter& painter) = 0;

		//create texture need for binding
		virtual ID3D11DepthStencilView* create(const EfTexture2D& text) const = 0;

		//virtual bool draw() = 0;
	};

}

