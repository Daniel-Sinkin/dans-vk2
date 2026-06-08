// vendor/glm/glm/ext/vector_bool1_precision.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../detail/type_vec1.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_vector_bool1_precision extension included")
#endif

namespace glm
{

	typedef vec<1, bool, highp>			highp_bvec1;

	typedef vec<1, bool, mediump>		mediump_bvec1;

	typedef vec<1, bool, lowp>			lowp_bvec1;

}
