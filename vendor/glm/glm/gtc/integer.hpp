// vendor/glm/glm/gtc/integer.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../detail/setup.hpp"
#include "../detail/qualifier.hpp"
#include "../common.hpp"
#include "../integer.hpp"
#include "../exponential.hpp"
#include "../ext/scalar_common.hpp"
#include "../ext/vector_common.hpp"
#include <limits>

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_integer extension included")
#endif

namespace glm
{

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> log2(vec<L, T, Q> const& v);

}

#include "integer.inl"
