#include <pbrt/logging/log_level.hpp>
#include <pbrt/logging/log_record.hpp>
#include <pbrt/logging/logger.hpp>

#include <iostream>

int main()
{
  pbrt::logging::Logger::set_callback([](const pbrt::logging::LogRecord &record) {
    std::cout << "[" << static_cast<int>(record.level) << "] " << record.message << "\n";
  });

  pbrt::logging::Logger::log(pbrt::logging::LogLevel::Trace, "This is a debug message\n{}", 52);

  return 0;
}
