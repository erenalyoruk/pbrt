#pragma once

#include "pbrt/alignment_of.hpp"
#include "pbrt/assertions.hpp"
#include "pbrt/inline.hpp"
#include "pbrt/math/constants.hpp"
#include "pbrt/math/traits.hpp"
#include "pbrt/math/utility.hpp"
#include "pbrt/simd.hpp" // IWYU pragma: keep
#include "pbrt/types.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>

namespace pbrt::math
{
template <Arithmetic T, usize N>
  requires(N > 0)
class Vector
{
public:
  using ValueType = T;
  using SizeType = usize;
  using Reference = T &;
  using ConstReference = T const &;
  using Pointer = T *;
  using ConstPointer = T const *;
  using Iterator = typename std::array<T, N>::iterator;
  using ConstIterator = typename std::array<T, N>::const_iterator;
  using ReverseIterator = typename std::array<T, N>::reverse_iterator;
  using ConstReverseIterator = typename std::array<T, N>::const_reverse_iterator;

  /**
   * @brief Default constructor. Initializes all elements to zero.
   */
  constexpr Vector() = default;

  /**
   * @brief Constructor that takes a variable number of arguments.
   *
   * @tparam U The type of the arguments. Must be convertible to T.
   */
  template <ConvertibleTo<T>... U>
    requires(sizeof...(U) == N)
  constexpr Vector(U &&...args) noexcept : m_data{static_cast<T>(std::forward<U>(args))...}
  {
  }

  /**
   * @brief Constructor from another vector of a different type.
   *
   * @tparam U The type of the other vector. Must be convertible to T.
   */
  template <ConvertibleTo<T> U>
  constexpr explicit Vector(Vector<U, N> const &other) noexcept
  {
    std::transform(other.cbegin(), other.cend(), begin(),
                   [](U const &value) { return static_cast<T>(value); });
  }

  /**
   * @brief Fill constructor. Initializes all elements to the same value.
   */
  constexpr explicit Vector(T const &value) noexcept
  {
    m_data.fill(value);
  }

  /**
   * @brief Subscript operator for non-const access.
   *
   * @param index The index of the element to access.
   * @return Reference& The element at the specified index.
   */
  [[nodiscard]] PBRT_INLINE constexpr Reference operator[](SizeType index) noexcept
  {
    PBRT_ASSERT_MSG(index < N, "Vector index out of bounds.");
    return m_data[index];
  }

  /**
   * @brief Subscript operator for const access.
   *
   * @param index The index of the element to access.
   * @return ConstReference The element at the specified index.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReference operator[](SizeType index) const noexcept
  {
    PBRT_ASSERT_MSG(index < N, "Vector index out of bounds.");
    return m_data[index];
  }

  /**
   * @brief Accessor for the underlying data.
   *
   * @param index The index of the element to access.
   * @return Reference The element at the specified index.
   */
  [[nodiscard]] PBRT_INLINE constexpr Reference at(SizeType index)
  {
    PBRT_ASSERT_MSG(index < N, "Index out of bounds.");
    return m_data[index];
  }

  /**
   * @brief Accessor for the underlying const data.
   *
   * @param index The index of the element to access.
   * @return ConstReference The element at the specified index.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReference at(SizeType index) const noexcept
  {
    PBRT_ASSERT_MSG(index < N, "Index out of bounds.");
    return m_data[index];
  }

  /**
   * @brief Accessor for first element.
   *
   * @return Reference The first element.
   */
  [[nodiscard]] PBRT_INLINE constexpr Reference x() noexcept
    requires(N >= 1)
  {
    return m_data[0];
  }

  /**
   * @brief Accessor for first element.
   *
   * @return ConstReference The first element.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReference x() const noexcept
    requires(N >= 1)
  {
    return m_data[0];
  }

  /**
   * @brief Accessor for second element.
   *
   * @return Reference The second element.
   */
  [[nodiscard]] PBRT_INLINE constexpr Reference y() noexcept
    requires(N >= 2)
  {
    return m_data[1];
  }

  /**
   * @brief Accessor for second element.
   *
   * @return ConstReference The second element.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReference y() const noexcept
    requires(N >= 2)
  {
    return m_data[1];
  }

  /**
   * @brief Accessor for third element.
   *
   * @return Reference The third element.
   */
  [[nodiscard]] PBRT_INLINE constexpr Reference z() noexcept
    requires(N >= 3)
  {
    return m_data[2];
  }

  /**
   * @brief Accessor for third element.
   *
   * @return ConstReference The third element.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReference z() const noexcept
    requires(N >= 3)
  {
    return m_data[2];
  }

  /**
   * @brief Accessor for fourth element.
   *
   * @return Reference The fourth element.
   */
  [[nodiscard]] PBRT_INLINE constexpr Reference w() noexcept
    requires(N >= 4)
  {
    return m_data[3];
  }

  /**
   * @brief Accessor for fourth element.
   *
   * @return ConstReference The fourth element.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReference w() const noexcept
    requires(N >= 4)
  {
    return m_data[3];
  }

  /**
   * @brief Add another vector to this vector.
   *
   * @param other The vector to add.
   * @return Vector& A reference to this vector.
   */
  PBRT_INLINE constexpr Vector &operator+=(Vector const &other) noexcept
  {
    for (SizeType i = 0; i < N; ++i)
    {
      m_data[i] += other.m_data[i];
    }

    return *this;
  }

  /**
   * @brief Add a scalar to this vector. All elements are added by the scalar.
   *
   * @param scalar The scalar to add.
   * @return Vector& A reference to this vector.
   */
  PBRT_INLINE constexpr Vector &operator+=(T const &scalar) noexcept
  {
    for (SizeType i = 0; i < N; ++i)
    {
      m_data[i] += scalar;
    }

    return *this;
  }

