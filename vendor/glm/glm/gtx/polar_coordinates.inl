// vendor/glm/glm/gtx/polar_coordinates.inl
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
namespace glm
{
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<3, T, Q> polar
	(
		vec<3, T, Q> const& euclidean
	)
	{
		T const Length(length(euclidean));
		vec<3, T, Q> const tmp(euclidean / Length);
		T const xz_dist(sqrt(tmp.x * tmp.x + tmp.z * tmp.z));

		return vec<3, T, Q>(
			asin(tmp.y),
			atan(tmp.x, tmp.z),
			xz_dist);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<3, T, Q> euclidean
	(
		vec<2, T, Q> const& polar
	)
	{
		T const latitude(polar.x);
		T const longitude(polar.y);

		return vec<3, T, Q>(
			cos(latitude) * sin(longitude),
			sin(latitude),
			cos(latitude) * cos(longitude));
	}

}
