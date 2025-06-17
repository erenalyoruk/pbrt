#include <gtest/gtest.h>
#include <pbrt/math/vector.hpp>

namespace pbrt::math::test
{
using test_t = ::testing::Types<i32, f32, f64>;

template <typename T>
class vector_comparison_test : public ::testing::Test
{
public:
  using value_type = T;
  static constexpr T kTolerance = std::is_same_v<T, f32> ? T{1e-6F} : T{1e-15};
};

TYPED_TEST_SUITE(vector_comparison_test, test_t);

// ============================================================================
// Comparison Tests
// ============================================================================

TYPED_TEST(vector_comparison_test, EqualityOperator)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{1}, T{2}, T{3}};
  vec3 v2{T{1}, T{2}, T{3}};
  vec3 v3{T{1}, T{2}, T{4}};

  EXPECT_TRUE(v1 == v2);
  EXPECT_FALSE(v1 == v3);

  // Self-equality
  EXPECT_TRUE(v1 == v1);
}

TYPED_TEST(vector_comparison_test, InequalityOperator)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{1}, T{2}, T{3}};
  vec3 v2{T{1}, T{2}, T{3}};
  vec3 v3{T{1}, T{2}, T{4}};

  EXPECT_FALSE(v1 != v2);
  EXPECT_TRUE(v1 != v3);

  // Self-inequality
  EXPECT_FALSE(v1 != v1);
}

TYPED_TEST(vector_comparison_test, ZeroVectorComparison)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 zero1{};
  vec3 zero2{T{0}, T{0}, T{0}};
  vec3 non_zero{T{1}, T{0}, T{0}};

  EXPECT_TRUE(zero1 == zero2);
  EXPECT_FALSE(zero1 == non_zero);
  EXPECT_TRUE(zero1 != non_zero);
}

TYPED_TEST(vector_comparison_test, FloatingPointComparison)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  if constexpr (std::floating_point<T>)
  {
    // Test exact equality
    vec3 v1{T{1.0}, T{2.0}, T{3.0}};
    vec3 v2{T{1.0}, T{2.0}, T{3.0}};

    EXPECT_TRUE(v1 == v2);

    // Test near-equality (vectors should be exactly equal for == to work)
    vec3 v3{T{1.0} + std::numeric_limits<T>::epsilon(), T{2.0}, T{3.0}};
    EXPECT_FALSE(v1 == v3); // Exact comparison, not approximate
  }
}

TYPED_TEST(vector_comparison_test, DifferentSizeComparison)
{
  using T = typename TestFixture::value_type;
  using vec2 = vector<T, 2>;
  using vec3 = vector<T, 3>;

  // These should be different types and not comparable
  static_assert(!std::is_same_v<vec2, vec3>);

  vec2 v2{T{1}, T{2}};
  vec3 v3{T{1}, T{2}, T{0}};

  // This should not compile:
  // EXPECT_FALSE(v2 == v3);
}

TYPED_TEST(vector_comparison_test, ComparisonWithTemporaries)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{1}, T{2}, T{3}};

  EXPECT_TRUE((v == vec3{T{1}, T{2}, T{3}}));
  EXPECT_FALSE((v == vec3{T{1}, T{2}, T{4}}));
  EXPECT_TRUE((v != vec3{T{1}, T{2}, T{4}}));
}
} // namespace pbrt::math::test
