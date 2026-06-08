// vendor/glm/glm/gtx/log_base.inl
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
namespace glm
{
	template<typename genType>
	GLM_FUNC_QUALIFIER genType log(genType const& x, genType const& base)
	{
		return glm::log(x) / glm::log(base);
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> log(vec<L, T, Q> const& x, vec<L, T, Q> const& base)
	{
		return glm::log(x) / glm::log(base);
	}
}
