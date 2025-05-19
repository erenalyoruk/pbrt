#pragma once

#include "pbrt/types.hpp"

namespace pbrt::logging
{
/**
 * @brief Enum class representing different log levels.
 *
 * The log levels are ordered from the most verbose (Trace) to the least verbose (Critical),
 * allowing filtering based on severity.
 */
enum class LogLevel : u8
{
  /**
   * @brief Detailed information, typically of interest only when diagnosing problems.
   */
  Trace = 0,

  /**
   * @brief Fine-grained informational events that are most useful to debug an application.
   */
  Debug,

  /**
   * @brief Informational messages that highlight the progress of the application at a
   * coarse-grained level.
   */
  Info,

  /**
   * @brief Potentially harmful situations that still allow the application to continue running.
   */
  Warning,

  /**
   * @brief Error events that might still allow the application to continue running.
   */
  Error,

  /**
   * @brief Very severe error events that will presumably lead the application to abort.
   */
  Critical
};
} // namespace pbrt::logging
