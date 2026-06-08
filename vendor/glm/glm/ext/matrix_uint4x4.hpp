// vendor/glm/glm/ext/matrix_uint4x4.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../mat4x4.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_matrix_uint4x4 extension included")
#endif

namespace glm
{

	typedef mat<4, 4, uint, defaultp>	umat4x4;

	typedef mat<4, 4, uint, defaultp>	umat4;

}
