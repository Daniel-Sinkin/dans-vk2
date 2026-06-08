// vendor/glm/glm/ext/matrix_uint2x3_sized.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../mat2x3.hpp"
#include "../ext/scalar_uint_sized.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_matrix_uint2x3_sized extension included")
#endif

namespace glm
{

	typedef mat<2, 3, uint8, defaultp>				u8mat2x3;

	typedef mat<2, 3, uint16, defaultp>				u16mat2x3;

	typedef mat<2, 3, uint32, defaultp>				u32mat2x3;

	typedef mat<2, 3, uint64, defaultp>				u64mat2x3;

}
