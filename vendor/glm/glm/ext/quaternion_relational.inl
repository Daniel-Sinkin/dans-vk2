// vendor/glm/glm/ext/quaternion_relational.inl
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
namespace glm
{
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<4, bool, Q> equal(qua<T, Q> const& x, qua<T, Q> const& y)
	{
		vec<4, bool, Q> Result;
		for(length_t i = 0; i < x.length(); ++i)
			Result[i] = x[i] == y[i];
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<4, bool, Q> equal(qua<T, Q> const& x, qua<T, Q> const& y, T epsilon)
	{
		vec<4, T, Q> v(x.x - y.x, x.y - y.y, x.z - y.z, x.w - y.w);
		return lessThan(abs(v), vec<4, T, Q>(epsilon));
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<4, bool, Q> notEqual(qua<T, Q> const& x, qua<T, Q> const& y)
	{
		vec<4, bool, Q> Result;
		for(length_t i = 0; i < x.length(); ++i)
			Result[i] = x[i] != y[i];
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<4, bool, Q> notEqual(qua<T, Q> const& x, qua<T, Q> const& y, T epsilon)
	{
		vec<4, T, Q> v(x.x - y.x, x.y - y.y, x.z - y.z, x.w - y.w);
		return greaterThanEqual(abs(v), vec<4, T, Q>(epsilon));
	}
}