  /**
   * @brief Subtract another vector from this vector.
   *
   * @param other The vector to subtract.
   * @return Vector& A reference to this vector.
   */
  PBRT_INLINE constexpr Vector &operator-=(Vector const &other) noexcept
  {
    for (SizeType i = 0; i < N; ++i)
    {
      m_data[i] -= other.m_data[i];
    }

    return *this;
  }

  /**
   * @brief Subtract a scalar from this vector. All elements are subtracted by the scalar.
   *
   * @param scalar The scalar to subtract.
   * @return Vector& A reference to this vector.
   */
  PBRT_INLINE constexpr Vector &operator-=(T const &scalar) noexcept
  {
    for (SizeType i = 0; i < N; ++i)
    {
      m_data[i] -= scalar;
    }

    return *this;
  }

  /**
   * @brief Multiply this vector by another vector element-wise.
   *
   * @param other The vector to multiply by.
   * @return Vector& A reference to this vector.
   */
  PBRT_INLINE constexpr Vector &operator*=(Vector const &other) noexcept
  {
    for (SizeType i = 0; i < N; ++i)
    {
      m_data[i] *= other.m_data[i];
    }

    return *this;
  }

  /**
   * @brief Multiply this vector by scalar.
   *
   * @param scalar The scalar to multiply by.
   * @return Vector& A reference to this vector.
   */
  PBRT_INLINE constexpr Vector &operator*=(T const &scalar) noexcept
  {
    for (SizeType i = 0; i < N; ++i)
    {
      m_data[i] *= scalar;
    }

    return *this;
  }

  /**
   * @brief Divide this vector by another vector element-wise.
   *
   * @param other The vector to divide by.
   * @return Vector& A reference to this vector.
   */
  PBRT_INLINE constexpr Vector &operator/=(Vector const &other) noexcept
  {
    for (SizeType i = 0; i < N; ++i)
    {
      m_data[i] = safe_divide(m_data[i], other.m_data[i]);
    }

    return *this;
  }

  /**
   * @brief Divide this vector by scalar.
   *
   * @param scalar The scalar to divide by.
   * @return Vector& A reference to this vector.
   */
  PBRT_INLINE constexpr Vector &operator/=(T const &scalar) noexcept
  {
    for (SizeType i = 0; i < N; ++i)
    {
      m_data[i] = safe_divide(m_data[i], scalar);
    }

    return *this;
  }

  /**
   * @brief Add another vector to this vector.
   *
   * @param other The vector to add.
   * @return Vector A new vector that is the sum of this vector and the other vector.
   */
  PBRT_INLINE constexpr Vector operator+(Vector const &other) const noexcept
  {
    Vector result{*this};
    result += other;
    return result;
  }

  /**
   * @brief Add a scalar to this vector. All elements are added by the scalar.
   *
   * @param scalar The scalar to add.
   * @return Vector A new vector that is the sum of this vector and the scalar.
   */
  PBRT_INLINE constexpr Vector operator+(T const &scalar) const noexcept
  {
    Vector result{*this};
    result += scalar;
    return result;
  }

  /**
   * @brief Negate this vector.
   *
   * @return Vector A new vector that is the negation of this vector.
   */
  PBRT_INLINE constexpr Vector operator-() const noexcept
  {
    Vector result{};
    for (SizeType i = 0; i < N; ++i)
    {
      result.m_data[i] = -m_data[i];
    }

    return result;
  }

  /**
   * @brief Subtract another vector from this vector.
   *
   * @param other The vector to subtract.
   * @return Vector A new vector that is the difference of this vector and the other vector.
   */
  PBRT_INLINE constexpr Vector operator-(Vector const &other) const noexcept
  {
    Vector result{*this};
    result -= other;
    return result;
  }

  /**
   * @brief Subtract a scalar from this vector. All elements are subtracted by the scalar.
   *
   * @param scalar The scalar to subtract.
   * @return Vector A new vector that is the difference of this vector and the scalar.
   */
  PBRT_INLINE constexpr Vector operator-(T const &scalar) const noexcept
  {
    Vector result{*this};
    result -= scalar;
    return result;
  }

  /**
   * @brief Multiply this vector by another vector element-wise.
   *
   * @param other The vector to multiply by.
   * @return Vector A new vector that is the product of this vector and the other vector.
   */
  PBRT_INLINE constexpr Vector operator*(Vector const &other) const noexcept
  {
    Vector result{*this};
    result *= other;
    return result;
  }

  /**
   * @brief Multiply this vector by a scalar.
   *
   * @param scalar The scalar to multiply by.
   * @return Vector A new vector that is the product of this vector and the scalar.
   */
  PBRT_INLINE constexpr Vector operator*(T const &scalar) const noexcept
  {
    Vector result{*this};
    result *= scalar;
    return result;
  }

  /**
   * @brief Divide this vector by another vector element-wise.
   *
   * @param other The vector to divide by.
   * @return Vector A new vector that is the quotient of this vector and the other vector.
   */
  PBRT_INLINE constexpr Vector operator/(Vector const &other) const noexcept
  {
    Vector result{*this};
    result /= other;
    return result;
  }

  /**
   * @brief Divide this vector by a scalar.
   *
   * @param scalar The scalar to divide by.
   * @return Vector A new vector that is the quotient of this vector and the scalar.
   */
  PBRT_INLINE constexpr Vector operator/(T const &scalar) const noexcept
  {
    Vector result{*this};
    result /= scalar;
    return result;
  }

