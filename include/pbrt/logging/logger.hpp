#pragma once

#include "pbrt/export.hpp"
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
   * @param format The format string for the message. It must be a valid `std::format`-compatible
   * string.
   * @param args Arguments to be formatted into the message.
   *
   * @note The format string should be compatible with `std::format`.
   */
  template <typename... Args>
  static void log(LogLevel level, std::format_string<Args...> format, Args &&...args);

private:
  inline static Callback logCallback{nullptr};

  /**
   * @internal
   * Helper implementation that logs a message with source location.
   */
  template <typename... Args>
  static void log_impl(LogLevel level, std::source_location location,
                       std::format_string<Args...> format, Args &&...args);
};

template <typename... Args>
void Logger::log(LogLevel level, std::format_string<Args...> format, Args &&...args)
{
  log_impl(level, std::source_location::current(), format, (std::forward<Args>(args))...);
}

template <typename... Args>
void Logger::log_impl(LogLevel level, std::source_location location,
                      std::format_string<Args...> format, Args &&...args)
{
  auto message{std::format(format, (std::forward<Args>(args))...)};
  logCallback({level, std::move(message), location});
}
} // namespace pbrt::logging

constexpr pbrt::logging::LogLevel PBRT_LOG_LEVEL{
#if defined(PBRT_BUILD_DEBUG)
  #if defined(PBRT_VERBOSE)
    pbrt::logging::LogLevel::Trace
  #else
    pbrt::logging::LogLevel::Debug
  #endif
#else
    pbrt::logging::LogLevel::Info
#endif
};

template <typename... Args>
constexpr void PBRT_LOG(pbrt::logging::LogLevel level, std::format_string<Args...> format,
                        Args &&...args)
{
#if PBRT_DISABLE_LOGGING
  (void)level;
  (void)format;
  ((void)std::forward<Args>(args), ...);
#else
  pbrt::logging::Logger::log(level, format, (std::forward<Args>(args))...);
#endif
}

template <typename... Args>
constexpr void PBRT_LOG_TRACE(std::format_string<Args...> format, Args &&...args)
{
  if constexpr (std::to_underlying(PBRT_LOG_LEVEL) <=
                std::to_underlying(pbrt::logging::LogLevel::Trace))
  {
    PBRT_LOG(pbrt::logging::LogLevel::Trace, format, (std::forward<Args>(args))...);
  }
}

template <typename... Args>
constexpr void PBRT_LOG_DEBUG(std::format_string<Args...> format, Args &&...args)
{
  if constexpr (std::to_underlying(PBRT_LOG_LEVEL) <=
                std::to_underlying(pbrt::logging::LogLevel::Debug))
  {
    PBRT_LOG(pbrt::logging::LogLevel::Debug, format, (std::forward<Args>(args))...);
  }
}

template <typename... Args>
constexpr void PBRT_LOG_INFO(std::format_string<Args...> format, Args &&...args)
{
  if constexpr (std::to_underlying(PBRT_LOG_LEVEL) <=
                std::to_underlying(pbrt::logging::LogLevel::Info))
  {
    PBRT_LOG(pbrt::logging::LogLevel::Info, format, (std::forward<Args>(args))...);
  }
}

template <typename... Args>
constexpr void PBRT_LOG_WARNING(std::format_string<Args...> format, Args &&...args)
{
  if constexpr (std::to_underlying(PBRT_LOG_LEVEL) <=
                std::to_underlying(pbrt::logging::LogLevel::Warning))
  {
    PBRT_LOG(pbrt::logging::LogLevel::Warning, format, (std::forward<Args>(args))...);
  }
}

template <typename... Args>
constexpr void PBRT_LOG_ERROR(std::format_string<Args...> format, Args &&...args)
{
  if constexpr (std::to_underlying(PBRT_LOG_LEVEL) <=
                std::to_underlying(pbrt::logging::LogLevel::Error))
  {
    PBRT_LOG(pbrt::logging::LogLevel::Error, format, (std::forward<Args>(args))...);
  }
}

template <typename... Args>
constexpr void PBRT_LOG_CRITICAL(std::format_string<Args...> format, Args &&...args)
{
  if constexpr (std::to_underlying(PBRT_LOG_LEVEL) <=
                std::to_underlying(pbrt::logging::LogLevel::Critical))
  {
    PBRT_LOG(pbrt::logging::LogLevel::Critical, format, (std::forward<Args>(args))...);
  }
}
