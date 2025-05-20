#include <catch2/catch_test_macros.hpp>
#include <pbrt/logging/log_level.hpp>
#include <pbrt/logging/log_record.hpp>
#include <pbrt/logging/logger.hpp>

#include <source_location>
#include <string>

TEST_CASE("Logger test", "[logging]")
{
  using namespace pbrt::logging;

  bool called{false};
  LogLevel level{};
  std::string msg;
  std::source_location location{};

  Logger::set_callback([&](const LogRecord &record) {
    called = true;
    level = record.level;
    msg = record.message;
    location = record.location;
  });

  PBRT_LOG_INFO("Test {}", 123);

  REQUIRE(called);
  REQUIRE(level == LogLevel::Info);
  REQUIRE(msg == "Test 123");

  std::string filename{location.file_name()};
  REQUIRE(filename.find("logging.cpp") != std::string::npos);
}
