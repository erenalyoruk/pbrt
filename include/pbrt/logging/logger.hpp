#pragma once

#include "pbrt/export.hpp"
#include "pbrt/inline.hpp"
#include "pbrt/logging/log_level.hpp"
#include "pbrt/logging/log_record.hpp"

#include <format>
#include <functional>

namespace pbrt::logging
{
/**
 * @brief Logger class for logging messages with different log levels.
 *
 * The Logger class provides a static interface for logging messages with various log levels.
 * It allows setting a callback function that is called whenever a log message is generated.
 * The callback accepts a @ref LogRecord "LogRecord" object containing the log level,
 * formatted message, and source location.
 */
class PBRT_API Logger
{
public:
  using Callback = std::function<void(const LogRecord &)>;

  /**
   * @brief Set the callback function for logging messages.
   *
   * The callback accepts a @ref LogRecord "LogRecord" object containing log level,
   * formatted message, and source location.
   *
   * @param callback The callback function to invoke for log messages.
   */
  static void set_callback(Callback callback);

  /**
   * @brief Log a message with the specified log level.
   *
   * Formats the message using the provided format string and arguments,
   * then calls the callback with a @ref LogRecord "LogRecord" object.
   *
   * @tparam Args Types of the arguments to be formatted.
   * @param level The log level for the message.
   * @param location The source location of the log message.
   * @param format The format string for the message. It must be a valid `std::format`-compatible
   * string.
   * @param args Arguments to be formatted into the message.
   *
   * @note The format string should be compatible with `std::format`.
   */
  template <typename... Args>
  PBRT_INLINE static void log(LogLevel level, std::source_location location,
                              std::format_string<Args...> format, Args &&...args);

private:
  inline static Callback logCallback{nullptr};
};

template <typename... Args>
PBRT_INLINE void Logger::log(LogLevel level, std::source_location location,
                             std::format_string<Args...> format, Args &&...args)
{
  logCallback({level, std::format(format, (std::forward<Args>(args))...), location});
}
} // namespace pbrt::logging

constexpr pbrt::u8 PBRT_LOG_LEVEL{
#if defined(PBRT_BUILD_DEBUG)
  #if defined(PBRT_VERBOSE)
    PBRT_LOG_LEVEL_TRACE
  #else
    PBRT_LOG_LEVEL_DEBUG
  #endif
#else
    PBRT_LOG_LEVEL_INFO
#endif
};

#if defined(PBRT_DISABLE_LOGGING)
  #define PBRT_LOG_TRACE(...) (void)0
#else
  #define PBRT_LOG(level, format, ...)                                                             \
    pbrt::logging::Logger::log(level, std::source_location::current(), format, __VA_ARGS__)
#endif

#if PBRT_LOG_LEVEL <= PBRT_LOG_LEVEL_TRACE
  #define PBRT_LOG_TRACE(format, ...) PBRT_LOG(pbrt::logging::LogLevel::Trace, format, __VA_ARGS__)
#else
  #define PBRT_LOG_DEBUG(...) (void)0
#endif

#if PBRT_LOG_LEVEL <= PBRT_LOG_LEVEL_DEBUG
  #define PBRT_LOG_DEBUG(format, ...) PBRT_LOG(pbrt::logging::LogLevel::Debug, format, __VA_ARGS__)
#else
  #define PBRT_LOG_DEBUG(...) (void)0
#endif

#if PBRT_LOG_LEVEL <= PBRT_LOG_LEVEL_INFO
  #define PBRT_LOG_INFO(format, ...) PBRT_LOG(pbrt::logging::LogLevel::Info, format, __VA_ARGS__)
#else
  #define PBRT_LOG_INFO(...) (void)0
#endif

#if PBRT_LOG_LEVEL <= PBRT_LOG_LEVEL_WARNING
  #define PBRT_LOG_WARNING(format, ...)                                                            \
    PBRT_LOG(pbrt::logging::LogLevel::Warning, format, __VA_ARGS__)
#else
  #define PBRT_LOG_WARNING(...) (void)0

#endif
#if PBRT_LOG_LEVEL <= PBRT_LOG_LEVEL_ERROR
  #define PBRT_LOG_ERROR(format, ...) PBRT_LOG(pbrt::logging::LogLevel::Error, format, __VA_ARGS__)
#else
  #define PBRT_LOG_ERROR(...) (void)0
#endif

#if PBRT_LOG_LEVEL <= PBRT_LOG_LEVEL_CRITICAL
  #define PBRT_LOG_CRITICAL(format, ...)                                                           \
    PBRT_LOG(pbrt::logging::LogLevel::Critical, format, __VA_ARGS__)
#else
  #define PBRT_LOG_CRITICAL(...) (void)0
#endif
