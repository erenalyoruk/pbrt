#pragma once

#include "pbrt/types.hpp"

namespace pbrt
{
template <typename T>
struct AlignmentOf
{
  static constexpr usize value{alignof(T)};
};

#if defined(PBRT_HAS_SSE42)
template <>
struct AlignmentOf<f64>
{
  static constexpr usize value{32};
};

template <>
struct AlignmentOf<f32>
{
  static constexpr usize value{32};
};
#endif

#define ALIGN_OF(T) AlignmentOf<T>::value
#define ALIGNED(T) alignas(ALIGN_OF(T))
} // namespace pbrt
