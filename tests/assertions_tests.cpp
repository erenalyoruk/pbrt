#include <catch2/catch_test_macros.hpp>
#include <pbrt/assertions.hpp>

#include <iostream>
#include <source_location>
#include <sstream>

TEST_CASE("Assertion failed prints expected output", "[assertions]")
{
  std::stringstream buf;
  auto *old = std::cerr.rdbuf(buf.rdbuf());

  auto sl = std::source_location::current();
  pbrt::assertion_failed("x > 0", sl);

  std::cerr.rdbuf(old);

  std::string out = buf.str();
  REQUIRE(out.find("Assertion failed: x > 0") != std::string::npos);
  REQUIRE(out.find("Function:") != std::string::npos);
  REQUIRE(out.find("File:") != std::string::npos);
  REQUIRE(out.find("Line:") != std::string::npos);
}
