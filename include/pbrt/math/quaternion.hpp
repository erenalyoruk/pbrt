#pragma once

#include "pbrt/export.hpp"
#include "pbrt/inline.hpp"
#include "pbrt/math/constants.hpp"
#include "pbrt/math/matrix.hpp"
#include "pbrt/math/traits.hpp"
#include "pbrt/math/vector.hpp"

#include <array>
#include <stdexcept>

namespace pbrt::math
{
/**
 * @brief A quaternion class for representing rotations in 3D space.
 *
 * Quaternions are represented as (w, x, y, z) where w is the scalar part
 * and (x, y, z) is the vector part. This representation is efficient for
 * 3D rotations and avoids gimbal lock.
 *
 * @tparam T Floating-point type (f32 or f64)
 */
template <FloatingPoint T>
class PBRT_API Quaternion
{
public:
  using ValueType = T;
  using VectorType = Vector<T, 3>;
  using SizeType = usize;

  /**
   * @brief Default constructor. Creates identity quaternion (1, 0, 0, 0).
   */
  PBRT_INLINE constexpr Quaternion() noexcept : m_components{1, 0, 0, 0}
  {
  }

  /**
   * @brief Construct quaternion from individual components.
   *
   * @param w Scalar component
   * @param x X component of vector part
   * @param y Y component of vector part
   * @param z Z component of vector part
   */
  PBRT_INLINE constexpr Quaternion(T const &w, T const &x, T const &y, T const &z) noexcept
      : m_components{w, x, y, z}
  {
  }

  /**
   * @brief Construct quaternion from scalar and vector parts.
   *
   * @param w Scalar component
   * @param v Vector component (x, y, z)
   */
  PBRT_INLINE constexpr Quaternion(T const &w, VectorType const &v) noexcept
      : m_components{w, v.x(), v.y(), v.z()}
  {
  }

  /**
   * @brief Construct quaternion from axis-angle representation.
   *
   * @param axis Normalized rotation axis
   * @param angleRadians Rotation angle in radians
   * @throws std::domain_error if axis is not normalized
   */
  PBRT_INLINE constexpr Quaternion(VectorType const &axis, T const &angleRadians)
  {
    if (std::abs(axis.length() - T{1}) > EPSILON<T>)
    {
      throw std::domain_error("Axis must be normalized");
    }

    const T halfAngle{angleRadians / T{2}};
    const T sinHalf{std::sin(halfAngle)};

    m_components[0] = std::cos(halfAngle); // w
    m_components[1] = axis.x() * sinHalf;  // x
    m_components[2] = axis.y() * sinHalf;  // y
    m_components[3] = axis.z() * sinHalf;  // z
  }

  /**
   * @brief Array subscript operator for component access.
   *
   * @param index Index of the component (0-3).
   * @return T const& Reference to the component at the given index.
   */
  PBRT_INLINE constexpr T const &operator[](SizeType index) const noexcept
  {
    return m_components[index];
  }

  /**
   * @brief Array subscript operator for component access.
   *
   * @param index Index of the component (0-3).
   * @return T& Reference to the component at the given index.
   */
  PBRT_INLINE constexpr T &operator[](SizeType index) noexcept
  {
    return m_components[index];
  }

  /**
   * @brief Component accessors for the quaternion.
   *
   * @return Returns the scalar and vector components of the quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr T w() const noexcept
  {
    return m_components[0];
  }

  /**
   * @brief Component accessors for the quaternion.
   *
   * @return Returns the scalar and vector components of the quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr T x() const noexcept
  {
    return m_components[1];
  }

  /**
   * @brief Component accessors for the quaternion.
   *
   * @return Returns the scalar and vector components of the quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr T y() const noexcept
  {
    return m_components[2];
  }

  /**
   * @brief Component accessors for the quaternion.
   *
   * @return Returns the scalar and vector components of the quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr T z() const noexcept
  {
    return m_components[3];
  }

  [[nodiscard]] PBRT_INLINE constexpr T &w() noexcept
  {
    return m_components[0];
  }

  /**
   * @brief Component accessors for the quaternion.
   *
   * @return Returns the scalar and vector components of the quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr T &x() noexcept
  {
    return m_components[1];
  }

  /**
   * @brief Component accessors for the quaternion.
   *
   * @return Returns the scalar and vector components of the quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr T &y() noexcept
  {
    return m_components[2];
  }

  /**
   * @brief Component accessors for the quaternion.
   *
   * @return Returns the scalar and vector components of the quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr T &z() noexcept
  {
    return m_components[3];
  }

  /**
   * @brief Get the vector part of the quaternion.
   *
   * @return VectorType containing the vector part (x, y, z).
   */
  [[nodiscard]] PBRT_INLINE constexpr VectorType vector() const noexcept
  {
    return {m_components[1], m_components[2], m_components[3]};
  }

