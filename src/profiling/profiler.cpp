#include "pbrt/profiling/profiler.hpp"

#include "pbrt/logging/logger.hpp"

#include <algorithm>
#include <format>

namespace pbrt::profiling
{
Profiler &Profiler::get_instance()
{
  static Profiler instance;
  return instance;
}

void Profiler::begin_profile(std::string const &name, std::source_location location)
{
  if (!m_enabled)
  {
    return;
  }

  std::lock_guard lock{m_mutex};

  m_activeTimers[name] = Clock::now();

  auto &profile{m_profiles[name]};
  if (profile.name.empty())
  {
    profile.name = name;
  }

  profile.depth = m_currentDepth;
  profile.location = location;
  ++m_currentDepth;
}

void Profiler::end_profile(std::string const &name)
{
  if (!m_enabled)
  {
    return;
  }

  auto const endTime{Clock::now()};

  std::lock_guard lock{m_mutex};
  --m_currentDepth;

  auto timerIt{m_activeTimers.find(name)};
  if (timerIt == m_activeTimers.end())
  {
    return; // no matching begin_profile call
  }

  auto const duration{std::chrono::duration_cast<Duration>(endTime - timerIt->second)};
  m_activeTimers.erase(timerIt);

  auto &profile{m_profiles[name]};
  if (profile.name.empty())
  {
    profile.name = name;
    profile.depth = m_currentDepth;
  }

  profile.add_sample(duration);
}

void Profiler::reset()
{
  std::lock_guard lock{m_mutex};
  m_profiles.clear();
  m_activeTimers.clear();
  m_frameCount = 0;
  m_lastFrameTime = Duration::zero();
  m_currentDepth = 0;
}

void Profiler::enable(bool enabled) noexcept
{
  std::lock_guard lock{m_mutex};
  m_enabled = enabled;
}

bool Profiler::is_enabled() const noexcept
{
  return m_enabled;
}

void Profiler::log_summary() const
{
  std::lock_guard lock{m_mutex};

  auto sortedProfiles{get_sorted_profiles()};

  constexpr auto totalWidth{92};
  std::string title{std::format(" PROFILER SUMMARY ({} profiles) ", sortedProfiles.size())};
  auto titleLength{title.length()};
  auto paddingTotal{totalWidth - titleLength};
  auto leftPadding{paddingTotal / 2};
  auto rightPadding{paddingTotal - leftPadding};

  std::string centeredTitle{std::string(leftPadding, '=') + title + std::string(rightPadding, '=')};

  PBRT_LOG_DEBUG("{}", centeredTitle);
  PBRT_LOG_DEBUG("{:<30} | {:>10} {:>14} {:>14} {:>14}", "Name", "Calls", "Total (ms)", "Min (ms)",
                 "Max (ms)");
  PBRT_LOG_DEBUG("{:-<92}", "");

  for (auto const &profile : sortedProfiles)
  {
    PBRT_LOG_DEBUG("{:<30} | {:>10} {:>14.3f} {:>14.3f} {:>14.3f}", profile.name, profile.callCount,
                   profile.total_time_ms(), profile.average_time_ms(),
                   std::chrono::duration<f64, std::milli>(profile.maxTime).count());
  }

  if (m_frameCount > 0)
  {
    PBRT_LOG_DEBUG("{:-<92}", "");
    PBRT_LOG_DEBUG("Frame Stats:");
    PBRT_LOG_DEBUG("  Frame Count: {}", m_frameCount);
    PBRT_LOG_DEBUG("  Last Frame Time: {:.3f}ms",
                   std::chrono::duration<f64, std::milli>(m_lastFrameTime).count());
    PBRT_LOG_DEBUG("{:=<92}", "");
  }
}

void Profiler::log_detailed_report() const
{
  std::lock_guard lock{m_mutex};

  auto sortedProfiles{get_sorted_profiles()};

  constexpr auto detailWidth{92};
  std::string detailTitle{" Detailed Profiling Report "};
  auto detailTitleLength{detailTitle.length()};
  auto detailPaddingTotal{detailWidth - detailTitleLength};
  auto detailPaddingLeft{detailPaddingTotal / 2};
  auto detailPaddingRight{detailPaddingTotal - detailPaddingLeft};

  std::string centeredDetailTitle{std::string(detailPaddingLeft, '=') + detailTitle +
                                  std::string(detailPaddingRight, '=')};

  PBRT_LOG_TRACE("{}", centeredDetailTitle);
  for (auto const &profile : sortedProfiles)
  {
    PBRT_LOG_TRACE("Profile: {}", profile.name);
    PBRT_LOG_TRACE("  Depth: {}", profile.depth);
    PBRT_LOG_TRACE("  Call Count: {}", profile.callCount);
    PBRT_LOG_TRACE("  Total Time: {:.6f}ms", profile.total_time_ms());
    PBRT_LOG_TRACE("  Average Time: {:.6f}ms", profile.average_time_ms());
    PBRT_LOG_TRACE("  Min Time: {:.6f}ms",
                   std::chrono::duration<f64, std::milli>(profile.minTime).count());
    PBRT_LOG_TRACE("  Max Time: {:.6f}ms",
                   std::chrono::duration<f64, std::milli>(profile.maxTime).count());
    PBRT_LOG_TRACE("");
  }
}

std::vector<ProfileData> Profiler::get_sorted_profiles() const
{
  std::vector<ProfileData> result;
  result.reserve(m_profiles.size());

  for (auto const &[name, profile] : m_profiles)
  {
    result.push_back(profile);
  }

  std::ranges::sort(
      result, [](ProfileData const &a, ProfileData const &b) { return a.totalTime > b.totalTime; });

  return result;
}

void Profiler::begin_frame()
{
  if (!m_enabled)
  {
    return;
  }

  std::lock_guard lock{m_mutex};
  m_frameStartTime = Clock::now();
}

void Profiler::end_frame()
{
  if (!m_enabled)
  {
    return;
  }

  auto const frameEnd = Clock::now();

  std::lock_guard lock{m_mutex};
  m_lastFrameTime = std::chrono::duration_cast<Duration>(frameEnd - m_frameStartTime);
  ++m_frameCount;
}

u64 Profiler::get_frame_count() const noexcept
{
  return m_frameCount;
}

Duration Profiler::get_last_frame_time() const noexcept
{
  return m_lastFrameTime;
}

ScopedTimer::ScopedTimer(std::string name, std::source_location location) noexcept
    : m_name{std::move(name)}, m_active{false}
{
  if (Profiler::get_instance().is_enabled())
  {
    Profiler::get_instance().begin_profile(m_name, location);
    m_active = true;
  }
}

ScopedTimer::~ScopedTimer() noexcept
{
  if (m_active)
  {
    Profiler::get_instance().end_profile(m_name);
  }
}
} // namespace pbrt::profiling
