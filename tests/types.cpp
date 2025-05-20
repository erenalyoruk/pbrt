#include <catch2/catch_test_macros.hpp>
#include <pbrt/types.hpp>

#include <type_traits>

TEST_CASE("Primitive types are correctly aliased", "[types]")
{
  REQUIRE(std::is_same_v<pbrt::u8, std::uint8_t>);
  REQUIRE(std::is_same_v<pbrt::u16, std::uint16_t>);
  REQUIRE(std::is_same_v<pbrt::u32, std::uint32_t>);
  REQUIRE(std::is_same_v<pbrt::u64, std::uint64_t>);

  REQUIRE(std::is_same_v<pbrt::i8, std::int8_t>);
  REQUIRE(std::is_same_v<pbrt::i16, std::int16_t>);
  REQUIRE(std::is_same_v<pbrt::i32, std::int32_t>);
  REQUIRE(std::is_same_v<pbrt::i64, std::int64_t>);

  REQUIRE(std::is_same_v<pbrt::f32, float>);
  REQUIRE(std::is_same_v<pbrt::f64, double>);

  REQUIRE(std::is_same_v<pbrt::byte, pbrt::u8>);
  REQUIRE(std::is_same_v<pbrt::usize, std::size_t>);
  REQUIRE(std::is_same_v<pbrt::isize, std::ptrdiff_t>);
}
