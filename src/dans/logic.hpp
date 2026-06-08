#pragma once

#include "dans/concepts.hpp"

namespace dans
{
[[nodiscard]] auto implies(BoolLike auto a, BoolLike auto b) noexcept -> bool
{
    return (not a) or b;
}

}  // namespace dans
