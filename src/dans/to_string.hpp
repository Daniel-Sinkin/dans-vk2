// src/dans/to_string.hpp
#pragma once
// Externals
#include <dans/development_markers.hpp>
// StdLib
#include <format>
#include <string>
#include <string_view>
//
namespace dans
{
// Wrap a to_string body as `TypeName(body)` when include_name is set, otherwise
// return the bare body. Every to_string in the project routes its include_name
// flag through here so the convention lives in exactly one place.
[[nodiscard]] inline def
with_name(std::string_view type_name, std::string_view body, bool include_name) -> std::string
{
    return include_name ? std::format("{}({})", type_name, body) : std::string{body};
}
}  // namespace dans
