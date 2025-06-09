#pragma once

#include "pbrt/api.hpp"
#include "pbrt/math/traits.hpp"

#include <limits>
#include <numbers>

namespace pbrt::math
{
template <FloatingPoint T>
PBRT_CONSTEXPR T kEpsilonScaling{16};

template <FloatingPoint T>
PBRT_CONSTEXPR T kPiV{std::numbers::pi_v<T>};

template <FloatingPoint T>
PBRT_CONSTEXPR T kInvPiV{std::numbers::inv_pi_v<T>};

template <FloatingPoint T>
PBRT_CONSTEXPR T kSqrt2V{std::numbers::sqrt2_v<T>};

template <FloatingPoint T>
PBRT_CONSTEXPR T kInvSqrt2V{T{1} / kSqrt2V<T>};

template <FloatingPoint T>
PBRT_CONSTEXPR T kEulerV{std::numbers::e_v<T>};

template <FloatingPoint T>
PBRT_CONSTEXPR T kEpsilonV{std::numeric_limits<T>::epsilon() * kEpsilonScaling<T>};

template <FloatingPoint T>
PBRT_CONSTEXPR T kShadowEpsilonV{T{0.0001}};
} // namespace pbrt::math
