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
      #define PBRT_API __attribute__((visibility("default")))
    #endif
  #endif
#else
  #define PBRT_API
#endif

#if defined(PBRT_COMPILER_MSVC)
  #define PBRT_FORCE_INLINE __forceinline
  #define PBRT_NO_INLINE __declspec(noinline)
#elif defined(PBRT_COMPILER_GCC)
  #define PBRT_FORCE_INLINE inline __attribute__((always_inline))
  #define PBRT_NO_INLINE __attribute__((noinline))
#else
  #define PBRT_FORCE_INLINE inline
  #define PBRT_NO_INLINE
#endif

#define PBRT_INLINE inline
#define PBRT_CONSTEXPR constexpr
#define PBRT_CONSTEVAL consteval

#if defined(PBRT_COMPILER_GCC)
  #define PBRT_HOT __attribute__((hot))
  #define PBRT_COLD __attribute__((cold))
#else
  #define PBRT_HOT
  #define PBRT_COLD
#endif

#if __has_cpp_attribute(nodiscard)
  #define PBRT_NODISCARD [[nodiscard]]
#else
  #define PBRT_NODISCARD
#endif
