#pragma once

#include "pbrt/logging/log_level.hpp"

#include <chrono>
#include <source_location>
#include <string>

namespace pbrt::logging
{
struct LogRecord
{
  /**
   * @brief Timestamp of the log record.
   */
  std::chrono::system_clock::time_point timestamp;

  /**
   * @brief Source location of the log record.
   */
  std::source_location location;

  /**
   * @brief Log level of the log record.
   */
  LogLevel level;

  /**
   * @brief Message associated with the log record.
   */
  std::string message;

  /**
   * @brief Constructs a LogRecord with the current timestamp and source location.
   *
   * @param level Log level of the log record.
   * @param message Message associated with the log record.
   * @param location Source location of the log record (default is current location).
   */
  LogRecord(LogLevel level, std::string message,
            std::source_location location = std::source_location::current());
};
} // namespace pbrt::logging
