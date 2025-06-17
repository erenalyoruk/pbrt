#pragma once

#include "pbrt/api.hpp"
#include "pbrt/math/traits.hpp"
#include "pbrt/simd.hpp"
#include "pbrt/types.hpp"

#include <array>

namespace pbrt::math
{
template <scalar_type T, usize R, usize C>
struct matrix_traits
{
  static constexpr bool kIsSimdCompatible{false};
  static constexpr usize kAlignment{alignof(T)};
  using storage_type = std::array<std::array<T, C>, R>;

  static PBRT_INLINE PBRT_CONSTEXPR auto zero() noexcept -> storage_type
  {
    return {};
  }
};

template <typename S, typename T, usize R, usize C>
union matrix_union {
  alignas(alignof(S)) std::array<S, R> simd_data;
  alignas(alignof(S)) std::array<std::array<T, C>, R> scalar_data;

  PBRT_CONSTEXPR matrix_union() noexcept : scalar_data{}
  {
  }

  matrix_union(std::array<S, R> const &data) noexcept : simd_data{data}
  {
  }

  PBRT_CONSTEXPR matrix_union(std::array<std::array<T, C>, R> const &data) noexcept
      : scalar_data{data}
  {
  }

  PBRT_INLINE PBRT_CONSTEXPR auto operator()(usize row, usize col) noexcept -> T &
  {
    return scalar_data[row][col];
  }

  PBRT_INLINE PBRT_CONSTEXPR auto operator()(usize row, usize col) const noexcept -> T
  {
    return scalar_data[row][col];
  }

  PBRT_INLINE auto operator()(usize row) noexcept -> S &
  {
    return simd_data[row];
  }

  PBRT_NODISCARD PBRT_INLINE auto row(usize row) noexcept -> S &
  {
    return simd_data[row];
  }

  PBRT_NODISCARD PBRT_INLINE auto row(usize row) const noexcept -> S
  {
    return simd_data[row];
  }
};

template <>
struct matrix_traits<f32, 4, 4>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{16};
  using simd_type = simde__m128;
  using storage_type = matrix_union<simd_type, f32, 4, 4>;

  static PBRT_INLINE auto zero() noexcept -> storage_type
  {
    return {{simde_mm_setzero_ps(), simde_mm_setzero_ps(), simde_mm_setzero_ps(),
             simde_mm_setzero_ps()}};
  }

  static PBRT_INLINE auto identity() noexcept -> storage_type
  {
    return {{simde_mm_setr_ps(1.0F, 0.0F, 0.0F, 0.0F), simde_mm_setr_ps(0.0F, 1.0F, 0.0F, 0.0F),
             simde_mm_setr_ps(0.0F, 0.0F, 1.0F, 0.0F), simde_mm_setr_ps(0.0F, 0.0F, 0.0F, 1.0F)}};
  }

  static PBRT_INLINE auto set1(f32 value) noexcept -> storage_type
  {
    auto const v{simde_mm_set1_ps(value)};
    return {{v, v, v, v}};
  }
};

template <>
struct matrix_traits<f32, 3, 3>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{16};
  using simd_type = simde__m128;
  using storage_type = matrix_union<simd_type, f32, 3, 3>;

  static PBRT_INLINE auto zero() noexcept -> storage_type
  {
    return {{simde_mm_setzero_ps(), simde_mm_setzero_ps(), simde_mm_setzero_ps()}};
  }

  static PBRT_INLINE auto identity() noexcept -> storage_type
  {
    return {{simde_mm_setr_ps(1.0F, 0.0F, 0.0F, 0.0F), simde_mm_setr_ps(0.0F, 1.0F, 0.0F, 0.0F),
             simde_mm_setr_ps(0.0F, 0.0F, 1.0F, 0.0F)}};
  }

  static PBRT_INLINE auto set1(f32 value) noexcept -> storage_type
  {
    auto const v{simde_mm_set1_ps(value)};
    return {{v, v, v}};
  }
};

template <>
struct matrix_traits<f64, 3, 3>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{32};
  using simd_type = simde__m256d;
  using storage_type = matrix_union<simd_type, f64, 3, 3>;

  static PBRT_INLINE auto zero() noexcept -> storage_type
  {
    return {{simde_mm256_setzero_pd(), simde_mm256_setzero_pd(), simde_mm256_setzero_pd()}};
  }

  static PBRT_INLINE auto identity() noexcept -> storage_type
  {
    return {{simde_mm256_setr_pd(1.0, 0.0, 0.0, 0.0), simde_mm256_setr_pd(0.0, 1.0, 0.0, 0.0),
             simde_mm256_setr_pd(0.0, 0.0, 1.0, 0.0)}};
  }

  static PBRT_INLINE auto set1(f64 value) noexcept -> storage_type
  {
    auto const v{simde_mm256_set1_pd(value)};
    return {{v, v, v}};
  }
};

template <>
struct matrix_traits<f64, 4, 4>
{
  static constexpr bool kIsSimdCompatible{true};
  static constexpr usize kAlignment{32};
  using simd_type = simde__m256d;
  using storage_type = matrix_union<simd_type, f64, 4, 4>;

  static PBRT_INLINE auto zero() noexcept -> storage_type
  {
    return {{simde_mm256_setzero_pd(), simde_mm256_setzero_pd(), simde_mm256_setzero_pd(),
             simde_mm256_setzero_pd()}};
  }

  static PBRT_INLINE auto identity() noexcept -> storage_type
  {
    return {{simde_mm256_setr_pd(1.0, 0.0, 0.0, 0.0), simde_mm256_setr_pd(0.0, 1.0, 0.0, 0.0),
             simde_mm256_setr_pd(0.0, 0.0, 1.0, 0.0), simde_mm256_setr_pd(0.0, 0.0, 0.0, 1.0)}};
  }

  static PBRT_INLINE auto set1(f64 value) noexcept -> storage_type
  {
    auto const v{simde_mm256_set1_pd(value)};
    return {{v, v, v, v}};
  }
};

template <typename T, usize R, usize C>
concept matrix_simd_compatible = matrix_traits<T, R, C>::kIsSimdCompatible;

template <typename T, usize R, usize C>
using matrix_storage_t = typename matrix_traits<T, R, C>::storage_type;

template <typename T, usize R, usize C>
  requires matrix_simd_compatible<T, R, C>
using matrix_simd_t = typename matrix_traits<T, R, C>::simd_type;

template <typename T, usize R, usize C>
constexpr usize kMatrixAlignmentV = matrix_traits<T, R, C>::kAlignment;
} // namespace pbrt::math
