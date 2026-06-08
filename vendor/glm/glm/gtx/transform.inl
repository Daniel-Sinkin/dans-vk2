// vendor/glm/glm/gtx/transform.inl
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
namespace glm
{
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<4, 4, T, Q> translate(vec<3, T, Q> const& v)
	{
		return translate(mat<4, 4, T, Q>(static_cast<T>(1)), v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<4, 4, T, Q> rotate(T angle, vec<3, T, Q> const& v)
	{
		return rotate(mat<4, 4, T, Q>(static_cast<T>(1)), angle, v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<4, 4, T, Q> scale(vec<3, T, Q> const& v)
	{
		return scale(mat<4, 4, T, Q>(static_cast<T>(1)), v);
	}

}
