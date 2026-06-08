// vendor/glm/glm/ext/matrix_double3x3_precision.hpp
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once
#include "../detail/type_mat3x3.hpp"

namespace glm
{

	typedef mat<3, 3, double, lowp>		lowp_dmat3;

	typedef mat<3, 3, double, mediump>	mediump_dmat3;

	typedef mat<3, 3, double, highp>	highp_dmat3;

	typedef mat<3, 3, double, lowp>		lowp_dmat3x3;

	typedef mat<3, 3, double, mediump>	mediump_dmat3x3;

	typedef mat<3, 3, double, highp>	highp_dmat3x3;

}
