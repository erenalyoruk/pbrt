#pragma once

#include "pbrt/export.hpp"
#include "pbrt/inline.hpp"

namespace pbrt::math
{
template <typename T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr T safe_divide(T a, T b) noexcept
{
  return (b != T{0}) ? (a / b) : T{0};
}
} // namespace pbrt::math
