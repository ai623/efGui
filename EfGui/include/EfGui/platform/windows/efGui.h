#pragma once
#include "efWindow.h"
#include "efExec.h"

namespace efgui 
{
	struct EfGuiInit 
	{
		EfResult operator() ();
	};

	extern EfGuiInit efGuiInit;
}