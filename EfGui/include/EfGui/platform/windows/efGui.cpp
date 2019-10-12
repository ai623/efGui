#include "efGui.h"


namespace efgui 
{
	EfGuiInit efGuiInit;

	EfResult EfGuiInit::operator()()
	{
		EfResult re;
		re = efWindowInit();
		if (re)return re;
		re = efPainterInit(mdebugMode, mmultiThread);
		if (re) return re;
		return re;
	}

	void EfGuiInit::uninit()
	{
		efPainterInit.uninit();
	}

}
