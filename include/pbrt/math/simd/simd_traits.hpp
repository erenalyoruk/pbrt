#pragma once

#include "pbrt/api.hpp"
#include "pbrt/simd.hpp"
#include "pbrt/types.hpp"

#include <array>

namespace pbrt::math::simd
{
template <typename T, usize N>
struct simd_traits
{
  static constexpr bool kIsSimdCompatible{false};
  static constexpr usize kAlignment{alignof(T)};
  using storage_type = std::array<T, N>;
};

template <>
struct simd_traits<f32, 4>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{16};
  using simd_type = __m128;

  union storage_type {
    alignas(kAlignment) simd_type simd_data;
    alignas(kAlignment) std::array<f32, 4> scalar_data;

    PBRT_CONSTEXPR storage_type() noexcept : simd_data{simde_mm_setzero_ps()}
    {
    }

    PBRT_CONSTEXPR explicit storage_type(simd_type data) noexcept : simd_data{data}
    {
    }
  };
};

template <>
struct simd_traits<f32, 8>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{32};
  using simd_type = __m256;

  union storage_type {
    alignas(kAlignment) simd_type simd_data;
    alignas(kAlignment) std::array<f32, 8> scalar_data;

    PBRT_CONSTEXPR storage_type() noexcept : simd_data{simde_mm256_setzero_ps()}
    {
    }

    PBRT_CONSTEXPR explicit storage_type(simd_type data) noexcept : simd_data{data}
    {
    }
  };
};

template <>
struct simd_traits<f64, 2>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{16};
  using simd_type = __m128d;

  union storage_type {
    alignas(kAlignment) simd_type simd_data;
    alignas(kAlignment) std::array<f64, 2> scalar_data;

    PBRT_CONSTEXPR storage_type() noexcept : simd_data{simde_mm_setzero_pd()}
    {
    }

    PBRT_CONSTEXPR explicit storage_type(simd_type data) noexcept : simd_data{data}
    {
    }
  };
};

template <>
struct simd_traits<f64, 4>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{32};
  using simd_type = __m256d;

  union storage_type {
    alignas(kAlignment) simd_type simd_data;
    alignas(kAlignment) std::array<f64, 4> scalar_data;

    PBRT_CONSTEXPR storage_type() noexcept : simd_data{simde_mm256_setzero_pd()}
    {
    }

    PBRT_CONSTEXPR explicit storage_type(simd_type data) noexcept : simd_data{data}
    {
    }
  };
};

template <typename T, usize N>
concept simd_compatible = simd_traits<T, N>::kIsSimdCompatible;

template <typename T, usize N>
concept sse_compatible = simd_compatible<T, N> && simd_traits<T, N>::kAlignment == 16;

template <typename T, usize N>
concept avx_compatible = simd_compatible<T, N> && simd_traits<T, N>::kAlignment == 32;

template <typename T, usize N>
using storage_t = typename simd_traits<T, N>::storage_type;

template <typename T, usize N>
  requires simd_compatible<T, N>
using simd_t = typename simd_traits<T, N>::simd_type;

template <typename T, usize N>
constexpr usize kAlignmentV = simd_traits<T, N>::kAlignment;
} // namespace pbrt::math::simd
