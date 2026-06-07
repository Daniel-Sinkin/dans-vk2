#pragma once

#include <concepts>

namespace dans::logic
{
template <std::convertible_to<bool> A, std::convertible_to<bool> B>
[[nodiscard]] auto implies(bool a, bool b) noexcept -> bool
{
    return (not a) or b;
}
}  // namespace dans::logic
