#pragma once
#include <stdint.h>
#include <string>

namespace efgui {
	struct EfDebug {
		EfDebug() = default;
		EfDebug(bool on) { isOn = on; }

		void operator() (std::string str);
		void error(std::string str);
		void error(std::string str, uint64_t errorCode);

		bool isOn = true;
	};

	void efDebugMsg(const std::string& msg);
	void efDebugError(const std::string& msg);
	void efDebugError(const std::string& msg, uint64_t errorCode);
}