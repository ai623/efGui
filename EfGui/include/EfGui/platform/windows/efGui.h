#pragma once
#include "efExec.h"
#include "efWindow.h"
#include "efPainter.h"

namespace efgui 
{
	struct EfGuiInit 
	{
		EfResult operator() ();
	};

	extern EfGuiInit efGuiInit;
}