  /**
   * @brief Equality operator. Each element is compared for equality.
   *
   * @param other The vector to compare with.
   * @return bool True if the vectors are equal, false otherwise.
   */
  PBRT_INLINE constexpr bool operator==(Vector const &other) const noexcept
  {
    for (usize i = 0; i < N; ++i)
    {
      if (m_data[i] != other[i])
      {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief Inequality operator. Each element is compared for inequality.
   *
   * @param other The vector to compare with.
   * @return bool True if the vectors are not equal, false otherwise.
   */
  PBRT_INLINE constexpr bool operator!=(Vector const &other) const noexcept
  {
    return !(*this == other);
  }

  /**
   * @brief Approximate equality operator. Each element is compared for approximate equality.
   * Requires floating point type.
   *
   * @param other The vector to compare with.
   * @param epsilon The tolerance for approximate equality.
   * @return bool True if the vectors are approximately equal, false otherwise.
   */
  PBRT_INLINE constexpr bool approx_equal(Vector const &other,
                                          T const &epsilon = PRECISION_EPSILON<T>) const noexcept
    requires FloatingPoint<T>
  {
    for (SizeType i = 0; i < N; ++i)
    {
      if (std::abs(m_data[i] - other.m_data[i]) > epsilon)
      {
        return false;
      }
    }

    return true;
  }

  /**
   * @brief Dot product of this vector with another vector.
   *
   * @param other The vector to compute the dot product with.
   * @return T The dot product of the two vectors.
   */
  [[nodiscard]] PBRT_INLINE constexpr T dot(Vector const &other) const noexcept
  {
    if constexpr (N == 1)
    {
      return m_data[0] * other.m_data[0];
    }

    if constexpr (N == 2)
    {
      return (m_data[0] * other.m_data[0]) + (m_data[1] * other.m_data[1]);
    }

    if constexpr (N == 3)
    {
      return (m_data[0] * other.m_data[0]) + (m_data[1] * other.m_data[1]) +
             (m_data[2] * other.m_data[2]);
    }

    if constexpr (N == 4)
    {
      return (m_data[0] * other.m_data[0]) + (m_data[1] * other.m_data[1]) +
             (m_data[2] * other.m_data[2]) + (m_data[3] * other.m_data[3]);
    }

    return std::inner_product(cbegin(), cend(), other.cbegin(), T{0});
  }

  /**
   * @brief Length squared of the vector.
   *
   * @return T The length squared of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr T length_squared() const noexcept
  {
    return dot(*this);
  }

  /**
   * @brief Length of the vector. Requires floating point type.
   *
   * @return T The length of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr T length() const noexcept
    requires FloatingPoint<T>
  {
    return std::sqrt(length_squared());
  }

  /**
   * @brief Distance squared between this vector and another vector.
   *
   * @param other The other vector.
   * @return T The distance squared between the two vectors.
   */
  [[nodiscard]] PBRT_INLINE constexpr T distance_squared(Vector const &other) const noexcept
  {
    return (*this - other).length_squared();
  }

  /**
   * @brief Distance between this vector and another vector. Requires floating point type.
   *
   * @param other The other vector.
   * @return T The distance between the two vectors.
   */
  [[nodiscard]] PBRT_INLINE constexpr T distance(Vector const &other) const noexcept
    requires FloatingPoint<T>
  {
    return (*this - other).length();
  }

  /**
   * @brief Normalize the vector. Requires floating point type.
   *
   * @return Vector& A reference to this vector.
   */
  PBRT_INLINE constexpr Vector &normalize() noexcept
    requires FloatingPoint<T>
  {
    T len{length()};
    PBRT_ASSERT_MSG(len > T{0}, "Cannot normalize a zero vector.");
    return *this /= len;
  }

  /**
   * @brief Normalize the vector. Requires floating point type.
   *
   * @return Vector A new vector that is the normalized version of this vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr Vector normalized() const noexcept
    requires FloatingPoint<T>
  {
    Vector result{*this};
    return result.normalize();
  }

  /**
   * @brief Normalize the vector. If the vector is zero, returns a fallback vector.
   * This function is useful for ensuring that the vector is not zero before normalizing.
   * It returns a fallback vector if the original vector is zero. Requires floating point type.
   *
   * @param fallback The fallback vector to return if this vector is zero.
   * @return Vector A new vector that is the normalized version of this vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr Vector safe_normalized(
      Vector const &fallback = unit_x()) const noexcept
    requires FloatingPoint<T>
  {
    const T lenSq{length_squared()};
    if (lenSq < PRECISION_EPSILON<T>)
    {
      return fallback;
    }

    return *this / std::sqrt(lenSq);
  }

  /**
   * @brief Project this vector onto another vector. Requires floating point type.
   *
   * @param other The vector to project onto.
   * @return Vector A new vector that is the projection of this vector onto the other vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr Vector project_onto(Vector const &other) const noexcept
    requires FloatingPoint<T>
  {
    const T otherLenSq{other.length_squared()};
    PBRT_ASSERT_MSG(otherLenSq > T{0}, "Cannot project onto a zero vector.");
    return other * (dot(other) / otherLenSq);
  }

  /**
   * @brief Reflect this vector around a normal vector. Requires floating point type.
   *
   * @param normal The normal vector to reflect around.
   * @return Vector A new vector that is the reflection of this vector around the normal vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr Vector reflect(Vector const &normal) const noexcept
    requires FloatingPoint<T>
  {
    return *this - (normal * (T{2} * dot(normal)));
  }

  /**
   * @brief Refract this vector through a surface with a given normal and index of refraction.
   * This function computes the refraction of the vector through a surface with a given normal
   * and index of refraction. It uses Snell's law to calculate the refraction angle.
   * If the refraction is total internal reflection, the function returns a zero vector.
   * The function assumes that the normal vector is normalized. Requires floating point type.
   *
   * @param normal The normal vector of the surface.
   * @param eta The index of refraction.
   * @return Vector A new vector that is the refraction of this vector through the surface.
   */
  [[nodiscard]] PBRT_INLINE constexpr Vector refract(Vector const &normal,
                                                     T const &eta) const noexcept
    requires FloatingPoint<T>
  {
    const T cosThetaI{dot(normal)};
    const T sin2ThetaT{eta * eta * (1 - cosThetaI * cosThetaI)};
    if (sin2ThetaT > T{1})
    {
      return Vector{};
    }

    return (*this * eta) - (normal * (eta * cosThetaI + std::sqrt(T{1} - sin2ThetaT)));
  }

  /**
   * @brief Linear interpolation between this vector and another vector. Requires floating point
   * type.
   *
   * @param other The other vector.
   * @param t The interpolation factor. A value of 0 returns this vector, and a value of 1 returns
   * the other vector.
   * @return Vector A new vector that is the result of the linear interpolation.
   */
  [[nodiscard]] PBRT_INLINE constexpr Vector lerp(Vector const &other, T const &t) const noexcept
    requires FloatingPoint<T>
  {
    return (*this * (T{1} - t)) + (other * t);
  }

  /**
   * @brief Get the minimum component of the vector.
   *
   * @return T The minimum component of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr T min_component() const noexcept
  {
    return *std::min_element(cbegin(), cend());
  }

  /**
   * @brief Get the maximum component of the vector.
   *
   * @return T The maximum component of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr T max_component() const noexcept
  {
    return *std::max_element(cbegin(), cend());
  }

  /**
   * @brief Get the minimum dimension of the vector.
   *
   * @return T The minimum dimension of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr SizeType min_dimension() const noexcept
  {
    return std::distance(cbegin(), std::min_element(cbegin(), cend()));
  }

  /**
   * @brief Get the maximum dimension of the vector.
   *
   * @return T The maximum dimension of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr SizeType max_dimension() const noexcept
  {
    return std::distance(cbegin(), std::max_element(cbegin(), cend()));
  }

  /**
   * @brief Get the absolute value of the vector.
   *
   * @return Vector A new vector with the absolute values of the components.
   */
  [[nodiscard]] PBRT_INLINE constexpr Vector abs() const noexcept
  {
    Vector result{*this};
    for (SizeType i = 0; i < N; ++i)
    {
      result.m_data[i] = std::abs(m_data[i]);
    }

    return result;
  }

  /**
   * @brief Clamp the vector components to a specified range.
   *
   * @param min The minimum value for clamping.
   * @param max The maximum value for clamping.
   * @return Vector A new vector with the clamped components.
   */
  [[nodiscard]] PBRT_INLINE constexpr Vector clamp(T const &min, T const &max) const noexcept
  {
    Vector result{*this};
    for (SizeType i = 0; i < N; ++i)
    {
      result.m_data[i] = std::clamp(m_data[i], min, max);
    }

    return result;
  }

  /**
   * @brief Clamp the vector components to a specified range.
   *
   * @param min The minimum value for clamping.
   * @param max The maximum value for clamping.
   * @return Vector A new vector with the clamped components.
   */
  [[nodiscard]] PBRT_INLINE constexpr Vector clamp(Vector const &min,
                                                   Vector const &max) const noexcept
  {
    Vector result{*this};
    for (SizeType i = 0; i < N; ++i)
    {
      result.m_data[i] = std::clamp(m_data[i], min.m_data[i], max.m_data[i]);
    }

    return result;
  }

  /**
   * @brief Get the zero vector.
   *
   * @return Vector A new vector with all components set to zero.
   */
  [[nodiscard]] PBRT_INLINE static constexpr Vector zero() noexcept
  {
    return Vector{};
  }

  /**
   * @brief Get the one vector.
   *
   * @return Vector A new vector with all components set to one.
   */
  [[nodiscard]] PBRT_INLINE static constexpr Vector one() noexcept
  {
    return Vector{T{1}};
  }

  /**
   * @brief Get the unit vector in the x direction.
   *
   * @return Vector A new vector with the x component set to one and all other components set to
   * zero.
   */
  [[nodiscard]] PBRT_INLINE static constexpr Vector unit_x() noexcept
    requires(N >= 1)
  {
    Vector result{};
    result[0] = T{1};
    return result;
  }

  /**
   * @brief Get the unit vector in the y direction.
   *
   * @return Vector A new vector with the y component set to one and all other components set to
   */
  [[nodiscard]] PBRT_INLINE static constexpr Vector unit_y() noexcept
    requires(N >= 2)
  {
    Vector result{};
    result[1] = T{1};
    return result;
  }

  /**
   * @brief Get the unit vector in the z direction.
   *
   * @return Vector A new vector with the z component set to one and all other components set to
   */
  [[nodiscard]] PBRT_INLINE static constexpr Vector unit_z() noexcept
    requires(N >= 3)
  {
    Vector result{};
    result[2] = T{1};
    return result;
  }

  /**
   * @brief Get the unit vector in the w direction.
   *
   * @return Vector A new vector with the w component set to one and all other components set to
   */
  [[nodiscard]] PBRT_INLINE static constexpr Vector unit_w() noexcept
    requires(N >= 4)
  {
    Vector result{};
    result[3] = T{1};
    return result;
  }

  /**
   * @brief Get the beginning of the vector.
   *
   * @return Iterator An iterator to the beginning of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr Iterator begin() noexcept
  {
    return m_data.begin();
  }

  /**
   * @brief Get the beginning of the vector.
   *
   * @return ConstIterator A const iterator to the beginning of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstIterator begin() const noexcept
  {
    return m_data.cbegin();
  }

  /**
   * @brief Get the beginning of the vector.
   *
   * @return ConstIterator A const iterator to the beginning of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstIterator cbegin() const noexcept
  {
    return m_data.cbegin();
  }

  /**
   * @brief Get the end of the vector.
   *
   * @return Iterator An iterator to the end of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr Iterator end() noexcept
  {
    return m_data.end();
  }

  /**
   * @brief Get the end of the vector.
   *
   * @return ConstIterator A const iterator to the end of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstIterator end() const noexcept
  {
    return m_data.cend();
  }

  /**
   * @brief Get the end of the vector.
   *
   * @return ConstIterator A const iterator to the end of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstIterator cend() const noexcept
  {
    return m_data.cend();
  }

  /**
   * @brief Get the reverse beginning of the vector.
   *
   * @return ReverseIterator A reverse iterator to the beginning of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ReverseIterator rbegin() noexcept
  {
    return m_data.rbegin();
  }

  /**
   * @brief Get the reverse beginning of the vector.
   *
   * @return ConstReverseIterator A const reverse iterator to the beginning of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReverseIterator rbegin() const noexcept
  {
    return m_data.crbegin();
  }

  /**
   * @brief Get the reverse beginning of the vector.
   *
   * @return ConstReverseIterator A const reverse iterator to the beginning of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReverseIterator crbegin() const noexcept
  {
    return m_data.crbegin();
  }

  /**
   * @brief Get the reverse end of the vector.
   *
   * @return ReverseIterator A reverse iterator to the end of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ReverseIterator rend() noexcept
  {
    return m_data.rend();
  }

  /**
   * @brief Get the reverse end of the vector.
   *
   * @return ConstReverseIterator A const reverse iterator to the end of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReverseIterator rend() const noexcept
  {
    return m_data.crend();
  }

  /**
   * @brief Get the reverse end of the vector.
   *
   * @return ConstReverseIterator A const reverse iterator to the end of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReverseIterator crend() const noexcept
  {
    return N;
  }

  /**
   * @brief Get the size of the vector.
   *
   * @return SizeType The size of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr SizeType size() const noexcept
  {
    return N;
  }

  /**
   * @brief Get the maximum size of the vector.
   *
   * @return SizeType The maximum size of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr SizeType max_size() const noexcept
  {
    return N;
  }

  /**
   * @brief Check if the vector is empty.
   *
   * @return bool True if the vector is empty, false otherwise.
   */
  [[nodiscard]] PBRT_INLINE constexpr bool empty() const noexcept
  {
    return false;
  }

  /**
   * @brief Get the underlying data of the vector.
   *
   * @return Pointer A pointer to the underlying data of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr Pointer data() noexcept
  {
    return m_data.data();
  }

  /**
   * @brief Get the underlying const data of the vector.
   *
   * @return ConstPointer A const pointer to the underlying data of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstPointer data() const noexcept
  {
    return m_data.data();
  }

  /**
   * @brief Get the first element of the vector.
   *
   * @return Reference The first element of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr Reference front() noexcept
  {
    return m_data.front();
  }

  /**
   * @brief Get the first element of the vector.
   *
   * @return ConstReference The first element of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReference front() const noexcept
  {
    return m_data.front();
  }

  /**
   * @brief Get the last element of the vector.
   *
   * @return Reference The last element of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr Reference back() noexcept
  {
    return m_data.back();
  }

  /**
   * @brief Get the last element of the vector.
   *
   * @return ConstReference The last element of the vector.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReference back() const noexcept
  {
    return m_data.back();
  }

private:
  alignas(ALIGN_OF(T)) std::array<T, N> m_data{};
};

/**
 * @brief Scalar multiplication of a vector.
 *
 * @tparam T The type of the vector components.
 * @tparam N  The number of components in the vector.
 * @param scalar The scalar value to multiply the vector by.
 * @param vector The vector to multiply.
 * @return Vector A new vector with each component multiplied by the scalar.
 */
template <Arithmetic T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, N> operator*(
    T const &scalar, Vector<T, N> const &vector) noexcept
{
  return vector * scalar;
}

/**
 * @brief Cross product of two 3D vectors.
 *
 * @tparam T The type of the vector components.
 * @param lhs The left-hand side vector.
 * @param rhs The right-hand side vector.
 * @return Vector A new vector that is the cross product of the two vectors.
 */
template <Arithmetic T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, 3> cross(Vector<T, 3> const &lhs,
                                                                Vector<T, 3> const &rhs) noexcept
{
  return {lhs[1] * rhs[2] - lhs[2] * rhs[1], lhs[2] * rhs[0] - lhs[0] * rhs[2],
          lhs[0] * rhs[1] - lhs[1] * rhs[0]};
}

/**
 * @brief Dot product of two vectors.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param lhs The left-hand side vector.
 * @param rhs The right-hand side vector.
 * @return T The dot product of the two vectors.
 */
template <Arithmetic T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr T dot(Vector<T, N> const &lhs,
                                                   Vector<T, N> const &rhs) noexcept
{
  return lhs.dot(rhs);
}

/**
 * @brief Length squared of a vector.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param vector The vector to calculate the length squared of.
 * @return T The length squared of the vector.
 */
template <Arithmetic T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr T length_squared(Vector<T, N> const &vector) noexcept
{
  return vector.length_squared();
}

/**
 * @brief Length of a vector.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param vector The vector to calculate the length of.
 * @return T The length of the vector.
 */
template <FloatingPoint T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr T length(Vector<T, N> const &vector) noexcept
{
  return vector.length();
}

/**
 * @brief Distance squared between two vectors.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param lhs The left-hand side vector.
 * @param rhs The right-hand side vector.
 * @return T The distance squared between the two vectors.
 */
template <Arithmetic T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr T distance_squared(Vector<T, N> const &lhs,
                                                                Vector<T, N> const &rhs) noexcept
{
  return lhs.distance_squared(rhs);
}

/**
 * @brief Distance between two vectors.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param lhs The left-hand side vector.
 * @param rhs The right-hand side vector.
 * @return T The distance between the two vectors.
 */
template <FloatingPoint T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr T distance(Vector<T, N> const &lhs,
                                                        Vector<T, N> const &rhs) noexcept
{
  return lhs.distance(rhs);
}

/**
 * @brief Normalize a vector.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param vector The vector to normalize.
 * @return Vector A new vector that is the normalized version of the input vector.
 */
template <Arithmetic T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, N> normalize(
    Vector<T, N> const &vector) noexcept
  requires FloatingPoint<T>
{
  return vector.normalized();
}

/**
 * @brief Normalize a vector with a fallback value.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param vector The vector to normalize.
 * @param fallback The fallback vector to use if the input vector is zero.
 * @return Vector A new vector that is the normalized version of the input vector or the fallback
 * vector.
 */
template <Arithmetic T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, N> safe_normalized(
    Vector<T, N> const &vector, Vector<T, N> const &fallback) noexcept
  requires FloatingPoint<T>
{
  return vector.safe_normalized(fallback);
}

/**
 * @brief Linear interpolation between two vectors.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param lhs The left-hand side vector.
 * @param rhs The right-hand side vector.
 * @param t The interpolation factor (0 <= t <= 1).
 * @return Vector A new vector that is the result of linear interpolation between the two vectors.
 */
template <FloatingPoint T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, N> lerp(Vector<T, N> const &lhs,
                                                               Vector<T, N> const &rhs,
                                                               T const &t) noexcept
{
  return lhs.lerp(rhs, t);
}

/**
 * @brief Reflect a vector around a normal.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param vector The vector to reflect.
 * @param normal The normal vector to reflect around.
 * @return Vector A new vector that is the reflection of the input vector around the normal.
 */
template <Arithmetic T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, N> reflect(
    Vector<T, N> const &vector, Vector<T, N> const &normal) noexcept
  requires FloatingPoint<T>
{
  return vector.reflect(normal);
}

/**
 * @brief Refract a vector through a surface with a given index of refraction.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param vector The vector to refract.
 * @param normal The normal vector at the surface.
 * @param eta The index of refraction of the medium.
 * @return Vector A new vector that is the refracted version of the input vector.
 */
template <Arithmetic T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, N> refract(Vector<T, N> const &vector,
                                                                  Vector<T, N> const &normal,
                                                                  T const &eta) noexcept
  requires FloatingPoint<T>
{
  return vector.refract(normal, eta);
}

/**
 * @brief Get the minimum component of a vector.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param lhs The left-hand side vector.
 * @param rhs The right-hand side vector.
 * @return Vector A new vector with each component being the minimum of the two vectors.
 */
template <Arithmetic T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, N> min(Vector<T, N> const &lhs,
                                                              Vector<T, N> const &rhs) noexcept
{
  Vector<T, N> result{};
  for (usize i = 0; i < N; ++i)
  {
    result[i] = std::min(lhs[i], rhs[i]);
  }

  return result;
}

/**
 * @brief Get the maximum component of a vector.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param lhs The left-hand side vector.
 * @param rhs The right-hand side vector.
 * @return Vector A new vector with each component being the maximum of the two vectors.
 */
template <Arithmetic T, usize N>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, N> max(Vector<T, N> const &lhs,
                                                              Vector<T, N> const &rhs) noexcept
{
  Vector<T, N> result{};
  for (usize i = 0; i < N; ++i)
  {
    result[i] = std::max(lhs[i], rhs[i]);
  }

  return result;
}

/**
 * @brief Get the x and y components of a 3D vector as a 2D vector.
 *
 * @tparam T
 * @param vector The 3D vector to extract the x and y components from.
 * @return Vector A new 2D vector containing the x and y components of the input vector.
 */
template <Arithmetic T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, 2> xy(Vector<T, 3> const &vector) noexcept
{
  return Vector<T, 2>{vector.x(), vector.y()};
}

/**
 * @brief Get the x and z components of a 3D vector as a 2D vector.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param vector The 3D vector to extract the x and z components from.
 * @return Vector A new 2D vector containing the x and z components of the input vector.
 */
template <Arithmetic T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, 2> xz(Vector<T, 3> const &vector) noexcept
{
  return Vector<T, 2>{vector.x(), vector.z()};
}
/**
 * @brief Get the y and z components of a 3D vector as a 2D vector.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param vector The 3D vector to extract the y and z components from.
 * @return Vector A new 2D vector containing the y and z components of the input vector.
 */
template <Arithmetic T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, 2> yz(Vector<T, 3> const &vector) noexcept
{
  return Vector<T, 2>{vector.y(), vector.z()};
}

/**
 * @brief Get the y and z components of a 3D vector as a 2D vector.
 *
 * @tparam T The type of the vector components.
 * @tparam N The number of components in the vector.
 * @param vector The 3D vector to extract the y and z components from.
 * @return Vector A new 2D vector containing the y and z components of the input vector.
 */
template <Arithmetic T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, 3> xyz(Vector<T, 4> const &vector) noexcept
{
  return Vector<T, 3>{vector.x(), vector.y(), vector.z()};
}

using Vec2u = Vector<u32, 2>;
using Vec3u = Vector<u32, 3>;
using Vec4u = Vector<u32, 4>;

using Vec2i = Vector<i32, 2>;
using Vec3i = Vector<i32, 3>;
using Vec4i = Vector<i32, 4>;

using Vec2f = Vector<f32, 2>;
using Vec3f = Vector<f32, 3>;
using Vec4f = Vector<f32, 4>;

using Vec2d = Vector<f64, 2>;
using Vec3d = Vector<f64, 3>;
using Vec4d = Vector<f64, 4>;

using Vec2 = Vec2f;
using Vec3 = Vec3f;
using Vec4 = Vec4f;

template <>
[[nodiscard]] PBRT_INLINE constexpr f32 Vector<f32, 2>::dot(Vector const &other) const noexcept
{
  return (m_data[0] * other.m_data[0]) + (m_data[1] * other.m_data[1]);
}

template <>
PBRT_INLINE constexpr Vector<f32, 4> &Vector<f32, 4>::operator+=(Vector const &other) noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};
  __m128 b{_mm_loadu_ps(other.m_data.data())};
  __m128 sum{_mm_add_ps(a, b)};

  _mm_storeu_ps(m_data.data(), sum);

  return *this;
#else
  for (usize i = 0; i < 4; ++i)
  {
    m_data[i] += other.m_data[i];
  }

  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Vector<f32, 4> &Vector<f32, 4>::operator-=(Vector const &other) noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};
  __m128 b{_mm_loadu_ps(other.m_data.data())};
  __m128 diff{_mm_sub_ps(a, b)};

  _mm_storeu_ps(m_data.data(), diff);

  return *this;
#else
  for (usize i = 0; i < 4; ++i)
  {
    m_data[i] -= other.m_data[i];
  }

  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Vector<f32, 4> &Vector<f32, 4>::operator*=(Vector const &other) noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};
  __m128 b{_mm_loadu_ps(other.m_data.data())};
  __m128 mul{_mm_mul_ps(a, b)};

