#pragma once

#include "pbrt/assert.hpp"
#include "pbrt/math/constants.hpp"
#include "pbrt/math/simd/simd_traits.hpp"
#include "pbrt/math/traits.hpp"
#include "pbrt/math/utility.hpp"
#include "pbrt/types.hpp"

#include <array>
#include <concepts>

namespace pbrt::math
{
template <scalar_type T, usize N>
class vector
{
public:
  using value_type = T;
  using size_type = usize;
  using reference = value_type &;
  using const_reference = value_type const &;
  using pointer = value_type *;
  using const_pointer = value_type const *;
  using iterator = typename std::array<T, N>::iterator;
  using const_iterator = typename std::array<T, N>::const_iterator;
  using reverse_iterator = typename std::array<T, N>::reverse_iterator;
  using const_reverse_iterator = typename std::array<T, N>::const_reverse_iterator;

  static constexpr bool kIsSimdCompatible{simd::simd_compatible<T, N>};

  PBRT_INLINE PBRT_CONSTEXPR vector() noexcept = default;

  PBRT_INLINE PBRT_CONSTEXPR explicit vector(T scalar) noexcept
  {
    if constexpr (kIsSimdCompatible)
    {
      if constexpr (std::same_as<T, f32> && N == 4)
      {
        data_.simd_data = simde_mm_set1_ps(scalar);
      }
      else if constexpr (std::same_as<T, f32> && N == 8)
      {
        data_.simd_data = simde_mm256_set1_ps(scalar);
      }
      else if constexpr (std::same_as<T, f64> && N == 2)
      {
        data_.simd_data = simde_mm_set1_pd(scalar);
      }
      else if constexpr (std::same_as<T, f64> && N == 4)
      {
        data_.simd_data = simde_mm256_set1_pd(scalar);
      }
      else
      {
        data_.scalar_data.fill(scalar);
      }
    }
    else
    {
      data_.fill(scalar);
    }
  }

  template <typename... Args>
    requires(sizeof...(Args) == N && (std::convertible_to<Args, T> && ...))
  PBRT_INLINE PBRT_CONSTEXPR explicit vector(Args &&...args) noexcept
  {
    if constexpr (kIsSimdCompatible)
    {
      if constexpr (std::same_as<T, f32> && N == 4)
      {
        data_.simd_data = simde_mm_setr_ps(static_cast<T>(std::forward<Args>(args))...);
      }
      else
      {
        data_.scalar_data = {static_cast<T>(std::forward<Args>(args))...};
      }
    }
    else
    {
      data_ = {static_cast<T>(std::forward<Args>(args))...};
    }
  }

  template <typename simd_type>
    requires(kIsSimdCompatible && std::same_as<simd_type, simd::simd_t<T, N>>)
  PBRT_INLINE PBRT_CONSTEXPR explicit vector(simd_type simd_data) noexcept : data_{simd_data}
  {
  }

  PBRT_INLINE PBRT_CONSTEXPR auto operator+=(vector const &other) noexcept -> vector &
  {
    if constexpr (kIsSimdCompatible)
    {
      if constexpr (std::same_as<T, f32> && N == 4)
      {
        data_.simd_data = simde_mm_add_ps(data_.simd_data, other.simd());
      }
      else if constexpr (std::same_as<T, f32> && N == 8)
      {
        data_.simd_data = simde_mm256_add_ps(data_.simd_data, other.simd());
      }
      else if constexpr (std::same_as<T, f64> && N == 2)
      {
        data_.simd_data = simde_mm_add_pd(data_.simd_data, other.simd());
      }
      else if constexpr (std::same_as<T, f64> && N == 4)
      {
        data_.simd_data = simde_mm256_add_pd(data_.simd_data, other.simd());
      }
    }
    else
    {
      for (size_type i{0}; i < size(); ++i)
      {
        (*this)[i] += other[i];
      }
    }

    return *this;
  }

  PBRT_INLINE PBRT_CONSTEXPR auto operator-=(vector const &other) noexcept -> vector &
  {
    if constexpr (kIsSimdCompatible)
    {
      if constexpr (std::same_as<T, f32> && N == 4)
      {
        data_.simd_data = simde_mm_sub_ps(data_.simd_data, other.simd());
      }
      else if constexpr (std::same_as<T, f32> && N == 8)
      {
        data_.simd_data = simde_mm256_sub_ps(data_.simd_data, other.simd());
      }
      else if constexpr (std::same_as<T, f64> && N == 2)
      {
        data_.simd_data = simde_mm_sub_pd(data_.simd_data, other.simd());
      }
      else if constexpr (std::same_as<T, f64> && N == 4)
      {
        data_.simd_data = simde_mm256_sub_pd(data_.simd_data, other.simd());
      }
    }
    else
    {
      for (size_type i{0}; i < size(); ++i)
      {
        (*this)[i] -= other[i];
      }
    }

    return *this;
  }

