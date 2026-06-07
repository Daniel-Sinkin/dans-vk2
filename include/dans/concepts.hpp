// include/dans/concepts.hpp
// https://github.com/Daniel-Sinkin/dans-core v0.1

#ifndef DANS_CORE_INCLUDE_DANS_CONCEPTS_HPP
#define DANS_CORE_INCLUDE_DANS_CONCEPTS_HPP
#pragma once

#include <concepts>
#include <string_view>

namespace dans
{
template <typename T>
concept BoolLike = std::convertible_to<T, bool>;

template <typename T>
concept CharLike = std::convertible_to<T, char>;

template <typename T>
concept StringLike = std::convertible_to<T, std::string_view>;
}  // namespace dans

#endif  // DANS_CORE_INCLUDE_DANS_CONCEPTS_HPP
