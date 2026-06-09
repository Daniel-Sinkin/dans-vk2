// src/dans/strings.hpp
#pragma once
// Externals
#include <dans/concepts.hpp>
#include <dans/development_markers.hpp>
// StdLib
#include <array>
#include <format>
#include <functional>
#include <string>
#include <string_view>
#include <vector>
//
namespace dans::str
{

inline constexpr std::array<char, 4> k_whitespace{' ', '\t', '\n', '\r'};
[[nodiscard]] constexpr auto is_whitespace(char c) noexcept
{
    const auto& w = k_whitespace;
    return c == w[0] or c == w[1] or c == w[2] or c == w[3];
}

[[nodiscard]] inline def split(std::string_view text, std::string_view delim)
    -> std::vector<std::string_view>
{
    std::vector<std::string_view> out{};
    if (delim.empty())
    {
        out.push_back(text);
        return out;
    }
    auto left = 0zu;
    while (true)
    {
        const auto pos = text.find(delim, left);
        if (pos == std::string_view::npos) break;
        out.push_back(text.substr(left, pos - left));
        left = pos + delim.size();
    }
    out.push_back(text.substr(left));
    return out;
}

static constexpr std::string_view k_default_sep{", "};

template <StringRange R>
[[nodiscard]] def join(R&& range, std::string_view sep = k_default_sep) -> std::string
{
    std::string out{};
    auto first = true;
    for (const std::string_view part : range)
    {
        if (not first) out += sep;
        out += part;
        first = false;
    }
    return out;
}

template <FormattableRange R>
[[nodiscard]] def join(R&& range, std::string_view sep = k_default_sep) -> std::string
{
    std::string out{};
    auto first = true;
    for (auto&& elem : range)
    {
        if (not first) out += sep;
        out += std::format("{}", elem);
        first = false;
    }
    return out;
}

template <input_range R, StringProjection<R> Proj>
[[nodiscard]] def join(R&& range, Proj proj, std::string_view sep = k_default_sep) -> std::string
{
    std::string out{};
    auto first = true;
    for (auto&& elem : range)
    {
        if (not first) out += sep;
        out += std::invoke(proj, elem);
        first = false;
    }
    return out;
}

namespace
{  // These are tests and examples on how to use them
using StringProj = decltype([](int) { return std::string{}; });
using StringViewProj = decltype([](int) { return std::string_view{}; });

static_assert(StringRange<std::vector<std::string>>);
static_assert(StringRange<std::vector<std::string_view>>);
static_assert(StringRange<std::vector<const char*>>);
static_assert(not StringRange<std::vector<int>>);
static_assert(FormattableRange<std::vector<int>>);
static_assert(StringProjection<StringProj, std::vector<int>>);
static_assert(StringProjection<StringViewProj, std::vector<int>>);
// clang-format on
}  // namespace
}  // namespace dans::str
