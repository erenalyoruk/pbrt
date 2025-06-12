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

  static PBRT_INLINE PBRT_CONSTEXPR auto zero() noexcept -> storage_type
  {
    return storage_type{};
  }
};

template <typename SimdType, typename ScalarType, usize N>
union simd_union {
  alignas(alignof(SimdType)) SimdType simd_data;
  alignas(alignof(SimdType)) std::array<ScalarType, N> scalar_data;

  PBRT_CONSTEXPR simd_union() noexcept : simd_data{simd_traits<ScalarType, N>::zero()}
  {
  }

  PBRT_CONSTEXPR simd_union(SimdType data) noexcept : simd_data{data}
  {
  }

  PBRT_CONSTEXPR simd_union(ScalarType scalar) noexcept
      : simd_data{simd_traits<ScalarType, N>::set1(scalar)}
  {
  }

  PBRT_CONSTEXPR simd_union(std::array<ScalarType, N> const &data) noexcept : scalar_data{data}
  {
  }

  constexpr auto operator[](usize index) noexcept -> ScalarType &
  {
    return scalar_data[index];
  }

  constexpr auto operator[](usize index) const noexcept -> ScalarType
  {
    return scalar_data[index];
  }
};

template <>
struct simd_traits<f32, 4>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{16};
  using simd_type = simde__m128;
  using storage_type = simd_union<simd_type, f32, 4>;

  static PBRT_INLINE PBRT_CONSTEXPR auto zero() noexcept -> simd_type
  {
    return _mm_setzero_ps();
  }

  static PBRT_INLINE PBRT_CONSTEXPR auto simd_set1(f32 value) noexcept -> simd_type
  {
    return _mm_set1_ps(value);
  }
};

template <>
struct simd_traits<f32, 8>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{32};
  using simd_type = simde__m256;
  using storage_type = simd_union<simd_type, f32, 8>;

  static PBRT_INLINE PBRT_CONSTEXPR auto zero() noexcept -> simd_type
  {
    return simde_mm256_setzero_ps();
  }

  static PBRT_INLINE PBRT_CONSTEXPR auto simd_set1(f32 value) noexcept -> simd_type
  {
    return _mm256_set1_ps(value);
  }
};

template <>
struct simd_traits<f64, 2>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{16};
  using simd_type = simde__m128d;
  using storage_type = simd_union<simd_type, f64, 2>;

  static PBRT_INLINE PBRT_CONSTEXPR auto zero() noexcept -> simd_type
  {
    return simde_mm_setzero_pd();
  }

  static PBRT_INLINE PBRT_CONSTEXPR auto set1(f64 value) noexcept -> simd_type
  {
    return _mm_set1_pd(value);
  }
};

template <>
struct simd_traits<f64, 4>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{32};
  using simd_type = simde__m256d;
  using storage_type = simd_union<simd_type, f64, 4>;

  static PBRT_INLINE PBRT_CONSTEXPR auto zero() noexcept -> simd_type
  {
    return simde_mm256_setzero_pd();
  }

  static PBRT_INLINE PBRT_CONSTEXPR auto set1(f64 value) noexcept -> simd_type
  {
    return _mm256_set1_pd(value);
  }
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
