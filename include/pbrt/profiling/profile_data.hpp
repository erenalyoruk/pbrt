#pragma once

#include "pbrt/export.hpp"
#include "pbrt/profiling/time.hpp"
#include "pbrt/types.hpp"

#include <source_location>
#include <string>

namespace pbrt::profiling
{
/**
 * @brief Data structure to hold profiling information for a specific section of code.
 */
struct PBRT_API ProfileData
{
  std::string name;                  // Name of the profile section
  Duration totalTime{0};             // Total time spent in this profile section
  Duration minTime{Duration::max()}; // Minimum time spent in this profile section
  Duration maxTime{Duration::min()}; // Maximum time spent in this profile section
  u64 callCount{0};                  // Number of times this profile section was called
  u64 depth{0};                      // Depth of the profile section in the call stack
  std::source_location location{std::source_location::current()}; // Source location for debugging

  /**
   * @brief Default constructor for ProfileData.
   */
  ProfileData() = default;

  /**
   * @brief Constructor for ProfileData with name and optional source location and depth.
   *
   * @param name Name of the profile section.
   * @param location Source location information for debugging.
   * @param depth Depth of the profile section in the call stack.
   */
  ProfileData(std::string name, std::source_location location = std::source_location::current(),
              u64 depth = 0);

  /**
   * @brief Add a sample duration to the profile data.
   *
   * @param sample Duration of the sample to add.
   */
  void add_sample(Duration sample);

  /**
   * @brief Get the average time spent in this profile section.
   *
   * @return f64 Average time in milliseconds.
   */
  [[nodiscard]] f64 average_time_ms() const noexcept;

  /**
   * @brief Get the total time spent in this profile section.
   *
   * @return f64 Total time in milliseconds.
   */
  [[nodiscard]] f64 total_time_ms() const noexcept;
};
} // namespace pbrt::profiling
