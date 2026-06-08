// vendor/glm/glm/ext/vector_float1_precision.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../detail/type_vec1.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_vector_float1_precision extension included")
#endif

namespace glm
{

	typedef vec<1, float, highp>		highp_vec1;

	typedef vec<1, float, mediump>		mediump_vec1;

	typedef vec<1, float, lowp>			lowp_vec1;

}
