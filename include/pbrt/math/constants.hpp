#pragma once

#include "pbrt/math/traits.hpp"
#include "pbrt/types.hpp"

#include <cmath>
#include <limits>
#include <numbers>

namespace pbrt::math
{
/**
 * @brief Mathematical constant PI.
 *
 * @tparam T Floating-point type (e.g., float, double)
 */
template <FloatingPoint T>
constexpr T PI{std::numbers::pi_v<T>};

/**
 * @brief STD epsilon value for floating-point comparisons (smallest difference between two).
 *
 * @tparam T Floating-point type (e.g., float, double)
 */
template <FloatingPoint T>
constexpr T STD_EPSILON{std::numeric_limits<T>::epsilon()};

/**
 * @brief Practical epsilon value for floating-point comparisons, used to determine if two values
 * are approximately equal.
 *
 * @tparam T Floating-point type (e.g., float, double)
 */
template <FloatingPoint T>
constexpr T EPSILON{1e-6};

/**
 * @brief High precision epsilon value for floating-point comparisons.
 *
 * @tparam T Floating-point type (e.g., float, double)
 */
template <FloatingPoint T>
constexpr T PRECISION_EPSILON{1e-10};

/**
 * @brief A loose epsilon value for floating-point comparisons, used for less strict equality
 * checks.
 *
 * @tparam T Floating-point type (e.g., float, double)
 */
template <FloatingPoint T>
constexpr T LOOSE_EPSILON{1e-4};

/**
 * @brief A very small epsilon value used for floating-point comparisons, often used to
 *
 * @tparam T Floating-point type (e.g., float, double)
 */
template <FloatingPoint T>
constexpr T ZERO_EPSILON{1e-8};

/**
 * @brief A constant epsilon value for floating-point comparisons, defined as 10 raised to the power
 * of a specified exponent.
 *
 * @tparam T Floating-point type (e.g., float, double)
 * @tparam Exponent The exponent to which 10 is raised, defaulting to -6.
 */
template <FloatingPoint T, i8 Exponent = -6>
const T EPSILON_E{T{1} * std::pow(T{10}, T{Exponent})};

/**
 * @brief Square root of 2 constant.
 *
 * @tparam T Floating-point type (e.g., float, double)
 */
template <FloatingPoint T>
constexpr T SQRT_2{std::numbers::sqrt2_v<T>};

/**
 * @brief Square root of 2 divided by 2 constant.
 *
 * This is often used in graphics and geometry calculations.
 *
 * @tparam T Floating-point type (e.g., float, double)
 */
template <FloatingPoint T>
constexpr T SQRT_HALF_2{std::numbers::sqrt2_v<T> / T{2}};
} // namespace pbrt::math
