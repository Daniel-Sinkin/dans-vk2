// vendor/glm/glm/gtc/color_space.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once

#include "../detail/setup.hpp"
#include "../detail/qualifier.hpp"
#include "../exponential.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"
#include <limits>

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_color_space extension included")
#endif

namespace glm
{

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> convertLinearToSRGB(vec<L, T, Q> const& ColorLinear);

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> convertLinearToSRGB(vec<L, T, Q> const& ColorLinear, T Gamma);

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> convertSRGBToLinear(vec<L, T, Q> const& ColorSRGB);

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> convertSRGBToLinear(vec<L, T, Q> const& ColorSRGB, T Gamma);

}

#include "color_space.inl"
