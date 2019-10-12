#pragma once
#include <stdint.h>
#include "efDebug.h"

#define _EfGui_Debug_Msg(msg)						efDebugMsg(msg)
#define _EfGui_Debug_Warning_Msg(msg)				efDebugMsg(msg)
#define _EfGui_Debug_Warning_Msg_Code(msg,code)
#define _EfGui_Debug_Error_Msg(msg)					efDebugError(msg)
#define _EfGui_Debug_Error_Msg_Code(str,code)		efDebugError(msg, errorCode)


#define _EfGui_Release_Comptr(p)			if(p){p->Release();}
#define _EfGui_Release_Comptr_To_Null(p)	if(p) {p->Release(); p=nullptr;}
#define _EfGui_AddRef_Comptr(p)				if(p) {p->AddRef();}
#define _EfGui_Copy_Comptr(x,p)				this->p = x.p; _EfGui_AddRef_Comptr(x.p)
#define _EfGui_Move_Comptr(x,p)				this->p = x.p; x.p=nullptr


namespace efgui 
{
	namespace _innerUsed 
	{
		
	}

}

