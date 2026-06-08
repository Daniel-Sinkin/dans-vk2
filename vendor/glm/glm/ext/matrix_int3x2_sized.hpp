// vendor/glm/glm/ext/matrix_int3x2_sized.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../mat3x2.hpp"
#include "../ext/scalar_int_sized.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_matrix_int3x2_sized extension included")
#endif

namespace glm
{

	typedef mat<3, 2, int8, defaultp>				i8mat3x2;

	typedef mat<3, 2, int16, defaultp>				i16mat3x2;

	typedef mat<3, 2, int32, defaultp>				i32mat3x2;

	typedef mat<3, 2, int64, defaultp>				i64mat3x2;

}
