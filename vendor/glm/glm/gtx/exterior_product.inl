// vendor/glm/glm/gtx/exterior_product.inl
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#include <limits>

namespace glm {
namespace detail
{
	template<typename T, qualifier Q, bool Aligned>
	struct compute_cross_vec2
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static T call(vec<2, T, Q> const& v, vec<2, T, Q> const& u)
		{
			GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559 || GLM_CONFIG_UNRESTRICTED_FLOAT, "'cross' accepts only floating-point inputs");

			return v.x * u.y - u.x * v.y;
		}
	};
}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T cross(vec<2, T, Q> const& x, vec<2, T, Q> const& y)
	{
		return detail::compute_cross_vec2<T, Q, detail::is_aligned<Q>::value>::call(x, y);
	}
}

