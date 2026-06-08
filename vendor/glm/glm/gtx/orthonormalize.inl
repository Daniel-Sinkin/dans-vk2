// vendor/glm/glm/gtx/orthonormalize.inl
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
namespace glm
{
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> orthonormalize(mat<3, 3, T, Q> const& m)
	{
		mat<3, 3, T, Q> r = m;

		r[0] = normalize(r[0]);

		T d0 = dot(r[0], r[1]);
		r[1] -= r[0] * d0;
		r[1] = normalize(r[1]);

		T d1 = dot(r[1], r[2]);
		d0 = dot(r[0], r[2]);
		r[2] -= r[0] * d0 + r[1] * d1;
		r[2] = normalize(r[2]);

		return r;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<3, T, Q> orthonormalize(vec<3, T, Q> const& x, vec<3, T, Q> const& y)
	{
		return normalize(x - y * dot(y, x));
	}
}
