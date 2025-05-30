#include <pbrt/assertions.hpp>
#include <pbrt/logging/log_level.hpp>
#include <pbrt/logging/log_record.hpp>
#include <pbrt/logging/logger.hpp>
#include <pbrt/profiling/profiler.hpp>

#include <print>
#include <thread>

namespace
{
void test_fn();
}

int main()
{
  pbrt::logging::Logger::get_instance().set_callback([](const pbrt::logging::LogRecord &record) {
    std::string level;
    switch (record.level)
    {
    case pbrt::logging::LogLevel::Trace:
      level = "TRACE";
      break;
    case pbrt::logging::LogLevel::Debug:
      level = "DEBUG";
      break;
    case pbrt::logging::LogLevel::Info:
      level = "INFO";
      break;
    case pbrt::logging::LogLevel::Warning:
      level = "WARNING";
      break;
    case pbrt::logging::LogLevel::Error:
      level = "ERROR";
      break;
    case pbrt::logging::LogLevel::Critical:
      level = "CRITICAL";
      break;
    }

    std::println("[{}] {}", level, record.message);
  });

  auto &profiler{pbrt::profiling::Profiler::get_instance()};

  profiler.begin_frame();

  test_fn();

  profiler.end_frame();

  profiler.log_summary();

  return 0;
}

namespace
{
void test_fn()
{
  PBRT_PROFILE_FUNCTION();

  PBRT_LOG_INFO("This is an info message from test_fn");
  PBRT_LOG_DEBUG("This is a debug message from test_fn");

  {
    PBRT_PROFILE_SCOPE("Test");

    for (int i = 0; i < 10; ++i)
    {
      PBRT_PROFILE_SCOPE("Loop iteration");

      PBRT_LOG_DEBUG("Loop iteration {}", i);

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
}
} // namespace
