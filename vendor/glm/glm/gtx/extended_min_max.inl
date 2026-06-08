// vendor/glm/glm/gtx/extended_min_max.inl
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
namespace glm
{
	template<typename T>
	GLM_FUNC_QUALIFIER T min(
		T const& x,
		T const& y,
		T const& z)
	{
		return glm::min(glm::min(x, y), z);
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> min
	(
		C<T> const& x,
		typename C<T>::T const& y,
		typename C<T>::T const& z
	)
	{
		return glm::min(glm::min(x, y), z);
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> min
	(
		C<T> const& x,
		C<T> const& y,
		C<T> const& z
	)
	{
		return glm::min(glm::min(x, y), z);
	}

	template<typename T>
	GLM_FUNC_QUALIFIER T min
	(
		T const& x,
		T const& y,
		T const& z,
		T const& w
	)
	{
		return glm::min(glm::min(x, y), glm::min(z, w));
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> min
	(
		C<T> const& x,
		typename C<T>::T const& y,
		typename C<T>::T const& z,
		typename C<T>::T const& w
	)
	{
		return glm::min(glm::min(x, y), glm::min(z, w));
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> min
	(
		C<T> const& x,
		C<T> const& y,
		C<T> const& z,
		C<T> const& w
	)
	{
		return glm::min(glm::min(x, y), glm::min(z, w));
	}

	template<typename T>
	GLM_FUNC_QUALIFIER T max(
		T const& x,
		T const& y,
		T const& z)
	{
		return glm::max(glm::max(x, y), z);
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> max
	(
		C<T> const& x,
		typename C<T>::T const& y,
		typename C<T>::T const& z
	)
	{
		return glm::max(glm::max(x, y), z);
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> max
	(
		C<T> const& x,
		C<T> const& y,
		C<T> const& z
	)
	{
		return glm::max(glm::max(x, y), z);
	}

	template<typename T>
	GLM_FUNC_QUALIFIER T max
	(
		T const& x,
		T const& y,
		T const& z,
		T const& w
	)
	{
		return glm::max(glm::max(x, y), glm::max(z, w));
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> max
	(
		C<T> const& x,
		typename C<T>::T const& y,
		typename C<T>::T const& z,
		typename C<T>::T const& w
	)
	{
		return glm::max(glm::max(x, y), glm::max(z, w));
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> max
	(
		C<T> const& x,
		C<T> const& y,
		C<T> const& z,
		C<T> const& w
	)
	{
		return glm::max(glm::max(x, y), glm::max(z, w));
	}
}
