#include "pbrt/assertions.hpp"

#include <iostream>
#include <print>

namespace pbrt
{
void PBRT_API assertion_failed(std::string_view expr, std::source_location location)
{
  std::println(std::cerr, "Assertion failed: {}\nFunction: {}\nFile: {}\nLine: {}:{}", expr,
               location.function_name(), location.file_name(), location.line(), location.column());
}
} // namespace pbrt
