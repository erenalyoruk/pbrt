#pragma once

#if defined(PBRT_COMPILER_MSVC)
  #define PBRT_INLINE __forceinline
#elif defined(PBRT_COMPILER_GCC)
  #define PBRT_INLINE inline __attribute__((always_inline))
#else
  #define PBRT_INLINE inline
#endif
