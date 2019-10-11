#include "efGui.h"


namespace efgui 
{
	EfGuiInit efGuiInit;

	EfResult EfGuiInit::operator()()
	{
		auto re = efWindowInit();

		return re;
	}

}
