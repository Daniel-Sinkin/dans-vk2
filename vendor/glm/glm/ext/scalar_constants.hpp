// vendor/glm/glm/ext/scalar_constants.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../detail/setup.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_scalar_constants extension included")
#endif

namespace glm
{

	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType epsilon();

	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType pi();

	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType cos_one_over_two();

}

#include "scalar_constants.inl"
