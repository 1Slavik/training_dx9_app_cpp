#include "CubicBezierCurve.h"

namespace slv
{
	CubicBezierCurve::CubicBezierCurve()
		: a(vec3()), b(vec3()), c(vec3()), d(vec3())
	{}
	CubicBezierCurve::CubicBezierCurve(const vec3 & a, const vec3 & b, const vec3 & c, const vec3 & d)
		: a(a), b(b), c(c), d(d)
	{}
	CubicBezierCurve::CubicBezierCurve(vec3 *points)
		: a(points[0]), b(points[1]), c(points[2]), d(points[3])
	{}

	vec3 CubicBezierCurve::Interpolate(float t) const
	{
		float tCompl = 1.0f - t;
		float tCompl2 = tCompl * tCompl;
		float tCompl3 = tCompl2 * tCompl;
		float t2 = t * t;
		float t3 = t2 * t;
		return
			tCompl3 * a
			+ 3.0f * t * tCompl2 * b
			+ 3.0f * t2 * tCompl * c
			+ t3 * d;
	}
}