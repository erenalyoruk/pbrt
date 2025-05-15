#pragma once

#if defined(PBRT_PLATFORM_WINDOWS) && defined(PBRT_BUILD_DLL)
  #if defined(PBRT_EXPORTS)
    #define PBRT_API __declspec(dllexport)
  #else
    #define PBRT_API __declspec(dllimport)
  #endif
#else
  #define PBRT_API
#endif
