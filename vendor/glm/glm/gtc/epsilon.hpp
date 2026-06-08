// vendor/glm/glm/gtc/epsilon.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../detail/setup.hpp"
#include "../detail/qualifier.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_epsilon extension included")
#endif

namespace glm
{

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, bool, Q> epsilonEqual(vec<L, T, Q> const& x, vec<L, T, Q> const& y, T const& epsilon);

	template<typename genType>
	GLM_FUNC_DECL bool epsilonEqual(genType const& x, genType const& y, genType const& epsilon);

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, bool, Q> epsilonNotEqual(vec<L, T, Q> const& x, vec<L, T, Q> const& y, T const& epsilon);

	template<typename genType>
	GLM_FUNC_DECL bool epsilonNotEqual(genType const& x, genType const& y, genType const& epsilon);

}

#include "epsilon.inl"