  PBRT_INLINE PBRT_CONSTEXPR auto operator*=(T scalar) noexcept -> vector &
  {
    if constexpr (kIsSimdCompatible)
    {
      if constexpr (std::same_as<T, f32> && N == 4)
      {
        data_.simd_data = simde_mm_mul_ps(data_.simd_data, simde_mm_set1_ps(scalar));
      }
      else if constexpr (std::same_as<T, f32> && N == 8)
      {
        data_.simd_data = simde_mm256_mul_ps(data_.simd_data, simde_mm256_set1_ps(scalar));
      }
      else if constexpr (std::same_as<T, f64> && N == 2)
      {
        data_.simd_data = simde_mm_mul_pd(data_.simd_data, simde_mm_set1_pd(scalar));
      }
      else if constexpr (std::same_as<T, f64> && N == 4)
      {
        data_.simd_data = simde_mm256_mul_pd(data_.simd_data, simde_mm256_set1_pd(scalar));
      }
    }
    else
    {
      for (size_type i{0}; i < size(); ++i)
      {
        (*this)[i] *= scalar;
      }
    }

    return *this;
  }

  PBRT_INLINE PBRT_CONSTEXPR auto operator*=(vector const &other) noexcept -> vector &
  {
    if constexpr (kIsSimdCompatible)
    {
      if constexpr (std::same_as<T, f32> && N == 4)
      {
        data_.simd_data = simde_mm_mul_ps(data_.simd_data, other.simd());
      }
      else if constexpr (std::same_as<T, f32> && N == 8)
      {
        data_.simd_data = simde_mm256_mul_ps(data_.simd_data, other.simd());
      }
      else if constexpr (std::same_as<T, f64> && N == 2)
      {
        data_.simd_data = simde_mm_mul_pd(data_.simd_data, other.simd());
      }
      else if constexpr (std::same_as<T, f64> && N == 4)
      {
        data_.simd_data = simde_mm256_mul_pd(data_.simd_data, other.simd());
      }
    }
    else
    {
      for (size_type i{0}; i < size(); ++i)
      {
        (*this)[i] *= other[i];
      }
    }

    return *this;
  }

  PBRT_INLINE PBRT_CONSTEXPR auto operator/=(T scalar) noexcept -> vector &
  {
    if constexpr (kIsSimdCompatible)
    {
      if constexpr (std::same_as<T, f32> && N == 4)
      {
        data_.simd_data = simde_mm_div_ps(data_.simd_data, simde_mm_set1_ps(scalar));
      }
      else if constexpr (std::same_as<T, f32> && N == 8)
      {
        data_.simd_data = simde_mm256_div_ps(data_.simd_data, simde_mm256_set1_ps(scalar));
      }
      else if constexpr (std::same_as<T, f64> && N == 2)
      {
        data_.simd_data = simde_mm_div_pd(data_.simd_data, simde_mm_set1_pd(scalar));
      }
      else if constexpr (std::same_as<T, f64> && N == 4)
      {
        data_.simd_data = simde_mm256_div_pd(data_.simd_data, simde_mm256_set1_pd(scalar));
      }
    }
    else
    {
      for (size_type i{0}; i < size(); ++i)
      {
        (*this)[i] /= scalar;
      }
    }

    return *this;
  }