  _mm_storeu_ps(m_data.data(), mul);

  return *this;
#else
  for (usize i = 0; i < 4; ++i)
  {
    m_data[i] *= other.m_data[i];
  }

  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Vector<f32, 4> &Vector<f32, 4>::operator*=(f32 const &scalar) noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};
  __m128 b{_mm_set1_ps(scalar)};
  __m128 mul{_mm_mul_ps(a, b)};

  _mm_storeu_ps(m_data.data(), mul);

  return *this;
#else
  for (usize i = 0; i < 4; ++i)
  {
    m_data[i] *= scalar;
  }

  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Vector<f32, 4> &Vector<f32, 4>::operator/=(Vector const &other) noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};
  __m128 b{_mm_loadu_ps(other.m_data.data())};
  __m128 div{_mm_div_ps(a, b)};

  _mm_storeu_ps(m_data.data(), div);

  return *this;
#else
  for (usize i = 0; i < 4; ++i)
  {
    m_data[i] /= other.m_data[i];
  }

  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Vector<f32, 4> &Vector<f32, 4>::operator/=(f32 const &scalar) noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};
  __m128 b{_mm_set1_ps(scalar)};
  __m128 div{_mm_div_ps(a, b)};

  _mm_storeu_ps(m_data.data(), div);

  return *this;
