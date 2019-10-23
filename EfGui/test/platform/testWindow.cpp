#include <string>
#include <cassert>

#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>
#include <DirectXMath.h>

#include <DirectXTK/Inc/WICTextureLoader.h>

#include <EfGui/platform/windows/efGui.h>
#include <EfGui/platform/windows/font/efFontPainter.h>
#include <EfGui/platform/windows/efCamera.h>
#include <EfGui/platform/windows/efShapeCompute.h>


#define ComPtr Microsoft::WRL::ComPtr

using namespace efgui;
using std::wstring;
using std::string;



struct MyWindow : EfWindow, EfLoop 
{
	~MyWindow() { _uninit(); }

	MyWindow() { _init(); }
	MyWindow(EfPainter& pt) :EfWindow(pt) { _init(); }

	virtual void whenDestroy() {
		_uninit();
	}
	
	virtual void whenMouseMove(EfPoint<long>& pos) {
		auto rect = getRect();
		mmousePos.x = pos.x;
		mmousePos.y = pos.y;
		whenPaint();
	}

	virtual void whenPaint() {
		auto& pt = getPainter();
		auto context = pt.getContext();
		auto rect = getRect();

		mclearColor[0] = (float)mmousePos.x / rect.width;
		mclearColor[1] = (float)mmousePos.y / rect.height;

		context->ClearRenderTargetView(getTargetView(), mclearColor);
		context->ClearDepthStencilView(mdsView.Get(), D3D11_CLEAR_DEPTH, 1., 0);

		if(1)
		{
			context->Draw(4, 0);
		}

		present();
	}

	virtual void whenChar(WPARAM code) {

	}
	virtual void whenIME(WPARAM notify, LPARAM command)
	{
		if (0&&notify == WM_IME_STARTCOMPOSITION) {
			auto imm = ImmGetContext(getHWnd());
			COMPOSITIONFORM  form;
			form.dwStyle = CFS_POINT;
			form.ptCurrentPos.x = 0;
			form.ptCurrentPos.y = 0;
			ImmSetCompositionWindow(imm,&form);
			ImmReleaseContext(getHWnd(), imm);
		}

	}
private:
	struct Vertex2D
	{
		float x;
		float y;
	};

	struct VertexTex2D
	{
		float x;
		float y;
		float u;
		float v;
	};

	EfPoint<long> mmousePos;
	float mclearColor[4]{ 0,0,0,1 };
	D3D11_VIEWPORT mviewport;
	EfCamera2D mcamera;
	wstring mdirPath = L"../x64/Debug/";

	ComPtr<ID3D11Texture2D> mdsBuff;
	ComPtr<ID3D11DepthStencilView> mdsView;
	ComPtr<ID3D11VertexShader> mvshader;
	ComPtr<ID3D11PixelShader> mpshader;
	ComPtr<ID3D11InputLayout> minputLy;

	ComPtr<ID3D11Resource> mpicture;
	ComPtr<ID3D11ShaderResourceView> mpictureView;
	ComPtr<ID3D11Buffer> mvertexBuff;
	ComPtr<ID3D11Buffer> mcameraBuff;
	ComPtr<ID3D11SamplerState> msamplerState;

	ComPtr<ID3D11RasterizerState> mrasterState;
	ComPtr<ID3D11BlendState> mblendState;

