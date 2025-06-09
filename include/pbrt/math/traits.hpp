#pragma once

#include "pbrt/types.hpp"

#include <concepts>

namespace pbrt::math
{
template <typename T>
concept ScalarType = std::floating_point<T> || std::integral<T>;

template <typename T>
concept FloatingPoint = std::floating_point<T>;

template <typename T>
concept SignedIntegral = std::signed_integral<T>;

template <typename T>
concept UnsignedIntegral = std::unsigned_integral<T>;

template <typename T>
concept VectorLike = requires(T const &vector) {
  { vector.size() } -> std::convertible_to<usize>;
  { vector[0] } -> ScalarType;
  typename T::value_type;
  requires ScalarType<typename T::value_type>;
};

template <typename T>
concept MatrixLike = requires(T const &matrix) {
  { matrix.rows() } -> std::convertible_to<usize>;
  { matrix.cols() } -> std::convertible_to<usize>;
  { matrix(0, 0) } -> ScalarType;
  typename T::value_type;
  requires ScalarType<typename T::value_type>;
};
} // namespace pbrt::math