#else
  for (usize i = 0; i < 4; ++i)
  {
    m_data[i] /= scalar;
  }

  return *this;
#endif
}

template <>
PBRT_INLINE constexpr bool Vector<f32, 4>::operator==(Vector const &other) const noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};
  __m128 b{_mm_loadu_ps(other.m_data.data())};

  __m128 cmp{_mm_cmpeq_ps(a, b)};
  int mask{_mm_movemask_ps(cmp)};

  return (mask & 0xF) == 0xF;
#else
  for (usize i = 0; i < 4; ++i)
  {
    if (m_data[i] != other[i])
    {
      return false;
    }
  }
  return true;
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr f32 Vector<f32, 4>::length() const noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};
  __m128 mul{_mm_mul_ps(a, a)};
  __m128 sum{_mm_hadd_ps(mul, mul)};

  sum = _mm_hadd_ps(sum, sum);

  return std::sqrt(_mm_cvtss_f32(sum));
#else
  return std::sqrt(length_squared());
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr f32 Vector<f32, 4>::min_component() const noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};

  __m128 min1{_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 3, 0, 1))};
  __m128 min2{_mm_min_ps(a, min1)};
  __m128 min3{_mm_shuffle_ps(min2, min2, _MM_SHUFFLE(1, 0, 3, 2))};

  __m128 result{_mm_min_ps(min2, min3)};

  return _mm_cvtss_f32(result);
