#pragma once

#if defined(PBRT_BUILD_SHARED)
  #if defined(PBRT_EXPORTS)
    #if defined(PBRT_PLATFORM_WINDOWS)
      #define PBRT_API __declspec(dllexport)
    #else
      #define PBRT_API __attribute__((visibility("default")))
    #endif
  #else
    #if defined(PBRT_PLATFORM_WINDOWS)
      #define PBRT_API __declspec(dllimport)
    #else
      #define PBRT_API
    #endif
  #endif
#else
  #define PBRT_API
#endif