  PBRT_INLINE PBRT_CONSTEXPR auto operator/=(vector const &other) noexcept -> vector &
  {
    if constexpr (kIsSimdCompatible)
    {
      if constexpr (std::same_as<T, f32> && N == 4)
      {
        data_.simd_data = simde_mm_div_ps(data_.simd_data, other.data_.simd_data);
      }
      else if constexpr (std::same_as<T, f32> && N == 8)
      {
        data_.simd_data = simde_mm256_div_ps(data_.simd_data, other.data_.simd_data);
      }
      else if constexpr (std::same_as<T, f64> && N == 2)
      {
        data_.simd_data = simde_mm_div_pd(data_.simd_data, other.data_.simd_data);
      }
      else if constexpr (std::same_as<T, f64> && N == 4)
      {
        data_.simd_data = simde_mm256_div_pd(data_.simd_data, other.data_.simd_data);
      }
    }
    else
    {
      for (size_type i{0}; i < size(); ++i)
      {
        (*this)[i] /= other[i];
      }
    }

    return *this;
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto operator==(vector const &other) const noexcept
      -> bool
  {
    if constexpr (kIsSimdCompatible)
    {
      if constexpr (std::same_as<T, f32> && N == 4)
      {
        return simde_mm_movemask_ps(simde_mm_cmpeq_ps(data_.simd_data, other.data_.simd_data)) ==
               0xf;
      }
      else if constexpr (std::same_as<T, f32> && N == 8)
      {
        return simde_mm256_movemask_ps(
                   simde_mm256_cmp_ps(data_.simd_data, other.data_.simd_data, _CMP_EQ_OQ)) == 0xff;
      }
      else if constexpr (std::same_as<T, f64> && N == 2)
      {
        return simde_mm_movemask_pd(simde_mm_cmpeq_pd(data_.simd_data, other.data_.simd_data)) ==
               0x3;
      }
      else if constexpr (std::same_as<T, f64> && N == 4)
      {
        return simde_mm256_movemask_pd(
                   simde_mm256_cmp_pd(data_.simd_data, other.data_.simd_data, _CMP_EQ_OQ)) == 0xf;
      }
    }
    else
    {
      for (size_type i{0}; i < size(); ++i)
      {
        if ((*this)[i] != other[i])
        {
          return false;
        }
      }
    }

    return true;
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto operator!=(vector const &other) const noexcept
      -> bool
  {
    return !(*this == other);
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto operator[](size_type index) noexcept -> reference
  {
    PBRT_ASSERT_LT(index, N);
    if constexpr (kIsSimdCompatible)
    {
      return data_.scalar_data[index];
    }
    else
    {
      return data_[index];
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto operator[](size_type index) const noexcept
      -> value_type
  {
    PBRT_ASSERT_LT(index, N);
    if constexpr (kIsSimdCompatible)
    {
      return data_.scalar_data[index];
    }
    else
    {
      return data_[index];
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto x() noexcept -> reference
    requires(N >= 1)
  {
    return (*this)[0];
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto y() noexcept -> reference
    requires(N >= 2)
  {
    return (*this)[1];
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto z() noexcept -> reference
    requires(N >= 3)
  {
    return (*this)[2];
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto w() noexcept -> reference
    requires(N >= 4)
  {
    return (*this)[3];
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto x() const noexcept -> value_type
    requires(N >= 1)
  {
    return (*this)[0];
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto y() const noexcept -> value_type
    requires(N >= 2)
  {
    return (*this)[1];
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto z() const noexcept -> value_type
    requires(N >= 3)
  {
    return (*this)[2];
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto w() const noexcept -> value_type
    requires(N >= 4)
  {
    return (*this)[3];
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto data() noexcept -> pointer
  {
    if constexpr (kIsSimdCompatible)
    {
      return data_.scalar_data.data();
    }
    else
    {
      return data_.data();
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto data() const noexcept -> const_pointer
  {
    if constexpr (kIsSimdCompatible)
    {
      return data_.scalar_data.data();
    }
    else
    {
      return data_.data();
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto simd() noexcept -> auto &
    requires kIsSimdCompatible
  {
    return data_.simd_data;
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto simd() const noexcept -> auto const &
    requires kIsSimdCompatible
  {
    return data_.simd_data;
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR static auto size() noexcept -> size_type
  {
    return N;
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto begin() noexcept -> iterator
  {
    if constexpr (kIsSimdCompatible)
    {
      return data_.scalar_data.begin();
    }
    else
    {
      return data_.begin();
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto end() noexcept -> iterator
  {
    if constexpr (kIsSimdCompatible)
    {
      return data_.scalar_data.end();
    }
    else
    {
      return data_.end();
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto begin() const noexcept -> const_iterator
  {
    if constexpr (kIsSimdCompatible)
    {
      return data_.simd_data.cbegin();
    }
    else
    {
      return data_.cbegin();
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto end() const noexcept -> const_iterator
  {
    if constexpr (kIsSimdCompatible)
    {
      return data_.scalar_data.cend();
    }
    else
    {
      return data_.cend();
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto cbegin() const noexcept -> const_iterator
  {
    if constexpr (kIsSimdCompatible)
    {
      return data_.scalar_data.cend();
    }
    else
    {
      return data_.cend();
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto cend() const noexcept -> const_iterator
  {
    if constexpr (kIsSimdCompatible)
    {
      return data_.scalar_data.cbegin();
    }
    else
    {
      return data_.cbegin();
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto rbegin() noexcept -> reverse_iterator
  {
    if constexpr (kIsSimdCompatible)
    {
      return reverse_iterator(data_.scalar_data.cend());
    }
    else
    {
      return reverse_iterator(data_.cend());
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto rend() noexcept -> reverse_iterator
  {
    if constexpr (kIsSimdCompatible)
    {
      return reverse_iterator(data_.scalar_data.cbegin());
    }
    else
    {
      return reverse_iterator(data_.cbegin());
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto rbegin() const noexcept -> const_reverse_iterator
  {
    if constexpr (kIsSimdCompatible)
    {
      return const_reverse_iterator(data_.scalar_data.cend());
    }
    else
    {
      return const_reverse_iterator(data_.cend());
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto rend() const noexcept -> const_reverse_iterator
  {
    if constexpr (kIsSimdCompatible)
    {
      return const_reverse_iterator(data_.scalar_data.cbegin());
    }
    else
    {
      return const_reverse_iterator(data_.cbegin());
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto crbegin() const noexcept -> const_reverse_iterator
  {
    if constexpr (kIsSimdCompatible)
    {
      return const_reverse_iterator(data_.scalar_data.cend());
    }
    else
    {
      return const_reverse_iterator(data_.cend());
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto crend() const noexcept -> const_reverse_iterator
  {
    if constexpr (kIsSimdCompatible)
    {
      return const_reverse_iterator(data_.scalar_data.cbegin());
    }
    else
    {
      return const_reverse_iterator(data_.cbegin());
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto length_squared() const noexcept -> value_type
  {
    return dot(*this, *this);
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto length() const noexcept -> value_type
    requires floating_point<T>
  {
    if constexpr (kIsSimdCompatible)
    {
      if constexpr (std::same_as<T, f32> && N == 4)
      {
        auto const dot_result{simde_mm_dp_ps(data_.simd_data, data_.simd_data, 0xF1)};
        return simde_mm_cvtss_f32(simde_mm_sqrt_ss(dot_result));
      }
      if constexpr (std::same_as<T, f32> && N == 8)
      {
        auto const mul{simde_mm256_mul_ps(data_.simd_data, data_.simd_data)};
        auto const sum{simde_mm256_hadd_ps(mul, mul)};
        auto const lo{simde_mm256_extractf128_ps(sum, 0)};
        auto const hi{simde_mm256_extractf128_ps(sum, 1)};
        auto const sum2{simde_mm256_add_ps(lo, hi)};
        auto const final{simde_mm_add_ps(sum, simde_mm_movehl_ps(sum2, sum2))};
        auto const scalar{simde_mm_add_ss(final, simde_mm_shuffle_ps(final, final, 0x55))};
        return simde_mm_cvtss_f32(simde_mm_sqrt_ss(scalar));
      }
      else if constexpr (std::same_as<T, f64> && N == 2)
      {
        auto const mul{simde_mm_mul_pd(data_.simd_data, data_.simd_data)};
        auto const sum{simde_mm_hadd_pd(mul, mul)};
        return simde_mm_cvtsd_f64(simde_mm_sqrt_sd(sum, sum));
      }
      else if constexpr (std::same_as<T, f64> && N == 4)
      {
        auto const mul{simde_mm256_mul_pd(data_.simd_data, data_.simd_data)};
        auto const sum{simde_mm256_hadd_pd(mul, mul)};
        auto const lo{simde_mm256_extractf128_ps(sum, 0)};
        auto const hi{simde_mm256_extractf128_ps(sum, 1)};
        auto const sum2{simde_mm256_add_ps(lo, hi)};
        return simde_mm_cvtss_f64(simde_mm_sqrt_sd(sum, sum));
      }
    }
    else
    {
      return safe_sqrt(length_squared());
    }
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto fast_length() const noexcept -> value_type
    requires floating_point<T>
  {
    auto const len_sq{length_squared()};
    if (len_sq > kEpsilonV<T>)
    {
      return T{1} / fast_inv_sqrt(len_sq);
    }

    return {};
  }

  PBRT_INLINE PBRT_CONSTEXPR auto normalize() noexcept -> vector &
    requires floating_point<T>
  {
    auto const len{length()};
    if (len > kEpsilonV<T>)
    {
      *this /= len;
    }
    else
    {
      *this = {};
    }

    return *this;
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto normalized() const noexcept -> vector
    requires floating_point<T>
  {
    auto const len{length()};
    if (len > kEpsilonV<T>)
    {
      return *this / len;
    }

    return {};
  }

  PBRT_INLINE PBRT_CONSTEXPR auto fast_normalize() noexcept -> vector &
    requires floating_point<T>
  {
    auto const len_sq{length_squared()};
    if (len_sq > kEpsilonV<T>)
    {
      *this *= fast_inv_sqrt(len_sq);
    }
    else
    {
      *this = {};
    }

    return *this;
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto fast_normalized() const noexcept -> vector
  {
    auto const len_sq{length_squared()};
    if (len_sq > kEpsilonV<T>)
    {
      return *this * fast_inv_sqrt(len_sq);
    }

    return {};
  }

private:
  simd::storage_t<T, N> data_{};
};

template <scalar_type T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto operator+(vector<T, N> const &lhs,
                                                         vector<T, N> const &rhs) noexcept
    -> vector<T, N>
{
  auto result{lhs};
  result += rhs;
  return result;
}

template <scalar_type T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto operator-(vector<T, N> const &lhs,
                                                         vector<T, N> const &rhs) noexcept
    -> vector<T, N>
{
  auto result{lhs};
  result -= rhs;
  return result;
}

template <scalar_type T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto operator-(vector<T, N> const &vec) noexcept
    -> vector<T, N>
{
  if constexpr (vector<T, N>::kIsSimdCompatible)
  {
    if constexpr (std::same_as<T, f32> && N == 4)
    {
      auto const zero{simde_mm_setzero_ps()};
      return vector<T, N>{simde_mm_sub_ps(zero, vec.simd())};
    }
    else if constexpr (std::same_as<T, f32> && N == 8)
    {
      auto const zero{simde_mm256_setzero_ps()};
      return vector<T, N>{simde_mm256_sub_ps(zero, vec.simd())};
    }
    else if constexpr (std::same_as<T, f64> && N == 2)
    {
      auto const zero{simde_mm_setzero_pd()};
      return vector<T, N>{simde_mm_sub_pd(zero, vec.simd())};
    }
    else if constexpr (std::same_as<T, f64> && N == 1)
    {
      auto const zero{simde_mm256_setzero_pd()};
      return vector<T, N>{simde_mm256_sub_pd(zero, vec.simd())};
    }
  }
  else
  {
    vector<T, N> result{};
    for (typename vector<T, N>::size_type i{0}; i < N; ++i)
    {
      result[i] = -vec[i];
    }

    return result;
  }
}

template <scalar_type T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto operator*(vector<T, N> const &lhs,
                                                         vector<T, N> const &rhs) noexcept
    -> vector<T, N>
{
  auto result{lhs};
  result *= rhs;
  return result;
}

template <scalar_type T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto operator*(vector<T, N> const &lhs,
                                                         T const &rhs) noexcept -> vector<T, N>
{
  auto result{lhs};
  result *= rhs;
  return result;
}

template <scalar_type T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto operator*(T const &lhs, vector<T, N> const &rhs)
    -> vector<T, N>
{
  auto result{rhs};
  result *= lhs;
  return result;
}

template <scalar_type T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto operator/(vector<T, N> const &lhs,
                                                         vector<T, N> const &rhs) noexcept
    -> vector<T, N>
{
  auto result{lhs};
  result /= rhs;
  return result;
}

template <scalar_type T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto operator/(vector<T, N> const &lhs,
                                                         T const &rhs) noexcept -> vector<T, N>
{
  auto result{lhs};
  result /= rhs;
  return result;
}

template <scalar_type T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto dot(vector<T, N> const &lhs,
                                                   vector<T, N> const &rhs) noexcept -> T
{
  if constexpr (vector<T, N>::kIsSimdCompatible)
  {
    if constexpr (std::same_as<T, f32> && N == 4)
    {
      return simde_mm_cvtss_f32(simde_mm_dp_ps(lhs.simd(), rhs.simd(), 0xF1));
    }
    else if constexpr (std::same_as<T, f32> && N == 8)
    {
      auto const squared{simde_mm256_mul_ps(lhs.simd(), rhs.simd())};
      auto const sum{simde_mm256_hadd_ps(squared, squared)};
      auto const sum2{simde_mm256_hadd_ps(sum, sum)};
      auto const lo{simde_mm256_extractf128_ps(sum2, 0)};
      auto const hi{simde_mm256_extractf128_ps(sum2, 1)};
      auto const final_sum{simde_mm_add_ps(lo, hi)};
      return simde_mm_cvtss_f32(final_sum);
    }
    else if constexpr (std::same_as<T, f64> && N == 2)
    {
      auto const squared{simde_mm_mul_pd(lhs.simd(), rhs.simd())};
      auto const sum{simde_mm_hadd_pd(squared, squared)};
      return simde_mm_cvtsd_f64(sum);
    }
    else if constexpr (std::same_as<T, f64> && N == 4)
    {
      auto const squared{simde_mm256_mul_pd(lhs.simd(), rhs.simd())};
      auto const sum{simde_mm256_hadd_pd(squared, squared)};
      auto const lo{simde_mm256_extractf128_pd(sum, 0)};
      auto const hi{simde_mm256_extractf128_pd(sum, 1)};
      auto const final_sum{simde_mm_add_pd(lo, hi)};
      return simde_mm_cvtsd_f64(final_sum);
    }
  }
  else
  {
    T result{0};
    for (typename vector<T, N>::size_type i{0}; i < N; ++i)
    {
      result += lhs[i] * rhs[i];
    }

    return result;
  }
}

template <scalar_type T>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto cross(vector<T, 3> const &lhs,
                                                     vector<T, 3> const &rhs) noexcept
    -> vector<T, 3>
{
  return vector<T, 3>{lhs.y() * rhs.z() - lhs.z() * rhs.y(), lhs.z() * rhs.x() - lhs.x() * rhs.z(),
                      lhs.x() * rhs.y() - lhs.y() * rhs.x()};
}

template <floating_point T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto distance_squared(vector<T, N> const &lhs,
                                                                vector<T, N> const &rhs) noexcept ->
    typename vector<T, N>::value_type
{
  return (lhs - rhs).length_squared();
}

template <floating_point T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto distance(vector<T, N> const &lhs,
                                                        vector<T, N> const &rhs) noexcept ->
    typename vector<T, N>::value_type
{
  return (lhs - rhs).length();
}

template <floating_point T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto reflect(vector<T, N> const &incident,
                                                       vector<T, N> const &normal) noexcept
    -> vector<T, N>
{
  return incident - T{2} * dot(incident, normal) * normal;
}

template <floating_point T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto refract(vector<T, N> const &incident,
                                                       vector<T, N> const &normal,
                                                       T const &eta) noexcept -> vector<T, N>
{
  auto const cos_theta{dot(-incident, normal)};
  auto const sin_theta_squared{T{1} - cos_theta * cos_theta};
  auto const sin_theta_t_squared{eta * eta * sin_theta_squared};

  if (sin_theta_t_squared > T{1})
  {
    return vector<T, N>{};
  }

  auto const cos_theta_t{safe_sqrt(T{1} - sin_theta_t_squared)};
  return eta * incident + (eta * cos_theta - cos_theta_t) * normal;
}

template <floating_point T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto lerp(vector<T, N> const &start,
                                                    vector<T, N> const &end, T t) noexcept
    -> vector<T, N>
{
  return start + t * (end - start);
}

template <scalar_type T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto min(vector<T, N> const &lhs,
                                                   vector<T, N> const &rhs) noexcept -> vector<T, N>
{
  vector<T, N> result{};
  for (typename vector<T, N>::size_type i{0}; i < N; ++i)
  {
    result[i] = min(lhs[i], rhs[i]);
  }

  return result;
}

template <scalar_type T, usize N>
PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto max(vector<T, N> const &lhs,
                                                   vector<T, N> const &rhs) noexcept -> vector<T, N>
{
  vector<T, N> result{};
  for (typename vector<T, N>::size_type i{0}; i < N; ++i)
  {
    result[i] = max(lhs[i], rhs[i]);
  }

  return result;
}

using vector2f = vector<f32, 2>;
using vector3f = vector<f32, 3>;
using vector4f = vector<f32, 4>;

using vector2d = vector<f64, 2>;
using vector3d = vector<f64, 3>;
using vector4d = vector<f64, 4>;

using vector2i = vector<i32, 2>;
using vector3i = vector<i32, 3>;
using vector4i = vector<i32, 4>;

using vector2u = vector<u32, 2>;
using vector3u = vector<u32, 3>;
using vector4u = vector<u32, 4>;
} // namespace pbrt::math