#else
  return *std::ranges::min_element(cbegin(), cend());
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr f32 Vector<f32, 4>::max_component() const noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};

  __m128 max1{_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 3, 0, 1))};
  __m128 max2{_mm_max_ps(a, max1)};
  __m128 max3{_mm_shuffle_ps(max2, max2, _MM_SHUFFLE(1, 0, 3, 2))};

  __m128 result{_mm_max_ps(max2, max3)};

  return _mm_cvtss_f32(result);
#else
  return *std::ranges::max_element(cbegin(), cend());
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr Vector<f32, 4> Vector<f32, 4>::abs() const noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};
  __m128 mask{_mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF))};
  __m128 result{_mm_and_ps(a, mask)};

  Vector<f32, 4> output{};
  _mm_storeu_ps(output.data(), result);

  return output;
#else
  Vector result{*this};
  for (SizeType i = 0; i < 4; ++i)
  {
    result.m_data[i] = std::abs(m_data[i]);
  }
  return result;
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr f32 Vector<f32, 4>::length_squared() const noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};
  __m128 mul{_mm_mul_ps(a, a)};
  __m128 sum{_mm_hadd_ps(mul, mul)};

  sum = _mm_hadd_ps(sum, sum);

  return _mm_cvtss_f32(sum);
#else
  return dot(*this);
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr f32 Vector<f32, 3>::dot(Vector const &other) const noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_set_ps(0.0F, m_data[2], m_data[1], m_data[0])};
  __m128 b{_mm_set_ps(0.0F, other[2], other[1], other[0])};
  __m128 mul{_mm_mul_ps(a, b)};
  __m128 sum{_mm_hadd_ps(mul, mul)};

  sum = _mm_hadd_ps(sum, sum);

  return _mm_cvtss_f32(sum);
