#pragma once

#include "pbrt/types.hpp"

#include <concepts>

namespace pbrt::math
{
template <typename T>
concept scalar_type = std::floating_point<T> || std::integral<T>;

template <typename T>
concept floating_point = std::floating_point<T>;

template <typename T>
concept signed_integral = std::signed_integral<T>;

template <typename T>
concept unsigned_integral = std::unsigned_integral<T>;

template <typename T>
concept vector_like = requires(T const &vector) {
  { vector.size() } -> std::convertible_to<usize>;
  { vector[0] } -> scalar_type;
  typename T::value_type;
  requires scalar_type<typename T::value_type>;
};

template <typename T>
concept matrix_like = requires(T const &matrix) {
  { matrix.rows() } -> std::convertible_to<usize>;
  { matrix.cols() } -> std::convertible_to<usize>;
  { matrix(0, 0) } -> scalar_type;
  typename T::value_type;
  requires scalar_type<typename T::value_type>;
};
} // namespace pbrt::math
