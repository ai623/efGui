#pragma once
#include <EfGui/common/efCommon.h>


namespace efgui 
{
	struct EfLoop 
	{
		
		virtual void loopFunc(){}
	};

	struct EfExec 
	{
		int operator() ();

		void setLoop(EfLoop* loop) { mloop = loop; _isEnableDefaultRun = true; }
		void setLoop(void(*loopFunc)()) { mloopFunc = loopFunc; _isEnableDefaultRun = true; }
		bool isEnableDefaultRun() const { return _isEnableDefaultRun; }
		void enableDefaultRun() { _isEnableDefaultRun = true; }
		void disableDefaultRun() { _isEnableDefaultRun = false; }
		void quit();

	private:
		void (*mloopFunc)() = nullptr;
		bool _isEnableDefaultRun = false;
		bool _quitWhenNowindows = true;
		EfLoop* mloop;
	};

	extern EfExec efExec;
}