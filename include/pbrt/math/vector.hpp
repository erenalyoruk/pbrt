#pragma once

#include "pbrt/assert.hpp"
#include "pbrt/math/simd/simd_traits.hpp"
#include "pbrt/math/traits.hpp"
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
      -> const_reference
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

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto x() const noexcept -> const_reference
    requires(N >= 1)
  {
    return (*this)[0];
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto y() const noexcept -> const_reference
    requires(N >= 2)
  {
    return (*this)[1];
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto z() const noexcept -> const_reference
    requires(N >= 3)
  {
    return (*this)[2];
  }

  PBRT_NODISCARD PBRT_INLINE PBRT_CONSTEXPR auto w() const noexcept -> const_reference
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

private:
  simd::storage_t<T, N> data_{};
};

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
