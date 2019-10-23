#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

#include <string>

#include "efCommon.h"
#include "efCamera.h"


#undef near
#undef far

#define _REL(p)		_EfGui_Release_Comptr(p)
#define _CPP(x,p)	_EfGui_Copy_Comptr(x,p)
#define _MVP(x,p)	_EfGui_Move_Comptr(x,p)
#define ReCatch(re) efErrorCatcher.setHResult(re)

namespace efgui
{
	namespace _efPainter {
		extern bool gdebugMode;
		extern bool gmultiThread;
		extern UINT gsampleCount;
	}

	struct EfPainterInit
	{
		EfResult operator()(bool debugMode, bool multiThread, UINT sampleCount);

		void uninit();
	};

	extern EfPainterInit efPainterInit;


	struct EfAdapter
	{
		~EfAdapter() { _del(); }
		EfAdapter(EfNoInit) {}
		EfAdapter(UINT index = 0) { _init(index); }
		bool init(UINT index = 0) { _del(); _init(index); }
		void uninit() { _uninit(); }


	private:
		IDXGIAdapter* madapter = nullptr;

		bool _init(UINT index);
		void _uninit() { _del(); _reset(); }
		void _del() { _REL(madapter); }
		void _reset() { madapter = nullptr; }
		void _copy(const EfAdapter& adapter) { _del(); _CPP(adapter, madapter); }
		void _move(EfAdapter&& adapter) { _del(); _MVP(adapter, madapter); }

		friend struct EfPainter;
		friend struct EfAdapterOutput;
	};

	struct EfAdapterOutput
	{
		~EfAdapterOutput() { _del(); }
		EfAdapterOutput(EfNoInit) {}
		EfAdapterOutput(const EfAdapter& adp, UINT index = 0) { _init(adp.madapter, index); }

		bool init(const EfAdapter& adp, UINT index) { _del(); return _init(adp.madapter, index); }
		void uninit() { _uninit(); }

		IDXGIOutput* getDXGIOutput() const { return moutput; }
		HMONITOR getHMonitor() const { DXGI_OUTPUT_DESC desc; moutput->GetDesc(&desc); return desc.Monitor; }
	private:
		IDXGIOutput* moutput = nullptr;

		bool _init(IDXGIAdapter* apt, UINT index);
		void _uninit() { _del(); _reset(); }
		void _del() { _REL(moutput); }
		void _reset() { moutput = nullptr; }

	public:

	};

	struct EfWindow;



	struct EfPainter
	{
		~EfPainter() { _del(); }
		EfPainter(EfNoInit) {}
		EfPainter() { _init(nullptr, _efPainter::gdebugMode); }
		EfPainter(const EfPainter& pt) { _copy(pt); }
		EfPainter(EfPainter&& pt) { _move(std::move(pt)); }

		EfPainter& operator = (const EfPainter& pt) { _copy(pt); return *this; }
		EfPainter& operator = (EfPainter&& pt) { _move(std::move(pt)); return *this; }

		bool init() { _del(); return _init(nullptr, _efPainter::gdebugMode); }
		void uninit() { _uninit(); }

		void setRenderTarget(EfWindow& wnd);
	private:
		ID3D11Device* mdevice = nullptr;
		ID3D11DeviceContext* mcontext = nullptr;
		D3D_FEATURE_LEVEL mlevel;

		bool _init(IDXGIAdapter* adapter, bool debugMode);
		void _uninit() { _del(); _reset(); }
		void _del() { _REL(mdevice); _REL(mcontext); }
		void _reset() { mdevice = nullptr; mcontext = nullptr; }
		void _copy(const EfPainter& pt) { _del(); _CPP(pt, mdevice); _CPP(pt, mcontext); mlevel = pt.mlevel; }
		void _move(EfPainter&& pt) { _del(); _MVP(pt, mdevice); _MVP(pt, mcontext); mlevel = pt.mlevel; }

	public:
		ID3D11Device* getDevice()const { return mdevice; }
		ID3D11DeviceContext* getContext() const { return mcontext; }
		D3D_FEATURE_LEVEL getLevel() const { return mlevel; }

		inline void setVertexBuffer(ID3D11Buffer* buff,UINT eleSize, UINT startSlot = 0, UINT offset = 0) const;
		inline void setVSConstantBuffer(ID3D11Buffer* buff, UINT startSlot = 0) const;
		inline void setPSConstantBuffer(ID3D11Buffer* buff, UINT startSlot = 0) const;
		inline void setPSShaderResources(ID3D11ShaderResourceView* res, UINT startSlot = 0) const;
		inline void setPSSampler(ID3D11SamplerState* state)const;

