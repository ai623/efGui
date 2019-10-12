#pragma once
#include "efExec.h"
#include "efWindow.h"
#include "efPainter.h"

namespace efgui 
{
	struct EfGuiInit 
	{
		EfResult operator() ();

		void enableDebugMode() { mdebugMode = true; }
		void disableDebugMode() { mdebugMode = false; }

		void enableMutiThread() { mmultiThread = true; }
		void disableMutiThread() { mmultiThread = false; }

		void uninit();

	private:
		bool mdebugMode = false;
		bool mmultiThread = false;
	};

	extern EfGuiInit efGuiInit;
}