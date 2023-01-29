#pragma once
#include <d3d9.h>
#include <d3dx9.h>

namespace slv
{
	using vec3 = D3DXVECTOR3;

	struct CubicBezierCurve
	{
		static const int s_pointsCount = 4;
		union
		{
			vec3 points[s_pointsCount];
			struct { vec3 a, b, c, d; };
		};

		CubicBezierCurve();
		CubicBezierCurve(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d);
		// 'points' is an array of "s_pointsCount" elements
		CubicBezierCurve(vec3 *points);

		vec3 Interpolate(float t) const;

		// this method is unsafe
		// be sure that 'index' is valid (in range [0; s_pointsCount-1])
		inline vec3& operator[](int index) { return points[index]; }
		inline vec3 operator[](int index) const { return points[index]; }
	};
}