  /**
   * @brief Get the scalar part of the quaternion.
   *
   * @return Scalar value representing the scalar part (w).
   */
  [[nodiscard]] PBRT_INLINE constexpr T scalar() const noexcept
  {
    return m_components[0];
  }

  /**
   * @brief Get the scalar part of the quaternion.
   *
   * @return T& representing the scalar part (w).
   */
  [[nodiscard]] PBRT_INLINE constexpr T &scalar() noexcept
  {
    return m_components[0];
  }

  /**
   * @brief Add another quaternion to this one.
   */
  PBRT_INLINE constexpr Quaternion &operator+=(Quaternion const &other) noexcept
  {
    m_components[0] += other.m_components[0];
    m_components[1] += other.m_components[1];
    m_components[2] += other.m_components[2];
    m_components[3] += other.m_components[3];
    return *this;
  }

  /**
   * @brief Subtract another quaternion from this one.
   */
  PBRT_INLINE constexpr Quaternion &operator-=(Quaternion const &other) noexcept
  {
    m_components[0] -= other.m_components[0];
    m_components[1] -= other.m_components[1];
    m_components[2] -= other.m_components[2];
    m_components[3] -= other.m_components[3];
    return *this;
  }

  /**
   * @brief Multiply this quaternion by a scalar.
   */
  PBRT_INLINE constexpr Quaternion &operator*=(T const &scalar) noexcept
  {
    m_components[0] *= scalar;
    m_components[1] *= scalar;
    m_components[2] *= scalar;
    m_components[3] *= scalar;
    return *this;
  }

  /**
   * @brief Multiply this quaternion by another quaternion (Hamilton product).
   */
  PBRT_INLINE constexpr Quaternion &operator*=(Quaternion const &other) noexcept
  {
    T w = m_components[0] * other.m_components[0] - m_components[1] * other.m_components[1] -
          m_components[2] * other.m_components[2] - m_components[3] * other.m_components[3];
    T x = m_components[0] * other.m_components[1] + m_components[1] * other.m_components[0] +
          m_components[2] * other.m_components[3] - m_components[3] * other.m_components[2];
    T y = m_components[0] * other.m_components[2] - m_components[1] * other.m_components[3] +
          m_components[2] * other.m_components[0] + m_components[3] * other.m_components[1];
    T z = m_components[0] * other.m_components[3] + m_components[1] * other.m_components[2] -
          m_components[2] * other.m_components[1] + m_components[3] * other.m_components[0];

    m_components = {w, x, y, z};
    return *this;
  }

  /**
   * @brief Add two quaternions.
   */
  [[nodiscard]] PBRT_INLINE constexpr Quaternion operator+(Quaternion const &other) const noexcept
  {
    Quaternion result{*this};
    result += other;
    return result;
  }

  /**
   * @brief Subtract two quaternions.
   */
  [[nodiscard]] PBRT_INLINE constexpr Quaternion operator-(Quaternion const &other) const noexcept
  {
    Quaternion result{*this};
    result -= other;
    return result;
  }

  /**
   * @brief Negate quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr Quaternion operator-() const noexcept
  {
    return {-m_components[0], -m_components[1], -m_components[2], -m_components[3]};
  }

  /**
   * @brief Multiply quaternion by scalar.
   */
  [[nodiscard]] PBRT_INLINE constexpr Quaternion operator*(T const &scalar) const noexcept
  {
    Quaternion result{*this};
    result *= scalar;
    return result;
  }

  /**
   * @brief Multiply two quaternions (Hamilton product).
   */
  [[nodiscard]] PBRT_INLINE constexpr Quaternion operator*(Quaternion const &other) const noexcept
  {
    Quaternion result{*this};
    result *= other;
    return result;
  }

  /**
   * @brief Check equality with epsilon tolerance.
   */
  [[nodiscard]] PBRT_INLINE constexpr bool operator==(Quaternion const &other) const noexcept
  {
    if constexpr (std::is_same_v<T, float>)
    {
      return approx_equal(other, EPSILON<float>);
    }
    else if constexpr (std::is_same_v<T, double>)
    {
      return approx_equal(other, PRECISION_EPSILON<double>);
    }
  }

  /**
   * @brief Check inequality.
   */
  [[nodiscard]] PBRT_INLINE constexpr bool operator!=(Quaternion const &other) const noexcept
  {
    return !(*this == other);
  }

