#pragma once
#include <EfGui/common/efCommon.h>


namespace efgui 
{
	struct EfExec 
	{
		int operator() ();

		void setDefualtRun(void (*func) ()) { defaultRun = func; _isEnableDefaultRun = true; }
		bool isEnableDefaultRun() const { return _isEnableDefaultRun; }
		void enableDefaultRun() { _isEnableDefaultRun = true; }
		void disableDefaultRun() { _isEnableDefaultRun = false; }
		void quit();

	private:
		void (*defaultRun)() = nullptr;
		bool _isEnableDefaultRun = false;
		bool _quitWhenNowindows = true;
	};

	extern EfExec efExec;
}