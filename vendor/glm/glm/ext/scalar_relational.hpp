// vendor/glm/glm/ext/scalar_relational.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../detail/qualifier.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_scalar_relational extension included")
#endif

namespace glm
{

	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR bool equal(genType const& x, genType const& y, genType const& epsilon);

	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR bool notEqual(genType const& x, genType const& y, genType const& epsilon);

	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR bool equal(genType const& x, genType const& y, int ULPs);

	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR bool notEqual(genType const& x, genType const& y, int ULPs);

}

#include "scalar_relational.inl"
