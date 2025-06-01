#pragma once

#include "pbrt/assertions.hpp"
#include "pbrt/export.hpp"
#include "pbrt/inline.hpp"
#include "pbrt/math/constants.hpp"
#include "pbrt/math/coordinate_system.hpp"
#include "pbrt/math/matrix.hpp"
#include "pbrt/math/traits.hpp"
#include "pbrt/math/vector.hpp"
#include "pbrt/types.hpp"

namespace pbrt::math
{
/**
 * @brief Create a translation matrix.
 *
 * @tparam T Floating-point type
 * @tparam N Dimension (2 for 2D, 3 for 3D)
 * @param translation Translation vector
 * @return Transformation matrix (3x3 for 2D, 4x4 for 3D)
 */
template <FloatingPoint T, usize N>
  requires(N == 2 || N == 3)
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, N + 1, N + 1> translate(
    Vector<T, N> const &translation) noexcept
{
  auto result{Matrix<T, N + 1, N + 1>::identity()};

  if constexpr (N == 2)
  {
    result(0, 2) = translation.x();
    result(1, 2) = translation.y();
  }
  else if constexpr (N == 3)
  {
    result(0, 3) = translation.x();
    result(1, 3) = translation.y();
    result(2, 3) = translation.z();
  }

  return result;
}

/**
 * @brief Create a translation matrix from individual components.
 *
 * @tparam T Floating-point type
 * @tparam Args Component types (must be convertible to T)
 * @param args Translation components (2 for 2D, 3 for 3D)
 * @return Transformation matrix (3x3 for 2D, 4x4 for 3D)
 */
template <FloatingPoint T, ConvertibleTo<T>... Args>
  requires(sizeof...(Args) >= 2 && sizeof...(Args) <= 3)
[[nodiscard]] PBRT_API PBRT_INLINE constexpr auto translate(Args &&...args) noexcept
{
  PBRT_STATIC_ASSERT(sizeof...(Args) == 2 || sizeof...(Args) == 3,
                     "translate requires 2 or 3 arguments");

  if constexpr (sizeof...(Args) == 2)
  {
    return translate(Vector<T, 2>{static_cast<T>(std::forward<Args>(args))...});
  }
  else
  {
    return translate(Vector<T, 3>{static_cast<T>(std::forward<Args>(args))...});
  }
}

/**
 * @brief Create a uniform scaling matrix.
 *
 * @tparam T Floating-point type
 * @tparam N Dimension (2 for 2D, 3 for 3D)
 * @param scaleFactor Uniform scale factor
 * @return Transformation matrix (3x3 for 2D, 4x4 for 3D)
 */
template <FloatingPoint T, usize N>
  requires(N == 2 || N == 3)
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, N + 1, N + 1> scale(
    T const &scaleFactor) noexcept
{
  auto result{Matrix<T, N + 1, N + 1>::identity()};

  if constexpr (N == 2)
  {
    result(0, 0) = scaleFactor;
    result(1, 1) = scaleFactor;
  }
  else if constexpr (N == 3)
  {
    result(0, 0) = scaleFactor;
    result(1, 1) = scaleFactor;
    result(2, 2) = scaleFactor;
  }

  return result;
}

/**
 * @brief Create a uniform scaling matrix from a vector.
 *
 * @tparam T Floating-point type
 * @tparam N Dimension (2 for 2D, 3 for 3D)
 */
template <FloatingPoint T, usize N>
  requires(N == 2 || N == 3)
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, N + 1, N + 1> scale(
    Vector<T, N> const &scale) noexcept
{
  auto result{Matrix<T, N + 1, N + 1>::identity()};

  if constexpr (N == 2)
  {
    result(0, 0) = scale.x();
    result(1, 1) = scale.y();
  }
  else if constexpr (N == 3)
  {
    result(0, 0) = scale.x();
    result(1, 1) = scale.y();
    result(2, 2) = scale.z();
  }

  return result;
}

/**
 * @brief Create a uniform scaling matrix from individual components.
 *
 * @tparam T Floating-point type
 * @tparam Args Component types (must be convertible to T)
 * @param args Scale components (2 for 2D, 3 for 3D)
 * @return Transformation matrix (3x3 for 2D, 4x4 for 3D)
 */
template <FloatingPoint T, ConvertibleTo<T>... Args>
  requires(sizeof...(Args) >= 2 && sizeof...(Args) <= 3)
[[nodiscard]] PBRT_API PBRT_INLINE constexpr auto scale(Args &&...args) noexcept
{
  PBRT_STATIC_ASSERT(sizeof...(Args) == 2 || sizeof...(Args) == 3,
                     "scale requires 2 or 3 arguments");

  if constexpr (sizeof...(Args) == 2)
  {
    return scale(Vector<T, 2>{static_cast<T>(std::forward<Args>(args))...});
  }
  else
  {
    return scale(Vector<T, 3>{static_cast<T>(std::forward<Args>(args))...});
  }
}

/**
 * @brief Create a 2D rotation matrix
 *
 * @tparam T Floating-point type
 * @param angleRadians Rotation angle in radians
 * @return 3x3 transformation matrix for 2D rotation
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, 3, 3> rotate(T const &angleRadians) noexcept
{
  auto result{Matrix<T, 3, 3>::identity()};
  T const cosTheta{std::cos(angleRadians)};
  T const sinTheta{std::sin(angleRadians)};

  result(0, 0) = cosTheta;
  result(0, 1) = -sinTheta;
  result(1, 0) = sinTheta;
  result(1, 1) = cosTheta;

  return result;
}

/**
 * @brief Create a 3D rotation matrix around X axis
 * @tparam T Floating-point type
 * @param angleRadians Rotation angle in radians
 * @return 4x4 transformation matrix for rotation around X axis
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, 4, 4> rotate_x(
    T const &angleRadians) noexcept
{
  auto result{Matrix<T, 4, 4>::identity()};
  T const cosTheta{std::cos(angleRadians)};
  T const sinTheta{std::sin(angleRadians)};

  result(1, 1) = cosTheta;
  result(1, 2) = -sinTheta;
  result(2, 1) = sinTheta;
  result(2, 2) = cosTheta;

  return result;
}

/**
 * @brief Create a 3D rotation matrix around Y axis
 * @tparam T Floating-point type
 * @param angleRadians Rotation angle in radians
 * @return 4x4 transformation matrix for rotation around Y axis
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, 4, 4> rotate_y(
    T const &angleRadians) noexcept
{
  auto result{Matrix<T, 4, 4>::identity()};
  T const cosTheta{std::cos(angleRadians)};
  T const sinTheta{std::sin(angleRadians)};

  result(0, 0) = cosTheta;
  result(0, 2) = sinTheta;
  result(2, 0) = -sinTheta;
  result(2, 2) = cosTheta;

  return result;
}

/**
 * @brief Create a 3D rotation matrix around Z axis
 * @tparam T Floating-point type
 * @param angleRadians Rotation angle in radians
 * @return 4x4 transformation matrix for rotation around Z axis
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, 4, 4> rotate_z(
    T const &angleRadians) noexcept
{
  auto result{Matrix<T, 4, 4>::identity()};
  T const cosTheta{std::cos(angleRadians)};
  T const sinTheta{std::sin(angleRadians)};

  result(0, 0) = cosTheta;
  result(0, 1) = -sinTheta;
  result(1, 0) = sinTheta;
  result(1, 1) = cosTheta;

  return result;
}

/**
 * @brief Create a 3D rotation matrix around arbitrary axis
 * @tparam T Floating-point type
 * @param axis Normalized rotation axis
 * @param angleRadians Rotation angle in radians
 * @return 4x4 transformation matrix for rotation around arbitrary axis
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, 4, 4> rotate(Vector<T, 3> const &axis,
                                                                    T const &angleRadians) noexcept
{
  auto result{Matrix<T, 4, 4>::identity()};
  T const cosTheta{std::cos(angleRadians)};
  T const sinTheta{std::sin(angleRadians)};
  T const oneMinusCos{1 - cosTheta};

  result(0, 0) = cosTheta + axis.x * axis.x * oneMinusCos;
  result(0, 1) = axis.x * axis.y * oneMinusCos - axis.z * sinTheta;
  result(0, 2) = axis.x * axis.z * oneMinusCos + axis.y * sinTheta;

  result(1, 0) = axis.y * axis.x * oneMinusCos + axis.z * sinTheta;
  result(1, 1) = cosTheta + axis.y * axis.y * oneMinusCos;
  result(1, 2) = axis.y * axis.z * oneMinusCos - axis.x * sinTheta;

  result(2, 0) = axis.z * axis.x * oneMinusCos - axis.y * sinTheta;
  result(2, 1) = axis.z * axis.y * oneMinusCos + axis.x * sinTheta;
  result(2, 2) = cosTheta + axis.z * axis.z * oneMinusCos;

  return result;
}

/**
 * @brief Create a look-at transformation matrix.
 *
 * @tparam T Floating-point type
 * @tparam System Coordinate system type (default is RightHanded)
 * @param eye Camera position in world coordinates
 * @param target Point the camera is looking at
 * @param up Up direction vector for the camera
 * @return Matrix<T, 4, 4> Look-at transformation matrix
 */
template <FloatingPoint T, CoordinateSystem System = CoordinateSystem::RightHanded>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, 4, 4> look_at(
    Vector<T, 3> const &eye, Vector<T, 3> const &target, Vector<T, 3> const &up) noexcept
{
  auto const forward{normalize(target - eye)};

  PBRT_ASSERT_MSG(length_squared(cross(forward, up)) > EPSILON<T>,
                  "Up vector is collinear with forward vector");

  auto const right{normalize(cross(forward, up))};
  auto const cameraUp{cross(right, forward)};

  Matrix<T, 4, 4> result{Matrix<T, 4, 4>::identity()};

  if constexpr (System == CoordinateSystem::RightHanded)
  {
    result(0, 0) = right.x;
    result(1, 0) = right.y;
    result(2, 0) = right.z;

    result(0, 1) = cameraUp.x;
    result(1, 1) = cameraUp.y;
    result(2, 1) = cameraUp.z;

    result(0, 2) = -forward.x;
    result(1, 2) = -forward.y;
    result(2, 2) = -forward.z;
    result(2, 3) = dot(forward, eye);
  }
  else
  {
    // Left-handed system
    result(0, 0) = -right.x;
    result(1, 0) = -right.y;
    result(2, 0) = -right.z;

    result(0, 1) = cameraUp.x;
    result(1, 1) = cameraUp.y;
    result(2, 1) = cameraUp.z;

    result(0, 2) = forward.x;
    result(1, 2) = forward.y;
    result(2, 2) = forward.z;
    result(2, 3) = -dot(forward, eye);
  }

  // Translation part
  result(0, 3) = -dot(right, eye);
  result(1, 3) = -dot(cameraUp, eye);

  return result;
}

/**
 * @brief Create a perspective projection matrix.
 *
 * @tparam T Floating-point type
 * @param fovRadians Field of view in radians
 * @param aspectRatio Aspect ratio (width / height)
 * @param nearPlane Distance to the near clipping plane
 * @param farPlane Distance to the far clipping plane
 * @return Matrix<T, 4, 4> Perspective projection matrix
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, 4, 4> perspective(T const &fovRadians,
                                                                         T const &aspectRatio,
                                                                         T const &nearPlane,
                                                                         T const &farPlane) noexcept
{
  PBRT_ASSERT(nearPlane > 0 && farPlane > nearPlane);

  auto result{Matrix<T, 4, 4>::zero()};
  T const tanHalfFov{std::tan(fovRadians / 2)};
  T const zRange{farPlane - nearPlane};

  result(0, 0) = 1 / (aspectRatio * tanHalfFov);
  result(1, 1) = 1 / tanHalfFov;
  result(2, 2) = -(farPlane + nearPlane) / zRange;
  result(2, 3) = -2 * farPlane * nearPlane / zRange;
  result(3, 2) = -1;

  return result;
}

/**
 * @brief Create an orthographic projection matrix.
 *
 * @tparam T Floating-point type
 * @param left Left coordinate of the orthographic box
 * @param right Right coordinate of the orthographic box
 * @param bottom Bottom coordinate of the orthographic box
 * @param top Top coordinate of the orthographic box
 * @param nearPlane Distance to the near clipping plane
 * @param farPlane Distance to the far clipping plane
 * @return Matrix<T, 4, 4> Orthographic projection matrix
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, 4, 4> orthographic(
    T const &left, T const &right, T const &bottom, T const &top, T const &nearPlane,
    T const &farPlane) noexcept
{
  PBRT_ASSERT(nearPlane < farPlane);

  auto result{Matrix<T, 4, 4>::zero()};
  T const xScale{2 / (right - left)};
  T const yScale{2 / (top - bottom)};
  T const zScale{1 / (farPlane - nearPlane)};

  result(0, 0) = xScale;
  result(1, 1) = yScale;
  result(2, 2) = zScale;
  result(0, 3) = -(right + left) * xScale / 2;
  result(1, 3) = -(top + bottom) * yScale / 2;
  result(2, 3) = -nearPlane * zScale;

  result(3, 3) = 1;

  return result;
}

/**
 * @brief Extract translation component from transformation matrix
 *
 * @tparam T Floating-point type
 * @tparam N Dimension (3 for 3D, 4 for 4D)
 */
template <FloatingPoint T, usize N>
  requires(N == 3 || N == 4)
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, N - 1> extract_translation(
    Matrix<T, N, N> const &transform) noexcept
{
  if constexpr (N == 3)
  {
    return Vector<T, 2>{transform(0, 2), transform(1, 2)};
  }
  else // N == 4
  {
    return Vector<T, 3>{transform(0, 3), transform(1, 3), transform(2, 3)};
  }
}

/**
 * @brief Extract scale component from transformation matrix
 *
 * @tparam T Floating-point type
 * @tparam N Dimension (3 for 3D, 4 for 4D)
 */
template <FloatingPoint T, usize N>
  requires(N == 3 || N == 4)
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Vector<T, N - 1> extract_scale(
    Matrix<T, N, N> const &transform) noexcept
{
  if constexpr (N == 3)
  {
    Vector<T, 2> xAxis{transform(0, 0), transform(1, 0)};
    Vector<T, 2> yAxis{transform(0, 1), transform(1, 1)};
    return Vector<T, 2>{length(xAxis), length(yAxis)};
  }
  else // N == 4
  {
    Vector<T, 3> xAxis{transform(0, 0), transform(1, 0), transform(2, 0)};
    Vector<T, 3> yAxis{transform(0, 1), transform(1, 1), transform(2, 1)};
    Vector<T, 3> zAxis{transform(0, 2), transform(1, 2), transform(2, 2)};
    return Vector<T, 3>{length(xAxis), length(yAxis), length(zAxis)};
  }
}

/**
 * @brief Check if matrix is affine transformation
 *
 * @tparam T Floating-point type
 * @tparam N Dimension (3 for 3D)
 */
template <FloatingPoint T, usize N>
  requires(N == 3 || N == 4)
[[nodiscard]] PBRT_API PBRT_INLINE constexpr bool is_affine(
    Matrix<T, N, N> const &transform) noexcept
{
  if constexpr (N == 3)
  {
    return transform(2, 0) == T(0) && transform(2, 1) == T(0) && transform(2, 2) == T(1);
  }
  else // N == 4
  {
    return transform(3, 0) == T(0) && transform(3, 1) == T(0) && transform(3, 2) == T(0) &&
           transform(3, 3) == T(1);
  }
}

/**
 * @brief Convert degrees to radians
 * @tparam T Floating-point type
 * @param degrees Angle in degrees
 * @return Angle in radians
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr T radians(T const &degrees) noexcept
{
  return degrees * (PI<T> / T(180));
}

/**
 * @brief Convert radians to degrees
 * @tparam T Floating-point type
 * @param radians Angle in radians
 * @return Angle in degrees
 */
template <FloatingPoint T>
[[nodiscard]] PBRT_API PBRT_INLINE constexpr T degrees(T const &radians) noexcept
{
  return radians * (T(180) / PI<T>);
}
} // namespace pbrt::math
