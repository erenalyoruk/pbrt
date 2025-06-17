#include <gtest/gtest.h>
#include <pbrt/math/vector.hpp>

namespace pbrt::math::test
{
using test_t = ::testing::Types<i32, f32, f64>;

template <typename T>
class vector_arithmetic_test : public ::testing::Test
{
public:
  using value_type = T;
  static constexpr T kTolerance = std::is_same_v<T, f32> ? T{1e-6F} : T{1e-15};
};

TYPED_TEST_SUITE(vector_arithmetic_test, test_t);

// ============================================================================
// Arithmetic Assignment Operators
// ============================================================================

TYPED_TEST(vector_arithmetic_test, AdditionAssignment)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{1}, T{2}, T{3}};
  vec3 v2{T{4}, T{5}, T{6}};

  v1 += v2;

  EXPECT_EQ(v1.x(), T{5});
  EXPECT_EQ(v1.y(), T{7});
  EXPECT_EQ(v1.z(), T{9});

  // Chain operations
  v1 += vec3{T{1}, T{1}, T{1}};
  EXPECT_EQ(v1.x(), T{6});
  EXPECT_EQ(v1.y(), T{8});
  EXPECT_EQ(v1.z(), T{10});
}

TYPED_TEST(vector_arithmetic_test, SubtractionAssignment)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{10}, T{8}, T{6}};
  vec3 v2{T{4}, T{3}, T{2}};

  v1 -= v2;

  EXPECT_EQ(v1.x(), T{6});
  EXPECT_EQ(v1.y(), T{5});
  EXPECT_EQ(v1.z(), T{4});
}

TYPED_TEST(vector_arithmetic_test, ScalarMultiplicationAssignment)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{2}, T{4}, T{6}};

  v *= T{3};

  EXPECT_EQ(v.x(), T{6});
  EXPECT_EQ(v.y(), T{12});
  EXPECT_EQ(v.z(), T{18});
}

TYPED_TEST(vector_arithmetic_test, VectorMultiplicationAssignment)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{2}, T{3}, T{4}};
  vec3 v2{T{5}, T{6}, T{7}};

  v1 *= v2;

  EXPECT_EQ(v1.x(), T{10});
  EXPECT_EQ(v1.y(), T{18});
  EXPECT_EQ(v1.z(), T{28});
}

TYPED_TEST(vector_arithmetic_test, ScalarDivisionAssignment)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  if constexpr (std::floating_point<T>)
  {
    vec3 v{T{6}, T{8}, T{12}};

    v /= T{2};

    EXPECT_EQ(v.x(), T{3});
    EXPECT_EQ(v.y(), T{4});
    EXPECT_EQ(v.z(), T{6});
  }
}

TYPED_TEST(vector_arithmetic_test, VectorDivisionAssignment)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  if constexpr (std::floating_point<T>)
  {
    vec3 v1{T{12}, T{15}, T{20}};
    vec3 v2{T{3}, T{5}, T{4}};

    v1 /= v2;

    EXPECT_EQ(v1.x(), T{4});
    EXPECT_EQ(v1.y(), T{3});
    EXPECT_EQ(v1.z(), T{5});
  }
}

// ============================================================================
// Binary Arithmetic Operators
// ============================================================================

TYPED_TEST(vector_arithmetic_test, VectorAddition)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{1}, T{2}, T{3}};
  vec3 v2{T{4}, T{5}, T{6}};

  auto result{v1 + v2};

  EXPECT_EQ(result.x(), T{5});
  EXPECT_EQ(result.y(), T{7});
  EXPECT_EQ(result.z(), T{9});

  // Original vectors unchanged
  EXPECT_EQ(v1.x(), T{1});
  EXPECT_EQ(v2.x(), T{4});
}

TYPED_TEST(vector_arithmetic_test, VectorSubtraction)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{10}, T{8}, T{6}};
  vec3 v2{T{4}, T{3}, T{2}};

  auto result{v1 - v2};

  EXPECT_EQ(result.x(), T{6});
  EXPECT_EQ(result.y(), T{5});
  EXPECT_EQ(result.z(), T{4});
}

TYPED_TEST(vector_arithmetic_test, UnaryNegation)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  if constexpr (std::is_signed_v<T>)
  {
    vec3 v{T{1}, T{-2}, T{3}};
    auto negated{-v};

    EXPECT_EQ(negated.x(), T{-1});
    EXPECT_EQ(negated.y(), T{2});
    EXPECT_EQ(negated.z(), T{-3});

    // Original unchanged
    EXPECT_EQ(v.x(), T{1});
    EXPECT_EQ(v.y(), T{-2});
    EXPECT_EQ(v.z(), T{3});
  }
}

TYPED_TEST(vector_arithmetic_test, ScalarMultiplication)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{2}, T{3}, T{4}};

  auto result1{v * T{3}};
  auto result2{T{3} * v};

  EXPECT_EQ(result1.x(), T{6});
  EXPECT_EQ(result1.y(), T{9});
  EXPECT_EQ(result1.z(), T{12});

  EXPECT_EQ(result2.x(), T{6});
  EXPECT_EQ(result2.y(), T{9});
  EXPECT_EQ(result2.z(), T{12});
}

TYPED_TEST(vector_arithmetic_test, VectorMultiplication)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{2}, T{3}, T{4}};
  vec3 v2{T{5}, T{6}, T{7}};

  auto result{v1 * v2};

  EXPECT_EQ(result.x(), T{10});
  EXPECT_EQ(result.y(), T{18});
  EXPECT_EQ(result.z(), T{28});
}

TYPED_TEST(vector_arithmetic_test, ScalarDivision)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  if constexpr (std::floating_point<T>)
  {
    vec3 v{T{6}, T{8}, T{12}};

    auto result{v / T{2}};

    EXPECT_EQ(result.x(), T{3});
    EXPECT_EQ(result.y(), T{4});
    EXPECT_EQ(result.z(), T{6});
  }
}

TYPED_TEST(vector_arithmetic_test, ArithmeticChaining)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{1}, T{2}, T{3}};
  vec3 v2{T{4}, T{5}, T{6}};
  vec3 v3{T{7}, T{8}, T{9}};

  auto result{v1 + v2 - v3};

  EXPECT_EQ(result.x(), T{-2}); // 1 + 4 - 7
  EXPECT_EQ(result.y(), T{-1}); // 2 + 5 - 8
  EXPECT_EQ(result.z(), T{0});  // 3 + 6 - 9
}
} // namespace pbrt::math::test
