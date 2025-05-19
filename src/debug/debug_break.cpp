#include "pbrt/debug/debug_break.hpp"

#if defined(PBRT_PLATFORM_WINDOWS)
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

  #define PBRT_DEBUG_BREAK() DebugBreak()
#else
  #error "Unsupported platform for debug break"
#endif

namespace pbrt::debug
{
void debug_break()
{
  PBRT_DEBUG_BREAK();
}
} // namespace pbrt::debug
