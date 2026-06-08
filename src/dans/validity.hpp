// src/dans/validity.hpp
#pragma once
// Internals
#include <dans/dev.hpp>
// Externals
#include <dans/development_markers.hpp>
#include <dans/types.hpp>
// StdLib
#include <concepts>
#include <string>
#include <type_traits>
#include <utility>
//
namespace dans
{
// A validity enum is u8-backed with 0 reserved as the single valid case. Each
// config type pairs with one, returned by a free validity(const Cfg&).
template <typename T>
concept Validity = std::is_enum_v<T> and std::same_as<std::underlying_type_t<T>, u8>;

// A config is validatable when a free validity(const Cfg&) returning a Validity
// enum is reachable by ADL.
template <typename Cfg>
concept Validatable = requires(const Cfg& cfg) {
    { validity(cfg) } -> Validity;
};

// The one is_valid: a config is valid iff its validity enum is the 0 case.
template <Validatable Cfg>
[[nodiscard]] def is_valid(const Cfg& cfg) -> bool
{
    return std::to_underlying(validity(cfg)) == 0;
}

// Name-wrapped label for a validity enum. Lives here (a non-member, so member
// to_string overloads can't hide it) and reaches the enum's own to_string by
// ADL; DANS_CHECK_VALID routes through it so the macro works inside any class.
template <Validity V>
[[nodiscard]] def validity_label(V v) -> std::string
{
    return to_string(v, true);
}
}  // namespace dans

// Panic (reporting the failing validity case) unless cfg is valid. validity is
// found by ADL, so expand this inside the cfg type's namespace or with a
// using-directive for it in scope.
#define DANS_CHECK_VALID(cfg)                                                                      \
    do                                                                                             \
    {                                                                                              \
        if (const auto dans_validity_ = validity(cfg); ::std::to_underlying(dans_validity_) != 0)  \
        {                                                                                          \
            DANS_PANIC_FMT("invalid config: {}", ::dans::validity_label(dans_validity_));          \
        }                                                                                          \
    } while (false)
