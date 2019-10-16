#pragma once
#include <stdint.h>
#include <string>

#include <crtdbg.h>
#include <Windows.h>

namespace efgui {
	struct EfDebug {
		EfDebug() = default;
		EfDebug(bool on) { isOn = on; }

		void operator() (std::string str);
		void error(std::string str);
		void error(std::string str, uint64_t errorCode);

		bool isOn = true;
	};

	struct EfErrorCatcher
	{
		inline void setHResult(HRESULT hResult)
		{
			mhResult = hResult;
		}

		inline HRESULT getHResult() {
			return mhResult;
		}

	private:
		HRESULT mhResult = 0;
	};
	extern EfErrorCatcher efErrorCatcher;

	void efDebugMsg(const std::string& msg);
	void efDebugError(const std::string& msg);
	void efDebugError(const std::string& msg, uint64_t errorCode);
	inline bool efHasMemoryLeaks(){return _CrtDumpMemoryLeaks();}


}