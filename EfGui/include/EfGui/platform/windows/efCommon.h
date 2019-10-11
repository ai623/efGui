#pragma once
#include <stdint.h>

#define _Debug_Msg(msg)						efDebugMsg(msg)
#define _Debug_Warning_Msg(msg)
#define _Debug_Warning_Msg_Code(msg,code)
#define _Debug_Error_Msg(msg)				efDebugError(msg)
#define _Debug_Error_Msg_Code(str,code)		efDebugError(msg, errorCode)

namespace efgui {
	
}

