#include <pbrt/assertions.hpp>
#include <pbrt/logging/log_level.hpp>
#include <pbrt/logging/log_record.hpp>
#include <pbrt/logging/logger.hpp>

#include <print>

int main()
{
  pbrt::logging::Logger::set_callback([](const pbrt::logging::LogRecord &record) {
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

    std::println("[{}] {} {}", level, record.message, record.location.file_name());
  });

  PBRT_LOG_TRACE("Trace: {}", 0);
  PBRT_LOG_DEBUG("Debug: {}", 1);
  PBRT_LOG_INFO("Info: {}", 2);
  PBRT_LOG_WARNING("Warning: {}", 3);
  PBRT_LOG_ERROR("Error: {}", 4);
  PBRT_LOG_CRITICAL("Critical: {}", 5);

  return 0;
}