  /**
   * @brief Check approximate equality with custom epsilon.
   */
  [[nodiscard]] PBRT_INLINE constexpr bool approx_equal(
      Quaternion const &other, T const &epsilon = EPSILON<T>) const noexcept
  {
    return std::abs(m_components[0] - other.m_components[0]) <= epsilon &&
           std::abs(m_components[1] - other.m_components[1]) <= epsilon &&
           std::abs(m_components[2] - other.m_components[2]) <= epsilon &&
           std::abs(m_components[3] - other.m_components[3]) <= epsilon;
  }

  /**
   * @brief Calculate the magnitude (norm) of the quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr T length() const noexcept
  {
    return std::sqrt(length_squared());
  }

  /**
   * @brief Calculate the squared magnitude of the quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr T length_squared() const noexcept
  {
    return m_components[0] * m_components[0] + m_components[1] * m_components[1] +
           m_components[2] * m_components[2] + m_components[3] * m_components[3];
  }

  /**
   * @brief Normalize this quaternion in place.
   *
   * @throws std::domain_error if the quaternion is zero (length is zero).
   */
  PBRT_INLINE constexpr void normalize()
  {
    const T len{length()};
    if (len <= EPSILON<T>)
    {
      throw std::domain_error("Cannot normalize zero quaternion");
    }

    const T invLen{T{1} / len};
    m_components[0] *= invLen;
    m_components[1] *= invLen;
    m_components[2] *= invLen;
    m_components[3] *= invLen;
  }

  /**
   * @brief Return normalized copy of this quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr Quaternion normalized() const
  {
    Quaternion result{*this};
    result.normalize();
    return result;
  }

  /**
   * @brief Return the conjugate of this quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr Quaternion conjugate() const noexcept
  {
    return {m_components[0], -m_components[1], -m_components[2], -m_components[3]};
  }

  /**
   * @brief Return the inverse of this quaternion.
   *
   * @throws std::domain_error if the quaternion is zero (length is zero).
   */
  [[nodiscard]] PBRT_INLINE constexpr Quaternion inverse() const
  {
    const T lenSquared{length_squared()};
    if (lenSquared <= EPSILON<T>)
    {
      throw std::domain_error("Cannot invert zero quaternion");
    }

    const T invLenSq = T{1} / lenSquared;
    return conjugate() * invLenSq;
  }

  /**
   * @brief Rotate a 3D vector by this quaternion.
   */
  [[nodiscard]] PBRT_INLINE constexpr VectorType rotate(VectorType const &v) const noexcept
  {
    const VectorType qvec{m_components[1], m_components[2], m_components[3]};
    const VectorType uv{cross(qvec, v)};
    const VectorType uuv{cross(qvec, uv)};

    return v + ((uv * m_components[0]) + uuv) * T{2};
  }

  /**
   * @brief Convert to axis-angle representation.
   *
   * @return std::pair<VectorType, T> Axis and angle in radians
   */
  [[nodiscard]] PBRT_INLINE constexpr std::pair<VectorType, T> to_axis_angle() const noexcept
  {
    const T vecLength{std::sqrt(m_components[1] * m_components[1] +
                                m_components[2] * m_components[2] +
                                m_components[3] * m_components[3])};

    if (vecLength < EPSILON<T>)
    {
      return {VectorType::unit_x(), T{0}};
    }

    const T angle{T{2} * std::atan2(vecLength, m_components[0])};
    VectorType axis{m_components[1] / vecLength, m_components[2] / vecLength,
                    m_components[3] / vecLength};

    return {axis, angle};
  }

  /**
   * @brief Create identity quaternion.
   */
  [[nodiscard]] PBRT_INLINE static constexpr Quaternion identity() noexcept
  {
    return {1, 0, 0, 0};
  }

  /**
   * @brief Create quaternion from Euler angles (XYZ order).
   */
  [[nodiscard]] PBRT_INLINE static constexpr Quaternion from_euler(T const &yaw, T const &pitch,
                                                                   T const &roll) noexcept
  {
    const T cy = std::cos(yaw * T{0.5});
    const T sy = std::sin(yaw * T{0.5});
    const T cp = std::cos(pitch * T{0.5});
    const T sp = std::sin(pitch * T{0.5});
    const T cr = std::cos(roll * T{0.5});
    const T sr = std::sin(roll * T{0.5});

    return {cy * cp * cr + sy * sp * sr, cy * cp * sr - sy * sp * cr, sy * cp * sr + cy * sp * cr,
            sy * cp * cr - cy * sp * sr};
  }

  /**
   * @brief Create quaternion from axis-angle representation.
   *
   * @param axis Normalized rotation axis (must be unit length)
   * @param angleRadians Rotation angle in radians
   * @return Quaternion representing the rotation
   */
  [[nodiscard]] PBRT_INLINE static constexpr Quaternion from_axis_angle(VectorType const &axis,
                                                                        T const &angleRadians)
  {
    return {axis, angleRadians};
  }

