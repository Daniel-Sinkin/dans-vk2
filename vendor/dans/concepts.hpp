// include/dans/concepts.hpp
// https://github.com/Daniel-Sinkin/dans-core v0.1

#ifndef DANS_CORE_INCLUDE_DANS_CONCEPTS_HPP
#define DANS_CORE_INCLUDE_DANS_CONCEPTS_HPP
#pragma once

#include <concepts>
#include <string_view>

namespace dans
{
using std::ranges::input_range;
using std::ranges::range_value_t;

template <typename T>
concept BoolLike = std::convertible_to<T, bool>;

template <typename T>
concept CharLike = std::convertible_to<T, char>;

template <typename T>
concept StringLike = std::convertible_to<T, std::string_view>;

template <typename T>
using ValueOf = std::ranges::range_value_t<T>;
template <typename T>
using RefOf = std::ranges::range_reference_t<T>;

template <typename R>
concept StringRange = input_range<R> and StringLike<ValueOf<R>>;

template <typename R>
concept FormattableRange = input_range<R> and not StringRange<R> and CharLike<ValueOf<R>>;

template <typename Proj, typename R>
concept StringProjection =
    std::invocable<Proj&, RefOf<R>>
    and std::convertible_to<std::invoke_result_t<Proj&, RefOf<R>>, std::string_view>;

template <typename T, typename S>
concept Comparable = requires(T t, S s) {
    { t == s } -> BoolLike;
};

}  // namespace dans

#endif  // DANS_CORE_INCLUDE_DANS_CONCEPTS_HPP
