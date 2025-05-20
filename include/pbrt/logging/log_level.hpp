#pragma once

#include "pbrt/types.hpp"

#define PBRT_LOG_LEVEL_TRACE 0
#define PBRT_LOG_LEVEL_DEBUG 1
#define PBRT_LOG_LEVEL_INFO 2
#define PBRT_LOG_LEVEL_WARNING 3
#define PBRT_LOG_LEVEL_ERROR 4
#define PBRT_LOG_LEVEL_CRITICAL 5

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
  Trace = PBRT_LOG_LEVEL_TRACE,

  /**
   * @brief Fine-grained informational events that are most useful to debug an application.
   */
  Debug = PBRT_LOG_LEVEL_DEBUG,

  /**
   * @brief Informational messages that highlight the progress of the application at a
   * coarse-grained level.
   */
  Info = PBRT_LOG_LEVEL_INFO,

  /**
   * @brief Potentially harmful situations that still allow the application to continue running.
   */
  Warning = PBRT_LOG_LEVEL_WARNING,

  /**
   * @brief Error events that might still allow the application to continue running.
   */
  Error = PBRT_LOG_LEVEL_ERROR,

  /**
   * @brief Very severe error events that will presumably lead the application to abort.
   */
  Critical = PBRT_LOG_LEVEL_CRITICAL
};
} // namespace pbrt::logging
