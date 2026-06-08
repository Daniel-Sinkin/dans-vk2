// vendor/glm/glm/gtx/matrix_transform_2d.inl
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#include "../trigonometric.hpp"

namespace glm
{

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> translate(
		mat<3, 3, T, Q> const& m,
		vec<2, T, Q> const& v)
	{
		mat<3, 3, T, Q> Result(m);
		Result[2] = m[0] * v[0] + m[1] * v[1] + m[2];
		return Result;
	}


	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> rotate(
		mat<3, 3, T, Q> const& m,
		T angle)
	{
		T const a = angle;
		T const c = cos(a);
		T const s = sin(a);

		mat<3, 3, T, Q> Result;
		Result[0] = m[0] * c + m[1] * s;
		Result[1] = m[0] * -s + m[1] * c;
		Result[2] = m[2];
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> scale(
		mat<3, 3, T, Q> const& m,
		vec<2, T, Q> const& v)
	{
		mat<3, 3, T, Q> Result;
		Result[0] = m[0] * v[0];
		Result[1] = m[1] * v[1];
		Result[2] = m[2];
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> shearX(
		mat<3, 3, T, Q> const& m,
		T y)
	{
		mat<3, 3, T, Q> Result(1);
		Result[0][1] = y;
		return m * Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> shearY(
		mat<3, 3, T, Q> const& m,
		T x)
	{
		mat<3, 3, T, Q> Result(1);
		Result[1][0] = x;
		return m * Result;
	}

}