  /**
   * @brief Convert quaternion to a 4x4 rotation matrix.
   *
   * @return Matrix<T, 4, 4> Matrix that represents the rotation
   */
  [[nodiscard]] PBRT_INLINE constexpr Matrix<T, 4, 4> to_matrix() const noexcept
  {
    const T w2{m_components[0] * m_components[0]};
    const T x2{m_components[1] * m_components[1]};
    const T y2{m_components[2] * m_components[2]};
    const T z2{m_components[3] * m_components[3]};

    const T wx{m_components[0] * m_components[1]};
    const T wy{m_components[0] * m_components[2]};
    const T wz{m_components[0] * m_components[3]};
    const T xy{m_components[1] * m_components[2]};
    const T xz{m_components[1] * m_components[3]};
    const T yz{m_components[2] * m_components[3]};

    auto result{Matrix<T, 4, 4>::identity()};

    result(0, 0) = w2 + x2 - y2 - z2;
    result(0, 1) = T{2} * (xy - wz);
    result(0, 2) = T{2} * (xz + wy);

    result(1, 0) = T{2} * (xy + wz);
    result(1, 1) = w2 - x2 + y2 - z2;
    result(1, 2) = T{2} * (yz - wx);

    result(2, 0) = T{2} * (xz - wy);
    result(2, 1) = T{2} * (yz + wx);
    result(2, 2) = w2 - x2 - y2 + z2;

    return result;
  }

private:
  std::array<T, 4> m_components;
};

/**
 * @brief Scalar multiplication (scalar * quaternion).
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Quaternion<T> operator*(
    T const &scalar, Quaternion<T> const &q) noexcept
{
  return {q.w() * scalar, q.x() * scalar, q.y() * scalar, q.z() * scalar};
}

/**
 * @brief Calculate dot product of two quaternions.
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr T dot(Quaternion<T> const &lhs,
                                                   Quaternion<T> const &rhs) noexcept
{
  return lhs.w() * rhs.w() + lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
}

/**
 * @brief Spherical linear interpolation between two quaternions.
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Quaternion<T> slerp(Quaternion<T> const &q1,
                                                                 Quaternion<T> const &q2,
                                                                 T const &t) noexcept
{
  T dotProduct{dot(q1, q2)};

  Quaternion<T> q2Adjusted{q2};
  if (dotProduct < T{0})
  {
    q2Adjusted = -q2Adjusted;
    dotProduct = -dotProduct;
  }

  if (dotProduct > T{0.9995})
  {
    Quaternion<T> result{q1 + t * (q2Adjusted - q1)};
    return result.normalized();
  }

  const T theta0{std::acos(std::abs(dotProduct))};
  const T theta{theta0 * t};
  const T sinTheta{std::sin(theta)};
  const T sinTheta0{std::sin(theta0)};

  const T s0{std::cos(theta) - (dotProduct * sinTheta / sinTheta0)};
  const T s1{sinTheta / sinTheta0};

  return (s0 * q1) + (s1 * q2Adjusted);
}

/**
 * @brief Create rotation matrix from quaternion.
 *
 * @tparam T Floating-point type
 * @param q Quaternion representing the rotation
 * @return Matrix<T, 4, 4> 4x4 rotation matrix
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, 4, 4> rotate(Quaternion<T> const &q) noexcept
{
  const T w2{q.w() * q.w()};
  const T x2{q.x() * q.x()};
  const T y2{q.y() * q.y()};
  const T z2{q.z() * q.z()};

  const T wx{q.w() * q.x()};
  const T wy{q.w() * q.y()};
  const T wz{q.w() * q.z()};
  const T xy{q.x() * q.y()};
  const T xz{q.x() * q.z()};
  const T yz{q.y() * q.z()};

  auto result{Matrix<T, 4, 4>::identity()};

  result(0, 0) = w2 + x2 - y2 - z2;
  result(0, 1) = T{2} * (xy - wz);
  result(0, 2) = T{2} * (xz + wy);

  result(1, 0) = T{2} * (xy + wz);
  result(1, 1) = w2 - x2 + y2 - z2;
  result(1, 2) = T{2} * (yz - wx);

  result(2, 0) = T{2} * (xz - wy);
  result(2, 1) = T{2} * (yz + wx);
  result(2, 2) = w2 - x2 - y2 + z2;

  return result;
}

using Quatf = Quaternion<float>;
using Quatd = Quaternion<double>;
using Quat = Quatf;
} // namespace pbrt::math
