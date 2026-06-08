// vendor/glm/glm/ext/scalar_integer.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../detail/setup.hpp"
#include "../detail/qualifier.hpp"
#include "../detail/_vectorize.hpp"
#include "../detail/type_float.hpp"
#include "../vector_relational.hpp"
#include "../common.hpp"
#include <limits>

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_scalar_integer extension included")
#endif

namespace glm
{

	template<typename genIUType>
	GLM_FUNC_DECL bool isPowerOfTwo(genIUType v);

	template<typename genIUType>
	GLM_FUNC_DECL genIUType nextPowerOfTwo(genIUType v);

	template<typename genIUType>
	GLM_FUNC_DECL genIUType prevPowerOfTwo(genIUType v);

	template<typename genIUType>
	GLM_FUNC_DECL bool isMultiple(genIUType v, genIUType Multiple);

	template<typename genIUType>
	GLM_FUNC_DECL genIUType nextMultiple(genIUType v, genIUType Multiple);

	template<typename genIUType>
	GLM_FUNC_DECL genIUType prevMultiple(genIUType v, genIUType Multiple);

	template<typename genIUType>
	GLM_FUNC_DECL int findNSB(genIUType x, int significantBitCount);

}

#include "scalar_integer.inl"
