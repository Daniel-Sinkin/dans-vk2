// src/dans/format.hpp
#pragma once
// Externals
#include <dans/development_markers.hpp>
// StdLib
#include <concepts>
#include <format>       // IWYU pragma: keep
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
//
namespace dans
{
// Convertible to std::string_view, not std::string: a to_string returning a
// std::string_view (the natural enum-name shape) is accepted, even though
// std::string_view -> std::string is explicit. to_string_any builds the string.
template <typename T>
concept has_free_to_string = requires(const T& value) {
    { to_string(value) } -> std::convertible_to<std::string_view>;
};

template <typename T>
concept has_member_to_string = requires(const T& value) {
    { value.to_string() } -> std::convertible_to<std::string_view>;
};

template <typename T>
concept has_to_string = has_free_to_string<T> or has_member_to_string<T>;

namespace detail
{
template <has_to_string T>
[[nodiscard]] auto to_string_any(const T& value) -> std::string
{
    if constexpr (has_free_to_string<T>)
    {
        return std::string{to_string(value)};
    }
    else
    {
        return std::string{value.to_string()};
    }
}

template <typename T>
concept formattable_directly = std::formattable<std::remove_cvref_t<T>, char>;

// Coerce one argument into something std::format accepts: a directly formattable
// value passes through unchanged; otherwise it is rendered via to_string; a bare
// enum with neither a formatter nor a to_string falls back to its underlying
// integer value.
template <typename T>
[[nodiscard]] def to_format_arg(T&& value) -> decltype(auto)
{
    using U = std::remove_cvref_t<T>;
    if constexpr (formattable_directly<T>)
    {
        return std::forward<T>(value);
    }
    else if constexpr (has_to_string<U>)
    {
        return to_string_any(value);
    }
    else if constexpr (std::is_enum_v<U>)
    {
        return std::to_underlying(value);
    }
    else
    {
        static_assert(
            has_to_string<U>,
            "format/panic argument is neither std::formattable, to_string-able, nor an enum: give "
            "it a std::formatter (see DANS_FORMAT_WITH_TO_STRING), a free to_string(const T&), or "
            "a const member T::to_string()"
        );
        return std::string{};  // unreachable; keeps format_arg_t sane so the assert is the only
                               // error
    }
}

// Render type std::format sees for an argument: the argument's own type when it
// is directly formattable (kept exactly so std::format's own deduction matches),
// otherwise the by-value result of to_format_arg (a std::string from to_string,
// or the underlying integer of a bare enum).
template <typename T>
using format_arg_t =
    std::conditional_t<formattable_directly<T>, T, decltype(to_format_arg(std::declval<T>()))>;
}  // namespace detail
}  // namespace dans

#define DANS_FORMAT_WITH_TO_STRING(T)                                                              \
    static_assert(                                                                                 \
        ::dans::has_to_string<T>,                                                                  \
        "DANS_FORMAT_WITH_TO_STRING(" #T "): need a free `to_string(const " #T                     \
        "&)` or a const member `" #T "::to_string()`, returning something "                        \
        "convertible to std::string_view (std::string, std::string_view, const char*)"             \
    );                                                                                             \
    template <>                                                                                    \
    struct std::formatter<T> : std::formatter<std::string>                                         \
    {                                                                                              \
        auto format(const T& value, std::format_context& ctx) const                                \
            -> std::format_context::iterator                                                       \
        {                                                                                          \
            return std::formatter<std::string>::format(::dans::detail::to_string_any(value), ctx); \
        }                                                                                          \
    };
