#pragma once

#include "pbrt/debug/debug_break.hpp"
#include "pbrt/export.hpp"
#include "pbrt/inline.hpp"

#include <source_location>
#include <string_view>

namespace pbrt
{
/**
 * @brief Debug break handler.
 */
PBRT_INLINE void assert_handler()
{
  debug::debug_break();
}

/**
 * @brief Handler for assertion failures.
 *
 * @param expression The expression that failed.
 * @param location The source location of the failure.
 */
void PBRT_API assertion_failed(std::string_view expression, std::source_location location);
} // namespace pbrt

#define PBRT_STATIC_ASSERT(expr, ...) static_assert(expr, __VA_ARGS__)

#define PBRT_VERIFY(expr)                                                                          \
  ((expr) ? (void)0                                                                                \
          : (pbrt::assertion_failed(#expr, std::source_location::current()),                       \
             pbrt::assert_handler(), (void)0))

#if defined(PBRT_BUILD_DEBUG)
  #define PBRT_ASSERT(expr, ...)                                                                   \
    ((expr) ? (void)0                                                                              \
            : (pbrt::assertion_failed(#expr, std::source_location::current()),                     \
               pbrt::assert_handler(), (void)0))

  #define PBRT_ASSERT_MSG(expr, msg)                                                               \
    ((expr) ? (void)0                                                                              \
            : (pbrt::assertion_failed(#expr ": " msg, std::source_location::current()),            \
               pbrt::assert_handler()))
#else
  #define PBRT_ASSERT(expr, ...) ((void)0)
  #define PBRT_ASSERT_MSG(expr, msg) ((void)0)
#endif
