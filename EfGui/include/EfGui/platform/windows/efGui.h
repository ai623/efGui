#pragma once
#include "efCommon.h"
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

		void setSampleCount(UINT count) { msampleCount = count; }
		void disableMultiSample() { msampleCount = 1; }

		void uninit();

	private:
		bool mdebugMode = false;
		bool mmultiThread = false;
		UINT msampleCount = 1;
	};

	extern EfGuiInit efGuiInit;
}