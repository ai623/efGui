#include "efGui.h"


namespace efgui 
{
	EfGuiInit efGuiInit;

	EfResult EfGuiInit::operator()()
	{
		EfResult re;
		_innerUsed::gdebugMode = mdebugMode;
		_innerUsed::gmultiThread = mmultiThread;
		_innerUsed::gsampleCount = msampleCount;
		re = efWindowInit();
		if (re)return re;
		re = efPainterInit(mdebugMode, mmultiThread,msampleCount);
		if (re) return re;
		return re;
	}

	void EfGuiInit::uninit()
	{
		efPainterInit.uninit();
	}

}
