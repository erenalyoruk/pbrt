#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <pbrt/math/vector.hpp>
#include <pbrt/simd.hpp>

#include <algorithm>
#include <type_traits>

namespace pbrt::math::test
{
// Test type definitions
using scalar_t = ::testing::Types<i8, i16, i32, i64, u8, u16, u32, u64, f32, f64>;
using floating_t = ::testing::Types<f32, f64>;
using integral_t = ::testing::Types<i8, i16, i32, i64, u8, u16, u32, u64>;
using simd_t = ::testing::Types<f32, f64>;

// Test fixtures with better organization
template <typename T>
class vector_constructor_test : public ::testing::Test
{
public:
  using value_type = T;
  static constexpr T kTestValue{42};
  static constexpr T kZero{0};
};

template <typename T>
class vector_arithmetic_test : public ::testing::Test
{
public:
  using value_type = T;
  static constexpr T kTolerance = std::is_same_v<T, f32> ? T{1e-6F} : T{1e-15};
};

template <typename T>
class vector_simd_test : public ::testing::Test
{
public:
  using value_type = T;
  static constexpr T kTolerance = std::is_same_v<T, f32> ? T{1e-6F} : T{1e-15};

  static constexpr auto simd_size() noexcept -> usize
  {
    if constexpr (std::is_same_v<T, f32>)
    {
      return 4;
    }
    else if constexpr (std::is_same_v<T, f64>)
    {
      return 2;
    }
    return 0;
  }

  static constexpr auto avx_size() noexcept -> usize
  {
    if constexpr (std::is_same_v<T, f32>)
    {
      return 8;
    }
    else if constexpr (std::is_same_v<T, f64>)
    {
      return 4;
    }
    return 0;
  }
};

TYPED_TEST_SUITE(vector_constructor_test, scalar_t);
TYPED_TEST_SUITE(Vector_arithmetic_test, floating_t);
TYPED_TEST_SUITE(vector_simd_test, simd_t);

// ============================================================================
// Constructor Tests
// ============================================================================

TYPED_TEST(vector_constructor_test, DefaultConstruction)
{
  using T = typename TestFixture::value_type;
  using vec2 = vector<T, 2>;
  using vec3 = vector<T, 3>;
  using vec4 = vector<T, 4>;

  vec2 v2{};
  constexpr vec3 v3{};
  vec4 v4{};

  EXPECT_EQ(v2.x(), T{0});
  EXPECT_EQ(v2.y(), T{0});

  EXPECT_EQ(v3.x(), T{0});
  EXPECT_EQ(v3.y(), T{0});
  EXPECT_EQ(v3.z(), T{0});

  EXPECT_EQ(v4.x(), T{0});
  EXPECT_EQ(v4.y(), T{0});
  EXPECT_EQ(v4.z(), T{0});
  EXPECT_EQ(v4.w(), T{0});
}

TYPED_TEST(vector_constructor_test, ScalarConstruction)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  constexpr T value{TestFixture::kTestValue};
  constexpr vec3 v{value};

  EXPECT_EQ(v.x(), value);
  EXPECT_EQ(v.y(), value);
  EXPECT_EQ(v.z(), value);
}

TYPED_TEST(vector_constructor_test, ComponentConstruction)
{
  using T = typename TestFixture::value_type;
  using vec2 = vector<T, 2>;
  using vec3 = vector<T, 3>;
  using vec4 = vector<T, 4>;

  vec2 v2{T{1}, T{2}};
  constexpr vec3 v3{T{1}, T{2}, T{3}};
  vec4 v4{T{1}, T{2}, T{3}, T{4}};

  EXPECT_EQ(v2.x(), T{1});
  EXPECT_EQ(v2.y(), T{2});

  EXPECT_EQ(v3.x(), T{1});
  EXPECT_EQ(v3.y(), T{2});
  EXPECT_EQ(v3.z(), T{3});

  EXPECT_EQ(v4.x(), T{1});
  EXPECT_EQ(v4.y(), T{2});
  EXPECT_EQ(v4.z(), T{3});
  EXPECT_EQ(v4.w(), T{4});
}

TYPED_TEST(vector_constructor_test, CopyConstruction)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  constexpr vec3 original{T{1}, T{2}, T{3}};
  constexpr vec3 copy{original};

  EXPECT_EQ(copy.x(), original.x());
  EXPECT_EQ(copy.y(), original.y());
  EXPECT_EQ(copy.z(), original.z());
  EXPECT_TRUE(copy == original);
}

TYPED_TEST(vector_constructor_test, MoveConstruction)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 original{T{1}, T{2}, T{3}};
  vec3 expected{original};
  vec3 moved{std::move(original)};

  EXPECT_EQ(moved.x(), expected.x());
  EXPECT_EQ(moved.y(), expected.y());
  EXPECT_EQ(moved.z(), expected.z());
}

TYPED_TEST(vector_constructor_test, InitializerListConstruction)
{
  using T = typename TestFixture::value_type;
  using vec4 = vector<T, 4>;

  std::array<T, 4> arr{T{1}, T{2}, T{3}, T{4}};
}

// ============================================================================
// Assignment Tests
// ============================================================================

TYPED_TEST(vector_constructor_test, CopyAssignment)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 original{T{1}, T{2}, T{3}};
  vec3 assigned{};

  assigned = original;

  EXPECT_EQ(assigned.x(), original.x());
  EXPECT_EQ(assigned.y(), original.y());
  EXPECT_EQ(assigned.z(), original.z());
  EXPECT_TRUE(assigned == original);
}

TYPED_TEST(vector_constructor_test, MoveAssignment)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 original{T{1}, T{2}, T{3}};
  vec3 expected{original};
  vec3 assigned{};

  assigned = std::move(original);

  EXPECT_EQ(assigned.x(), expected.x());
  EXPECT_EQ(assigned.y(), expected.y());
  EXPECT_EQ(assigned.z(), expected.z());
}

TYPED_TEST(vector_constructor_test, SelfAssignment)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{1}, T{2}, T{3}};
  vec3 original{v};

  v = v; // Self-assignment // NOLINT

  EXPECT_EQ(v.x(), original.x());
  EXPECT_EQ(v.y(), original.y());
  EXPECT_EQ(v.z(), original.z());
}
} // namespace pbrt::math::test
