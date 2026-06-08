// vendor/glm/glm/ext/matrix_double4x4_precision.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once
#include "../detail/type_mat4x4.hpp"

namespace glm
{

	typedef mat<4, 4, double, lowp>		lowp_dmat4;

	typedef mat<4, 4, double, mediump>	mediump_dmat4;

	typedef mat<4, 4, double, highp>	highp_dmat4;

	typedef mat<4, 4, double, lowp>		lowp_dmat4x4;

	typedef mat<4, 4, double, mediump>	mediump_dmat4x4;

	typedef mat<4, 4, double, highp>	highp_dmat4x4;

}
