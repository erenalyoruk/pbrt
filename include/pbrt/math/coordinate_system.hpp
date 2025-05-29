#pragma once

#include "pbrt/types.hpp"

namespace pbrt::math
{
/**
 * @brief Enumeration for coordinate systems.
 */
enum class CoordinateSystem : u8
{
  RightHanded = 0, // Right-handed coordinate system
  LeftHanded = 1,  // Left-handed coordinate system
};
} // namespace pbrt::math
