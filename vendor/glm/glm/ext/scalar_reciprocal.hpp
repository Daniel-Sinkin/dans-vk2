// vendor/glm/glm/ext/scalar_reciprocal.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../detail/setup.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_scalar_reciprocal extension included")
#endif

namespace glm
{

	template<typename genType>
	GLM_FUNC_DECL genType sec(genType angle);

	template<typename genType>
	GLM_FUNC_DECL genType csc(genType angle);

	template<typename genType>
	GLM_FUNC_DECL genType cot(genType angle);

	template<typename genType>
	GLM_FUNC_DECL genType asec(genType x);

	template<typename genType>
	GLM_FUNC_DECL genType acsc(genType x);

	template<typename genType>
	GLM_FUNC_DECL genType acot(genType x);

	template<typename genType>
	GLM_FUNC_DECL genType sech(genType angle);

	template<typename genType>
	GLM_FUNC_DECL genType csch(genType angle);

	template<typename genType>
	GLM_FUNC_DECL genType coth(genType angle);

	template<typename genType>
	GLM_FUNC_DECL genType asech(genType x);

	template<typename genType>
	GLM_FUNC_DECL genType acsch(genType x);

	template<typename genType>
	GLM_FUNC_DECL genType acoth(genType x);

}

#include "scalar_reciprocal.inl"
