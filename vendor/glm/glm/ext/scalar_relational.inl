// vendor/glm/glm/ext/scalar_relational.inl
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#include "../common.hpp"
#include "../ext/scalar_int_sized.hpp"
#include "../ext/scalar_uint_sized.hpp"
#include "../detail/type_float.hpp"

namespace glm
{
	template<typename genType>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR bool equal(genType const& x, genType const& y, genType const& epsilon)
	{
		return abs(x - y) <= epsilon;
	}

	template<typename genType>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR bool notEqual(genType const& x, genType const& y, genType const& epsilon)
	{
		return abs(x - y) > epsilon;
	}

	template<typename genType>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR bool equal(genType const& x, genType const& y, int MaxULPs)
	{
		detail::float_t<genType> const a(x);
		detail::float_t<genType> const b(y);

		if(a.negative() != b.negative())
			return false;

		typename detail::float_t<genType>::int_type const DiffULPs = abs(a.i - b.i);
		return DiffULPs <= MaxULPs;
	}

	template<typename genType>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR bool notEqual(genType const& x, genType const& y, int ULPs)
	{
		return !equal(x, y, ULPs);
	}
}
