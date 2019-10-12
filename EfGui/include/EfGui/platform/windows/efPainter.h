#pragma once
#include <EfGui/common/efCommon.h>

//#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <string>

#include "efCommon.h"



#define _REL(p)		_EfGui_Release_Comptr(p)
#define _CPP(x,p)	_EfGui_Copy_Comptr(x,p)
#define _MVP(x,p)	_EfGui_Move_Comptr(x,p)

namespace efgui 
{
	namespace _efPainter {
		extern bool gdebugMode;
	}

	struct EfPainterInit 
	{
		EfResult operator()(bool debugMode, bool multiThread);
		void uninit();
	};

	extern EfPainterInit efPainterInit;

	struct EfAdapter
	{
		~EfAdapter() { _del(); }
		EfAdapter(EfNoInit){}
		EfAdapter(UINT index = 0) { _init(index); }
		bool init(UINT index = 0) { _del(); _init(index); }
	private:
		IDXGIAdapter* madapter = nullptr;

		bool _init(UINT index);
		void _uninit() { _del(); _reset(); }
		void _del() { _REL(madapter); }
		void _reset() { madapter = nullptr; }
		void _copy(const EfAdapter& adapter) { _del(); _CPP(adapter, madapter); }
		void _move(EfAdapter&& adapter) { _del(); _MVP(adapter, madapter); }

		friend struct EfPainter;
	};

	struct EfPainter 
	{
		~EfPainter() { _del(); }
		EfPainter(EfNoInit){}
		EfPainter() { _init(nullptr, _efPainter::gdebugMode); }
		EfPainter(const EfPainter& pt) { _copy(pt); }
		EfPainter(EfPainter&& pt) { _move(std::move(pt)); }

		EfPainter& operator = (const EfPainter& pt) { _copy(pt); return *this; }
		EfPainter& operator = (EfPainter&& pt) { _move(std::move(pt)); return *this; }

	private:
		ID3D11Device* mdevice = nullptr;
		ID3D11DeviceContext* mcontext = nullptr;
		D3D_FEATURE_LEVEL mlevel;

		bool _init(IDXGIAdapter* adapter, bool debugMode);
		void _uninit() { _del(); _reset(); }
		void _del() { _REL(mdevice); _REL(mcontext); }
		void _reset() { mdevice = nullptr; mcontext = nullptr; }
		void _copy(const EfPainter& pt) { _del(); _CPP(pt, mdevice); _CPP(pt, mcontext); mlevel = pt.mlevel; }
		void _move(EfPainter&& pt) { _del(); _MVP(pt, mdevice); _MVP(pt, mcontext); mlevel = pt.mlevel;}

	public:
		ID3D11Device* getDevice()const { return mdevice; }
		ID3D11DeviceContext* getContext() const { return mcontext; }
		D3D_FEATURE_LEVEL getLevel() const { return mlevel; }

		IDXGIFactory* createIDXGIFactory() const;
	};
}




#undef	_REL
#undef	_CPP
#undef	_MVP