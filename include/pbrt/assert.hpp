#pragma once

#include "pbrt/api.hpp"

#include <source_location>
#include <string_view>

namespace pbrt::detail
{
PBRT_API void assert_handler(
    std::string_view expression, std::string_view message,
    std::source_location const &location = std::source_location::current());

[[noreturn]] PBRT_API void abort_handler(
    std::string_view message,
    std::source_location const &location = std::source_location::current());
} // namespace pbrt::detail

#define PBRT_STATIC_ASSERT(expr) static_assert(expr, #expr)
#define PBRT_STATIC_ASSERT_MSG(expr, msg) static_assert(expr, msg)

#if !defined(PBRT_DISABLE_ASSERTIONS)
  #define PBRT_ASSERT(expr)                                                                        \
    if (!(expr))                                                                                   \
    {                                                                                              \
      ::pbrt::detail::assert_handler(#expr, "");                                                   \
    }

  #define PBRT_ASSERT_MSG(expr, msg)                                                               \
    if (!(expr)) [[unlikely]]                                                                      \
    {                                                                                              \
      ::pbrt::detail::assert_handler(#expr, msg);                                                  \
    }

  #define PBRT_ASSERT_EQ(a, b) PBRT_ASSERT((a) == (b))
  #define PBRT_ASSERT_NE(a, b) PBRT_ASSERT((a) != (b))
  #define PBRT_ASSERT_LT(a, b) PBRT_ASSERT((a) < (b))
  #define PBRT_ASSERT_LE(a, b) PBRT_ASSERT((a) <= (b))
  #define PBRT_ASSERT_GT(a, b) PBRT_ASSERT((a) > (b))
  #define PBRT_ASSERT_GE(a, b) PBRT_ASSERT((a) >= (b))

  #define PBRT_UNREACHABLE(msg) ::pbrt::detail::abort_handler("Unreachable code: " msg)
#else
  #define PBRT_ASSERT(expr) ((void)0)
  #define PBRT_ASSERT_MSG(expr, msg) ((void)0)
  #define PBRT_ASSERT_EQ(a, b) ((void)0)
  #define PBRT_ASSERT_NE(a, b) ((void)0)
  #define PBRT_ASSERT_LT(a, b) ((void)0)
  #define PBRT_ASSERT_LE(a, b) ((void)0)
  #define PBRT_ASSERT_GT(a, b) ((void)0)
  #define PBRT_ASSERT_GE(a, b) ((void)0)
  #define PBRT_UNREACHABLE(msg) ((void)0)
#endif
