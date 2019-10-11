#pragma once
#include <stdint.h>
#include "efDebug.h"


#define _EfGui_Debug_Msg(msg)						efDebugMsg(msg)
#define _EfGui_Debug_Warning_Msg(msg)				efDebugMsg(msg)
#define _EfGui_Debug_Warning_Msg_Code(msg,code)
#define _EfGui_Debug_Error_Msg(msg)					efDebugError(msg)
#define _EfGui_Debug_Error_Msg_Code(str,code)		efDebugError(msg, errorCode)

namespace efgui {
	
}

