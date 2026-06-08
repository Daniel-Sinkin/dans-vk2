// vendor/glm/glm/detail/func_exponential_simd.inl
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#include "../simd/exponential.h"

#if GLM_ARCH & GLM_ARCH_SSE2_BIT

namespace glm{
namespace detail
{
	template<qualifier Q>
	struct compute_sqrt<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<4, float, Q> call(vec<4, float, Q> const& v)
		{
			vec<4, float, Q> Result;
			Result.data = _mm_sqrt_ps(v.data);
			return Result;
		}
	};

#	if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
	template<>
	struct compute_sqrt<4, float, aligned_lowp, true>
	{
		GLM_FUNC_QUALIFIER static vec<4, float, aligned_lowp> call(vec<4, float, aligned_lowp> const& v)
		{
			vec<4, float, aligned_lowp> Result;
			Result.data = glm_vec4_sqrt_lowp(v.data);
			return Result;
		}
	};
#	endif
}
}

#endif
