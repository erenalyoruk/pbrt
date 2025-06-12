#pragma once

#include "pbrt/api.hpp"
#include "pbrt/math/constants.hpp"
#include "pbrt/math/traits.hpp"

#include <algorithm>
#include <cmath>

namespace pbrt::math
{
template <scalar_type T>
PBRT_NODISCARD PBRT_API PBRT_FORCE_INLINE PBRT_CONSTEXPR auto min(T lhs, T rhs) noexcept -> T
{
  return std::min(lhs, rhs);
}

template <scalar_type T>
PBRT_NODISCARD PBRT_API PBRT_FORCE_INLINE PBRT_CONSTEXPR auto max(T lhs, T rhs) noexcept -> T
{
  return std::max(lhs, rhs);
}

template <scalar_type T>
PBRT_NODISCARD PBRT_API PBRT_FORCE_INLINE PBRT_CONSTEXPR auto clamp(T value, T min_value,
                                                                    T max_value) noexcept -> T
{
  return std::clamp(value, min_value, max_value);
}

template <floating_point T>
PBRT_NODISCARD PBRT_API PBRT_FORCE_INLINE PBRT_CONSTEXPR auto lerp(T start, T end, T time) noexcept
    -> T
{
  return std::lerp(start, end, time);
}

template <floating_point T>
PBRT_NODISCARD PBRT_API PBRT_FORCE_INLINE PBRT_CONSTEXPR auto radians(T degrees) noexcept -> T
{
  return degrees * (kPiV<T> / T{180});
}

template <floating_point T>
PBRT_NODISCARD PBRT_API PBRT_FORCE_INLINE PBRT_CONSTEXPR auto degrees(T radians) noexcept -> T
{
  return radians * (T{180} / kPiV<T>);
}

template <floating_point T>
PBRT_NODISCARD PBRT_API PBRT_FORCE_INLINE PBRT_CONSTEXPR auto safe_sqrt(T value) noexcept -> T
{
  return std::sqrt(max(T{0}, value));
}

template <floating_point T>
PBRT_NODISCARD PBRT_API PBRT_FORCE_INLINE PBRT_CONSTEXPR auto fast_inv_sqrt(T value) noexcept -> T
{
  if constexpr (std::same_as<T, f32>)
  {
    union {
      f32 f;
      u32 i;
    } conv = {.f = value};
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f *= T{1.5} - T{0.5} * value * conv.f * conv.f;
    return conv.f;
  }
  else if constexpr (std::same_as<T, f64>)
  {
    union {
      f64 f;
      u64 i;
    } conv = {.f = value};
    conv.i = 0x5fe6ec85e7de30da - (conv.i >> 1);
    conv.f *= T{1.5} - T{0.5} * value * conv.f * conv.f;
    return conv.f;
  }
}
} // namespace pbrt::math
