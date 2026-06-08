// vendor/glm/glm/ext/matrix_int2x4_sized.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../mat2x4.hpp"
#include "../ext/scalar_int_sized.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_matrix_int2x4_sized extension included")
#endif

namespace glm
{

	typedef mat<2, 4, int8, defaultp>				i8mat2x4;

	typedef mat<2, 4, int16, defaultp>				i16mat2x4;

	typedef mat<2, 4, int32, defaultp>				i32mat2x4;

	typedef mat<2, 4, int64, defaultp>				i64mat2x4;

}
