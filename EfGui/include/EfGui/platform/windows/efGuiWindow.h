#pragma once
#include <EfGui/common/efCommon.h>

#include <stdint.h>

#define UNICODE
#include <Windows.h>

#include "efCommon.h"
#include "efDebug.h"



namespace efgui 
{
	namespace _innerUsed 
	{
		//extern const wchar_t gwcName[] = L"BaseWindow";
		//extern HINSTANCE ghInstance;
		//extern PWSTR gpCmdLine;
		//extern int gnCmdShow;
	}

	extern struct EfWindowInit efWindowInit;
	//public type 
	//destructor
	//constructor
	//checker
	//getter
	//setter
	//other methods

	//private:
	//inner type
	//private data
	//private methods
	//friend struct
	//friend func

	//common
	//...

	inline bool EfSuccess(EfResult re) { return re == 0; }
	inline bool EfFail(EfResult re) { return re; }

	struct EfWindowInit 
	{
		EfResult operator() ();
	};


	struct EfWindow 
	{
		//destructor
		virtual ~EfWindow() { _uninit(); }
		//constructor
		EfWindow(EfNoInit){}
		EfWindow(){ _init(); }
		//init
		bool init() { _init(); }
		//checker
		//getter
		int getWindowsNum() const;
		//callback
		virtual void whenCreate() {}			//Caution: when construct, the son's virtual methods not initialized!
		virtual void whenDestroy() {}			//Caution: when destruct virtual methods have been destroyed!
		virtual void whenPaint() {}
	private:
		bool _init();
		void _uninit() { if (mhWnd) { DestroyWindow(mhWnd); } }
		void _copy(const EfWindow&) = delete;
		void _move(EfWindow&&) = delete;

		HWND mhWnd = NULL;

	public:
		HWND getHWnd() const { return mhWnd; }
		void setHWndNull() { mhWnd = NULL; }
	};

}