	void _init() 
	{
		HRESULT hr;
		auto& painter = getPainter();
		auto context = painter.getContext();
		auto device = painter.getDevice();
		auto rect = getRect();
		auto targetView = getTargetView();

		//init viewport
		{
			mviewport = { 0,0,(float)rect.width,(float)rect.height,0.,1. };
		}
		//create depth stencil buffer
		{
			mdsBuff.Attach(painter.createDSTextureForWnd(*this));
			device->CreateDepthStencilView(mdsBuff.Get(), nullptr, &mdsView);
		}

		//create vs,ps,inputlayout
		{
			using namespace efd3d11;
			using namespace efd3d11::efdesc;

			wstring vsPath = mdirPath + L"VertexShader.cso", psPath = mdirPath + L"PixelShader.cso";

			const D3D11_INPUT_ELEMENT_DESC inputLy[]
			{
				{"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
				{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,8,D3D11_INPUT_PER_VERTEX_DATA,0 }
			};

			auto vBlob = readFileToBlob(vsPath); hr = efErrorCatcher.getHResult();
			mvshader.Attach(painter.createVertexShader(vBlob)); hr = efErrorCatcher.getHResult();
			mpshader.Attach(painter.createPixelShader(psPath)); hr = efErrorCatcher.getHResult();
			minputLy.Attach(painter.createInputLayout(vBlob, inputLy, ARRAYSIZE(inputLy))); hr = efErrorCatcher.getHResult();
			vBlob->Release();
		}

		//create camera buffer
		{
			mcamera.setWidthHeight(mviewport.Width, mviewport.Height);
			mcamera.setLTPos(0, 0);
			EfCamera2DInfo info;
			mcamera.getCamera2DInfo(info);


			D3D11_SUBRESOURCE_DATA data;
			D3D11_BUFFER_DESC desc;

			desc.ByteWidth = sizeof(info);
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			data.pSysMem = &info;

			mcameraBuff.Attach(painter.createBuffer(desc, data));
		}

		//create sampler state 
		{
			using efd3d11::efdesc::gefSamplerDesc;
			auto desc = gefSamplerDesc;
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			//desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			//desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			//desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			//desc.BorderColor[3] = 0;
			msamplerState.Attach(painter.createSamplerState(desc));
		}

		//create render state
		if(0)
		{
			D3D11_RASTERIZER_DESC desc;
			context->RSGetState(&mrasterState);
		}

		//create blend state
		if(0)
		{
			D3D11_BLEND_DESC desc{};
			desc.IndependentBlendEnable = FALSE;
			auto& RTBDesc = desc.RenderTarget[0];
			RTBDesc.BlendEnable = TRUE;


			desc.RenderTarget->BlendEnable = TRUE;
			hr = device->CreateBlendState(&desc, &mblendState);
		}

		float textWidth;
		float textHeight;
		//create texture view
		{
			ComPtr<ID3D11Texture2D> texture;

			//create buffer
			{
				EfFontPainter pt;
				//auto family = pt.getSysFontFamily(L"Microsoft Yahei");
				//auto name = family.getFamilyName();

				pt.setSysFormat(L"Microsoft Yahei", 10);
				pt.setTextLayout(L"Hello,world!");
				auto text = pt.draw();
				textWidth = text.cols;
				textHeight = text.rows;

				D3D11_TEXTURE2D_DESC desc;
				D3D11_SUBRESOURCE_DATA data;
				data.pSysMem = text.data;
				data.SysMemPitch = text.rowByteSize();

				desc.Width = text.cols;
				desc.Height = text.rows;
				desc.MipLevels = 1;
				desc.ArraySize = 1;
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc.SampleDesc = { 1,0 };
				desc.Usage = D3D11_USAGE_IMMUTABLE;
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				hr = device->CreateTexture2D(&desc, &data, &texture);
			}
			

			hr = device->CreateShaderResourceView(texture.Get(), nullptr, &mpictureView);

		}
		//create vertex buffer
		{
			using namespace efshape;
			float pos[2]{};
			float wh[2]{textWidth,textHeight };

			VertexTex2D vertices[8];
			rectToStripe2DTex(pos, wh, (float*)vertices);

			D3D11_SUBRESOURCE_DATA data;
			D3D11_BUFFER_DESC desc;
			desc.ByteWidth = sizeof(vertices);
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			data.pSysMem = vertices;

			mvertexBuff.Attach(painter.createBuffer(desc, data));
		}


		if (0)
		{
			//create texture
			Vertex2D resolution[2]{ {(float)rect.width, (float)rect.height} };
			{
				using DirectX::CreateWICTextureFromFile;
				wstring pngName = L"huaji.jpg";
				hr = CreateWICTextureFromFile(device, (mdirPath + pngName).c_str(), &mpicture, &mpictureView);
				//using efd3d11::createDXGISurface;

			}
			//create vertex buffer
			{
				VertexTex2D rectangle[4];
				Vertex2D rectPos{ 0,0 };
				Vertex2D rectScale{ 400,400 };

				genRect(rectPos, rectScale, rectangle,resolution[0]);
				rectangle[0].u = 0; rectangle[0].v = 0;
				rectangle[1].u = 1; rectangle[1].v = 0;
				rectangle[2].u = 0; rectangle[2].v = 1;
				rectangle[3].u = 1; rectangle[3].v = 1;

				D3D11_SUBRESOURCE_DATA data;
				D3D11_BUFFER_DESC desc;
				desc.ByteWidth = sizeof(rectangle);
				desc.Usage = D3D11_USAGE_IMMUTABLE;
				desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				desc.StructureByteStride = 0;

				data.pSysMem = rectangle;

				mvertexBuff.Attach(painter.createBuffer(desc, data));
			}
	
		}

		//set context
		{
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			context->IASetInputLayout(minputLy.Get());
			painter.setVertexBuffer(mvertexBuff.Get(), sizeof(VertexTex2D));
			painter.setVSConstantBuffer(mcameraBuff.Get());
			painter.setPSConstantBuffer(mcameraBuff.Get());
			painter.setPSShaderResources(mpictureView.Get());
			painter.setPSSampler(msamplerState.Get());
			context->VSSetShader(mvshader.Get(), nullptr, 0);
			context->PSSetShader(mpshader.Get(), nullptr, 0);

			//context->RSSetState(mrasterState.Get());
			context->RSSetViewports(1, &mviewport);
			context->OMSetRenderTargets(1, &targetView, mdsView.Get());
		}
	}
	void _uninit() { if (getWindowsNum() == 1) { efExec.quit(); } }

	void genRect(Vertex2D& pos, Vertex2D& rect, VertexTex2D* data, Vertex2D& wh) {
		float l,t,r,b;
		l = pos.x *2 - wh.x;
		t = -pos.y * 2 + wh.y;
		r = l+ rect.x * 2;
		b = t - rect.y * 2;

		data[0] = { l,t };
		data[1] = { r,t };
		data[2] = { l,b } ;
		data[3] = { r,b };
	}
};



int efMain() {

	efGuiInit.enableDebugMode();
	auto re = efGuiInit();

//	{
//#if (_WIN32_WINNT >= 0x0A00 /*_WIN32_WINNT_WIN10*/)
//		Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
//		if (FAILED(initialize))
//			return 0;
//#else
//		HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
//		if (FAILED(hr))
//			return 0;
//#endif
//	}



	MyWindow wnd;

	efExec();

	
	return 0;
}

#undef ComPtr