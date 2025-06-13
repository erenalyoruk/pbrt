#pragma once

#include "pbrt/api.hpp"
#include "pbrt/simd.hpp"
#include "pbrt/types.hpp"

#include <array>

namespace pbrt::math::simd
{
template <typename T, usize N>
struct vector_traits
{
  static constexpr bool kIsSimdCompatible{false};
  static constexpr usize kAlignment{alignof(T)};
  using storage_type = std::array<T, N>;

  static PBRT_INLINE PBRT_CONSTEXPR auto zero() noexcept -> storage_type
  {
    return storage_type{};
  }
};

template <typename S, typename T, usize N>
union vector_union {
  alignas(alignof(S)) S simd_data;
  alignas(alignof(S)) std::array<T, N> scalar_data;

  vector_union() noexcept : simd_data{vector_traits<T, N>::zero()}
  {
  }

  vector_union(S data) noexcept : simd_data{data}
  {
  }

  vector_union(T scalar) noexcept : simd_data{vector_traits<T, N>::set1(scalar)}
  {
  }

  vector_union(std::array<T, N> const &data) noexcept : scalar_data{data}
  {
  }

  PBRT_INLINE PBRT_CONSTEXPR auto operator[](usize index) noexcept -> T &
  {
    return scalar_data[index];
  }

  PBRT_INLINE PBRT_CONSTEXPR auto operator[](usize index) const noexcept -> T
  {
    return scalar_data[index];
  }
};

template <>
struct vector_traits<f32, 4>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{16};
  using simd_type = simde__m128;
  using storage_type = vector_union<simd_type, f32, 4>;

  static PBRT_INLINE auto zero() noexcept -> simd_type
  {
    return _mm_setzero_ps();
  }

  static PBRT_INLINE auto simd_set1(f32 value) noexcept -> simd_type
  {
    return _mm_set1_ps(value);
  }
};

template <>
struct vector_traits<f32, 8>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{32};
  using simd_type = simde__m256;
  using storage_type = vector_union<simd_type, f32, 8>;

  static PBRT_INLINE auto zero() noexcept -> simd_type
  {
    return simde_mm256_setzero_ps();
  }

  static PBRT_INLINE auto simd_set1(f32 value) noexcept -> simd_type
  {
    return _mm256_set1_ps(value);
  }
};

template <>
struct vector_traits<f64, 2>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{16};
  using simd_type = simde__m128d;
  using storage_type = vector_union<simd_type, f64, 2>;

  static PBRT_INLINE auto zero() noexcept -> simd_type
  {
    return simde_mm_setzero_pd();
  }

  static PBRT_INLINE auto set1(f64 value) noexcept -> simd_type
  {
    return _mm_set1_pd(value);
  }
};

template <>
struct vector_traits<f64, 4>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{32};
  using simd_type = simde__m256d;
  using storage_type = vector_union<simd_type, f64, 4>;

  static PBRT_INLINE auto zero() noexcept -> simd_type
  {
    return simde_mm256_setzero_pd();
  }

  static PBRT_INLINE auto set1(f64 value) noexcept -> simd_type
  {
    return _mm256_set1_pd(value);
  }
};

template <typename T, usize N>
concept vector_simd_compatible = vector_traits<T, N>::kIsSimdCompatible;

template <typename T, usize N>
concept vector_sse_compatible =
    vector_simd_compatible<T, N> && vector_traits<T, N>::kAlignment == 16;

template <typename T, usize N>
concept vector_avx_compatible =
    vector_simd_compatible<T, N> && vector_traits<T, N>::kAlignment == 32;

template <typename T, usize N>
using vector_storage_t = typename vector_traits<T, N>::storage_type;

template <typename T, usize N>
  requires vector_simd_compatible<T, N>
using vector_simd_t = typename vector_traits<T, N>::simd_type;

template <typename T, usize N>
constexpr usize kVectorAlignmentV = vector_traits<T, N>::kAlignment;
} // namespace pbrt::math::simd
