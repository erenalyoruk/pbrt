#include "pbrt/logging/log_record.hpp"

#include <utility>

namespace pbrt::logging
{
LogRecord::LogRecord(LogLevel level, std::string message, std::source_location location)
    : timestamp{std::chrono::system_clock::now()}, location{location}, level{level},
      message{std::move(message)}
{
}
} // namespace pbrt::logging
