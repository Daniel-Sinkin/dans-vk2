// vendor/glm/glm/ext/matrix_int3x3_sized.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../mat3x3.hpp"
#include "../ext/scalar_int_sized.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_matrix_int3x3_sized extension included")
#endif

namespace glm
{

	typedef mat<3, 3, int8, defaultp>				i8mat3x3;

	typedef mat<3, 3, int16, defaultp>				i16mat3x3;

	typedef mat<3, 3, int32, defaultp>				i32mat3x3;

	typedef mat<3, 3, int64, defaultp>				i64mat3x3;


	typedef mat<3, 3, int8, defaultp>				i8mat3;

	typedef mat<3, 3, int16, defaultp>				i16mat3;

	typedef mat<3, 3, int32, defaultp>				i32mat3;

	typedef mat<3, 3, int64, defaultp>				i64mat3;

}
