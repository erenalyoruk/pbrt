#pragma once

#include <type_traits>

namespace pbrt::math
{
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

template <typename T, typename U>
concept ConvertibleTo = std::is_convertible_v<T, U>;
} // namespace pbrt::math
