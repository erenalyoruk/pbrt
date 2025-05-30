#pragma once

#include <chrono>

namespace pbrt::profiling
{
using Clock = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using Duration = std::chrono::nanoseconds;
} // namespace pbrt::profiling
