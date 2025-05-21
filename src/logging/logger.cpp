#include "pbrt/logging/logger.hpp"

#include <utility>

namespace pbrt::logging
{
void Logger::set_callback(Callback callback)
{
  m_logCallback = std::move(callback);
}
} // namespace pbrt::logging
