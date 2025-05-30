#pragma once

#include "pbrt/export.hpp"
#include "pbrt/profiling/profile_data.hpp"
#include "pbrt/profiling/time.hpp"

#include <mutex>
#include <source_location>
#include <string>
#include <unordered_map>
#include <vector>

// TODO: make threaded profiling work correctly

namespace pbrt::profiling
{
/**
 * @brief A singleton profiler class for measuring and logging performance metrics.
 */
class PBRT_API Profiler
{
public:
  /**
   * @brief Get the singleton instance of the Profiler.
   *
   * @return Profiler& Reference to the singleton instance.
   */
  static Profiler &get_instance();

  /**
   * @brief Begin profiling a named section of code.
   *
   * @param name Name of the profile section.
   * @param location Source location information for debugging.
   */
  void begin_profile(std::string const &name,
                     std::source_location location = std::source_location::current());

  /**
   * @brief End profiling a named section of code.
   *
   * @param name Name of the profile section.
   */
  void end_profile(std::string const &name);

  /**
   * @brief Reset the profiler state, clearing all profiles and timers.
   */
  void reset();

  /**
   * @brief Enable or disable profiling.
   *
   * @param enabled Whether to enable or disable profiling. Defaults to true.
   */
  void enable(bool enabled = true) noexcept;

  /**
   * @brief Check if profiling is enabled.
   *
   * @return true if profiling is enabled, false otherwise.
   * @return false if profiling is disabled.
   */
  [[nodiscard]] bool is_enabled() const noexcept;

  /**
   * @brief Log a summary of the profiling data. Logging is done at the debug level.
   */
  void log_summary() const;

  /**
   * @brief Log a detailed report of the profiling data. Logging is done at the trace level.
   */
  void log_detailed_report() const;

  /**
   * @brief Get a sorted list of profile data.
   *
   * @return std::vector<ProfileData> A vector of ProfileData sorted by total time.
   */
  [[nodiscard]] std::vector<ProfileData> get_sorted_profiles() const;

  /**
   * @brief Begin a new frame for profiling.
   */
  void begin_frame();

  /**
   * @brief End the current frame for profiling.
   */
  void end_frame();

  /**
   * @brief Get the frame count.
   *
   * @return u64 The number of frames that have been profiled.
   */
  [[nodiscard]] u64 get_frame_count() const noexcept;

  /**
   * @brief Get the time taken for the last frame.
   *
   * @return Duration The duration of the last frame in milliseconds.
   */
  [[nodiscard]] Duration get_last_frame_time() const noexcept;

private:
  Profiler() = default;

  mutable std::mutex m_mutex;

  std::unordered_map<std::string, ProfileData> m_profiles;
  std::unordered_map<std::string, TimePoint> m_activeTimers;

  bool m_enabled{true};

  TimePoint m_frameStartTime;
  Duration m_lastFrameTime{0};
  u64 m_frameCount{0};
  u64 m_currentDepth{0};
};

/**
 * @brief Scoped timer class for profiling a specific section of code.
 */
class PBRT_API ScopedTimer
{
public:
  /**
   * @brief Construct a ScopedTimer with a name and optional source location.
   *
   * @param name Name of the profile section.
   * @param location Source location information for debugging.
   */
  explicit ScopedTimer(std::string name,
                       std::source_location location = std::source_location::current()) noexcept;

  /**
   * @brief Destructor for ScopedTimer.
   *
   */
  ~ScopedTimer() noexcept;

  // prevent copying and moving
  ScopedTimer(ScopedTimer const &) = delete;
  ScopedTimer &operator=(ScopedTimer const &) = delete;
  ScopedTimer(ScopedTimer &&) = delete;
  ScopedTimer &operator=(ScopedTimer &&) = delete;

private:
  std::string m_name;
  bool m_active;
};
}; // namespace pbrt::profiling

#define PBRT_PROFILE_SCOPE(name) pbrt::profiling::ScopedTimer _pbrt_timer(name)
#define PBRT_PROFILE_FUNCTION() PBRT_PROFILE_SCOPE(__FUNCTION__)
#define PBRT_PROFILE_BLOCK(name) pbrt::profiling::ScopedTimer _pbrt_timer_##__LINE__(name)

#if !defined(PBRT_DISABLE_PROFILING)
  #define PBRT_PROFILE_SCOPE_DEBUG(name) PBRT_PROFILE_SCOPE(name)
  #define PBRT_PROFILE_FUNCTION_DEBUG() PBRT_PROFILE_FUNCTION()
#else
  #define PBRT_PROFILE_SCOPE_DEBUG(name) ((void)0)
  #define PBRT_PROFILE_FUNCTION_DEBUG() ((void)0)
#endif
