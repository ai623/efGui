#pragma once

namespace efgui
{
	namespace efshape
	{
		template<typename T>
		inline void rectToStripe2D(const T ltPos[2],const T wh[2], T reData[2 * 4])
		{
			auto l = ltPos[0], t = ltPos[1];
			auto r = l + wh[0], b = t - wh[1];
			auto p = reData;
#define Assign(a,b) *p =a; p++; *p =b; p++
			Assign(l, t);
			Assign(r, t);
			Assign(l, b);
			Assign(r, b);
#undef Assign
		}

		template<typename T>
		inline void rectToStripe2DTex(const T ltPos[2], const T wh[2], T reData[4 * 4])
		{
			auto l = ltPos[0], t = ltPos[1];
			auto r = l + wh[0], b = t - wh[1];
			auto p = reData;
#define Assign(a,b,u,v) *p =a; p++; *p =b; p++; *p = u; p++; *p =v; p++
			Assign(l, t, 0, 0);
			Assign(r, t, 1, 0);
			Assign(l, b, 0, 1);
			Assign(r, b, 1, 1);
#undef Assign
		}
	}
}