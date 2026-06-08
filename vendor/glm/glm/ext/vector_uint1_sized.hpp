// vendor/glm/glm/ext/vector_uint1_sized.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../ext/vector_uint1.hpp"
#include "../ext/scalar_uint_sized.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_vector_uint1_sized extension included")
#endif

namespace glm
{

	typedef vec<1, uint8, defaultp>		u8vec1;

	typedef vec<1, uint16, defaultp>	u16vec1;

	typedef vec<1, uint32, defaultp>	u32vec1;

	typedef vec<1, uint64, defaultp>	u64vec1;

}
