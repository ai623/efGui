#pragma once
#include <type_traits>
#include <array>

namespace efgui 
{

	template<typename T, size_t Size>
	struct EfVector : std::array<T, Size>{

	};

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