// vendor/glm/glm/ext/quaternion_relational.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../vector_relational.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_quaternion_relational extension included")
#endif

namespace glm
{

	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, bool, Q> equal(qua<T, Q> const& x, qua<T, Q> const& y);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, bool, Q> equal(qua<T, Q> const& x, qua<T, Q> const& y, T epsilon);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, bool, Q> notEqual(qua<T, Q> const& x, qua<T, Q> const& y);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, bool, Q> notEqual(qua<T, Q> const& x, qua<T, Q> const& y, T epsilon);

}

#include "quaternion_relational.inl"
