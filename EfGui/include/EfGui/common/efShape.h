#pragma once

namespace efgui 
{
	template<typename T>
	struct EfPoint {
		T x;
		T y;
	};

	template <typename T>
	struct EfRect {
		T width;
		T height;
	};
}