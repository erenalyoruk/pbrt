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
PBRT_API class Logger
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
