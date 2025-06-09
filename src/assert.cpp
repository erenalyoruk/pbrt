#include "pbrt/assert.hpp"

#include <iostream>
#include <source_location>

#if defined(PBRT_PLATFORM_WINDOWS) && defined(PBRT_BUILD_DEBUG)
  #include <intrin.h>
  #define PBRT_DEBUG_BREAK() __debugbreak()
#elif defined(PBRT_COMPILER_GCC) && defined(PBRT_BUILD_DEBUG)
  #include <csignal>
  #define PBRT_DEBUG_BREAK() std::raise(SIGTRAP)
#else
  #define PBRT_DEBUG_BREAK() std::abort()
#endif

namespace pbrt::detail
{
void assert_handler(std::string_view expression, std::string_view message,
                    std::source_location const &location)
{
#if defined(PBRT_VERBOSE)
  std::cerr << "PBRT Assertion failed!\n"
            << "  Expression: " << expression << "\n"
            << "  File: " << location.file_name() << "\n"
            << "  Line: " << location.line() << "\n"
            << "  Function: " << location.function_name() << "\n";

  if (!message.empty())
  {
    std::cerr << "  Message: " << message << '\n';
  }

  std::cerr.flush();
#else
  std::cerr << "Assertion failed: " << expression;
  if (!message.empty())
  {
    std::cerr << " (" << message << ")";
  }

  std::cerr << " at " << location.file_name() << ":" << location.line() << '\n';
#endif

  PBRT_DEBUG_BREAK();

#if !defined(PBRT_DISABLE_EXCEPTIONS)
  throw std::runtime_error("Assertion failed");
#else
  std::abort();
#endif
}
} // namespace pbrt::detail
