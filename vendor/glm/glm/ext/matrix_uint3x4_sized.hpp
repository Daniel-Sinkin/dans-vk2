// vendor/glm/glm/ext/matrix_uint3x4_sized.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../mat3x4.hpp"
#include "../ext/scalar_uint_sized.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_matrix_uint3x4_sized extension included")
#endif

namespace glm
{

	typedef mat<3, 4, uint8, defaultp>				u8mat3x4;

	typedef mat<3, 4, uint16, defaultp>				u16mat3x4;

	typedef mat<3, 4, uint32, defaultp>				u32mat3x4;

	typedef mat<3, 4, uint64, defaultp>				u64mat3x4;

}
