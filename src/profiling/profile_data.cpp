#include "pbrt/profiling/profile_data.hpp"

#include <utility>

namespace pbrt::profiling
{
ProfileData::ProfileData(std::string name, std::source_location location, u64 depth)
    : name{std::move(name)}, depth{depth}, location{location}
{
}

void ProfileData::add_sample(Duration sample)
{
  totalTime += sample;
  minTime = std::min(minTime, sample);
  maxTime = std::max(maxTime, sample);
  ++callCount;
}

f64 ProfileData::average_time_ms() const noexcept
{
  if (callCount == 0)
  {
    return 0.0;
  }

  return std::chrono::duration<f64, std::milli>(totalTime).count() / static_cast<f64>(callCount);
}

f64 ProfileData::total_time_ms() const noexcept
{
  return std::chrono::duration<f64, std::milli>(totalTime).count();
}
} // namespace pbrt::profiling
