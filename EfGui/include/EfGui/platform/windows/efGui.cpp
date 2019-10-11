#include "efGui.h"


namespace efgui 
{
	EfGuiInit efGuiInit;

	EfResult EfGuiInit::operator()()
	{
		EfResult re;
		re = efWindowInit();
		if (re)return re;
		re = efPainterInit(true, false);
		if (re) return re;
		return re;
	}

}