#else
  return (m_data[0] * other.m_data[0]) + (m_data[1] * other.m_data[1]) +
         (m_data[2] * other.m_data[2]);
#endif
}

template <>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<f32, 3> cross(
    Vector<f32, 3> const &lhs, Vector<f32, 3> const &rhs) noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_set_ps(0.0F, lhs[2], lhs[1], lhs[0])};
  __m128 b{_mm_set_ps(0.0F, rhs[2], rhs[1], rhs[0])};

  __m128 aYzx{_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1))};
  __m128 bYzx{_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1))};

  __m128 bZxy{_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2))};
  __m128 aZxy{_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2))};

  __m128 mul1{_mm_mul_ps(aYzx, bZxy)};
  __m128 mul2{_mm_mul_ps(aZxy, bYzx)};

  __m128 result{_mm_sub_ps(mul1, mul2)};

  alignas(16) std::array<f32, 4> data{};
  _mm_store_ps(data.data(), result);

  return {data[0], data[1], data[2]};
#else
  return {(lhs[1] * rhs[2]) - (lhs[2] * rhs[1]), (lhs[2] * rhs[0]) - (lhs[0] * rhs[2]),
          (lhs[0] * rhs[1]) - (lhs[1] * rhs[0])};
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr f32 Vector<f32, 4>::dot(Vector const &other) const noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 a{_mm_loadu_ps(m_data.data())};
  __m128 b{_mm_loadu_ps(other.m_data.data())};
  __m128 mul{_mm_mul_ps(a, b)};
  __m128 sum{_mm_hadd_ps(mul, mul)};

  sum = _mm_hadd_ps(sum, sum);

  return _mm_cvtss_f32(sum);
