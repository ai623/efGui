#pragma once
#include "efGuiWindow.h"
#include "efExec.h"

namespace efgui 
{
	struct EfGuiInit 
	{
		EfResult operator() ();
	};

	extern EfGuiInit efGuiInit;
}