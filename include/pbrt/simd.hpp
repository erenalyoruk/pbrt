#pragma once

#if !defined(PBRT_DISABLE_SIMD)
  #if defined(PBRT_SIMD_LEVEL_AVX2)
    #include <immintrin.h>
  #elif defined(PBRT_SIMD_LEVEL_SSE41)
    #include <smmintrin.h>
  #endif
#endif