#else
  return (m_data[0] * other.m_data[0]) + (m_data[1] * other.m_data[1]) +
         (m_data[2] * other.m_data[2]) + (m_data[3] * other.m_data[3]);
#endif
}

template <>
PBRT_INLINE constexpr Vector<f64, 4> &Vector<f64, 4>::operator+=(Vector const &other) noexcept
{
#if defined(PBRT_HAS_AVX2)
  __m256d a{_mm256_load_pd(m_data.data())};
  __m256d b{_mm256_load_pd(other.data())};
  __m256d sum{_mm256_add_pd(a, b)};

  _mm256_store_pd(data(), sum);
  return *this;
#else
  for (usize i = 0; i < 4; ++i)
  {
    m_data[i] += other[i];
  }

  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Vector<f64, 4> &Vector<f64, 4>::operator-=(Vector const &other) noexcept
{
#if defined(PBRT_HAS_AVX2)
  __m256d a{_mm256_loadu_pd(m_data.data())};
  __m256d b{_mm256_loadu_pd(other.m_data.data())};

  __m256d result{_mm256_sub_pd(a, b)};
  _mm256_storeu_pd(m_data.data(), result);

  return *this;
#else
  for (usize i = 0; i < 4; ++i)
  {
    m_data[i] -= other[i];
  }
  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Vector<f64, 4> &Vector<f64, 4>::operator*=(Vector const &other) noexcept
{
#if defined(PBRT_HAS_AVX2)
  __m256d a{_mm256_loadu_pd(m_data.data())};
  __m256d b{_mm256_loadu_pd(other.m_data.data())};

  __m256d result{_mm256_mul_pd(a, b)};
  _mm256_storeu_pd(m_data.data(), result);

  return *this;
#else
  for (usize i = 0; i < 4; ++i)
  {
    m_data[i] *= other.m_data[i];
  }
  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Vector<f64, 4> &Vector<f64, 4>::operator*=(f64 const &scalar) noexcept
{
#if defined(PBRT_HAS_AVX2)
  __m256d a{_mm256_loadu_pd(m_data.data())};
  __m256d b{_mm256_set1_pd(scalar)};
  __m256d result{_mm256_mul_pd(a, b)};

  _mm256_storeu_pd(m_data.data(), result);

  return *this;
#else
  for (usize i = 0; i < 4; ++i)
  {
    m_data[i] *= scalar;
  }
  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Vector<f64, 4> &Vector<f64, 4>::operator/=(Vector const &other) noexcept
{
#if defined(PBRT_HAS_AVX2)
  __m256d a{_mm256_loadu_pd(m_data.data())};
  __m256d b{_mm256_loadu_pd(other.m_data.data())};

  __m256d result{_mm256_div_pd(a, b)};
  _mm256_storeu_pd(m_data.data(), result);

  return *this;
#else
  for (usize i = 0; i < 4; ++i)
  {
    m_data[i] /= other.m_data[i];
  }
  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Vector<f64, 4> &Vector<f64, 4>::operator/=(f64 const &scalar) noexcept
{
#if defined(PBRT_HAS_AVX2)
  __m256d a{_mm256_loadu_pd(m_data.data())};
  __m256d b{_mm256_set1_pd(scalar)};
  __m256d result{_mm256_div_pd(a, b)};

  _mm256_storeu_pd(m_data.data(), result);

  return *this;
#else
  for (usize i = 0; i < 4; ++i)
  {
    m_data[i] /= scalar;
  }
  return *this;
#endif
}

template <>
PBRT_INLINE constexpr bool Vector<f64, 4>::operator==(Vector const &other) const noexcept
{
#if defined(PBRT_HAS_AVX2)
  __m256d a{_mm256_loadu_pd(m_data.data())};
  __m256d b{_mm256_loadu_pd(other.m_data.data())};

  __m256d cmp{_mm256_cmp_pd(a, b, _CMP_EQ_OQ)};
  int mask{_mm256_movemask_pd(cmp)};

  return (mask & 0xF) == 0xF;
#else
  for (usize i = 0; i < 4; ++i)
  {
    if (m_data[i] != other.m_data[i])
    {
      return false;
    }
  }
  return true;
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr f64 Vector<f64, 2>::dot(Vector const &other) const noexcept
{
  return (m_data[0] * other.m_data[0]) + (m_data[1] * other.m_data[1]);
}

template <>
[[nodiscard]] PBRT_INLINE constexpr f64 Vector<f64, 3>::dot(Vector const &other) const noexcept
{
  return (m_data[0] * other.m_data[0]) + (m_data[1] * other.m_data[1]) +
         (m_data[2] * other.m_data[2]);
}

template <>
[[nodiscard]] PBRT_INLINE constexpr f64 Vector<f64, 4>::dot(Vector const &other) const noexcept
{
#if defined(PBRT_HAS_AVX2)
  __m256d a{_mm256_load_pd(m_data.data())};
  __m256d b{_mm256_load_pd(other.m_data.data())};

  __m256d mul{_mm256_mul_pd(a, b)};
  __m256d sum{_mm256_hadd_pd(mul, mul)};

  __m128d sumHigh{_mm256_extractf128_pd(sum, 1)};
  __m128d sumLow{_mm256_castpd256_pd128(sum)};
  __m128d finalSum{_mm_add_pd(sumHigh, sumLow)};

  return _mm_cvtsd_f64(finalSum);
#else
  return (m_data[0] * other.m_data[0]) + (m_data[1] * other.m_data[1]) +
         (m_data[2] * other.m_data[2]) + (m_data[3] * other.m_data[3]);
#endif
}
} // namespace pbrt::math
