#pragma once

#if !defined(PBRT_DISABLE_SIMD)
  #if defined(PBRT_HAS_AVX2)
    #include <immintrin.h>
  #endif

  #if defined(PBRT_HAS_SSE42)
    #include <smmintrin.h>
  #endif
#endif