		IDXGIDevice* createDXGIDevice() const;
		IDXGIAdapter* createDXGIAdpter() const;
		IDXGIFactory* createDXGIFactory() const;
		ID3D11Texture2D* createDSTextureForWnd(const EfWindow& wnd)const;

		inline ID3D11VertexShader* createVertexShader(ID3DBlob* vFile) const;
		inline ID3D11VertexShader* createVertexShader(const std::wstring& path) const;
		inline ID3D11PixelShader* createPixelShader(ID3DBlob* pFile) const;
		inline ID3D11PixelShader* createPixelShader(const std::wstring& path) const;
		inline ID3D11InputLayout* createInputLayout(ID3DBlob* vFile, const D3D11_INPUT_ELEMENT_DESC* desc, unsigned int num)const;
		inline ID3D11Buffer* createBuffer(const D3D11_BUFFER_DESC& buffDesc, const D3D11_SUBRESOURCE_DATA& resDesc)const;
		inline ID3D11SamplerState* createSamplerState(const D3D11_SAMPLER_DESC& desc)const;
	};



	inline void EfPainter::setVertexBuffer(ID3D11Buffer* buff,UINT stride, UINT startSlot, UINT offset) const
	{
		mcontext->IASetVertexBuffers(startSlot, 1, &buff, &stride, &offset);
	}

	inline void EfPainter::setVSConstantBuffer(ID3D11Buffer* buff, UINT startSlot) const
	{
		mcontext->VSSetConstantBuffers(startSlot, 1, &buff);
	}

	inline void EfPainter::setPSConstantBuffer(ID3D11Buffer* buff, UINT startSlot) const
	{
		mcontext->PSSetConstantBuffers(startSlot, 1, &buff);
	}

	inline void EfPainter::setPSShaderResources(ID3D11ShaderResourceView* res, UINT startSlot) const
	{
		mcontext->PSSetShaderResources(startSlot, 1, &res);
	}

	inline void EfPainter::setPSSampler(ID3D11SamplerState* state) const
	{
		mcontext->PSSetSamplers(0, 1, &state);
	}


	inline ID3D11VertexShader* EfPainter::createVertexShader(ID3DBlob* blob) const
	{
		HRESULT hr;
		ID3D11VertexShader* vs;
		ReCatch(hr = mdevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vs));
		return vs;
	}

	inline ID3D11VertexShader* EfPainter::createVertexShader(const std::wstring& path) const
	{
		HRESULT hr;
		ID3D11VertexShader* vs;
		ID3DBlob* blob;

		ReCatch(D3DReadFileToBlob(path.c_str(), &blob));
		ReCatch(hr = mdevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vs));

		return vs;
	}

	inline ID3D11PixelShader* EfPainter::createPixelShader(ID3DBlob* blob) const
	{
		HRESULT hr;
		ID3D11PixelShader* ps;
		ReCatch(hr = mdevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &ps));
		return ps;
	}

	inline ID3D11PixelShader* EfPainter::createPixelShader(const std::wstring& path) const
	{
		HRESULT hr;
		ID3D11PixelShader* ps;
		ID3DBlob* blob;

		ReCatch(D3DReadFileToBlob(path.c_str(), &blob));
		ReCatch(hr = mdevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &ps));
		return ps;
	}

	inline ID3D11InputLayout* EfPainter::createInputLayout(ID3DBlob* blob,const D3D11_INPUT_ELEMENT_DESC* desc, unsigned int num) const
	{
		HRESULT hr;
		ID3D11InputLayout* layout;
		ReCatch(hr = mdevice->CreateInputLayout(desc, num, blob->GetBufferPointer(), blob->GetBufferSize(), &layout));
		return layout;
	}

	inline ID3D11Buffer* EfPainter::createBuffer(const D3D11_BUFFER_DESC& buffDesc, const D3D11_SUBRESOURCE_DATA& resDesc) const
	{
		HRESULT hr;
		ID3D11Buffer* buffer;
		ReCatch(hr = mdevice->CreateBuffer(&buffDesc, &resDesc, &buffer));
		return buffer;
	}

	inline ID3D11SamplerState* EfPainter::createSamplerState(const D3D11_SAMPLER_DESC& desc) const
	{
		HRESULT hr;
		ID3D11SamplerState* sampler;
		ReCatch(hr = mdevice->CreateSamplerState(&desc, &sampler));
		return sampler;
	}

}



namespace efgui
{
	struct Frustum2D
	{
		float width;
		float height;
	};
	
	struct Frustum3D
	{
		float width;
		float height;
		float focus; //distance between project plane and focus
	};
}


#undef ReCatch
#undef	_REL
#undef	_CPP
#undef	_MVP