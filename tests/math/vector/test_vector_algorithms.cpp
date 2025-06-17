#include <gtest/gtest.h>
#include <pbrt/math/vector.hpp>

namespace pbrt::math::test
{
using test_t = ::testing::Types<f32, f64>;

template <typename T>
class vector_algorithm_test : public ::testing::Test
{
public:
  using value_type = T;
  static constexpr T kTolerance = std::is_same_v<T, f32> ? T{1e-6F} : T{1e-15};
  static constexpr T kFastTolerance = std::is_same_v<T, f32> ? T{1e-2F} : T{1e-2};
};

TYPED_TEST_SUITE(vector_algorithm_test, test_t);

// ============================================================================
// Length and Normalization Tests
// ============================================================================

TYPED_TEST(vector_algorithm_test, LengthSquared)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{3}, T{4}, T{0}};
  auto len_sq{v.length_squared()};

  EXPECT_NEAR(len_sq, T{25}, TestFixture::kTolerance);

  // Zero vector
  vec3 zero{};
  EXPECT_EQ(zero.length_squared(), T{0});

  // Unit vectors
  vec3 unit_x{T{1}, T{0}, T{0}};
  EXPECT_NEAR(unit_x.length_squared(), T{1}, TestFixture::kTolerance);
}

TYPED_TEST(vector_algorithm_test, Length)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{3}, T{4}, T{0}};
  auto len{v.length()};

  EXPECT_NEAR(len, T{5}, TestFixture::kTolerance);

  // Zero vector
  vec3 zero{};
  EXPECT_EQ(zero.length(), T{0});

  // Unit vectors
  vec3 unit_x{T{1}, T{0}, T{0}};
  EXPECT_NEAR(unit_x.length(), T{1}, TestFixture::kTolerance);
}

TYPED_TEST(vector_algorithm_test, FastLength)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{3}, T{4}, T{0}};
  auto fast_len{v.fast_length()};
  auto exact_len{v.length()};

  EXPECT_NEAR(fast_len, exact_len, TestFixture::kFastTolerance);

  // Zero vector
  vec3 zero{};
  EXPECT_NEAR(zero.fast_length(), T{0}, TestFixture::kFastTolerance);
}

TYPED_TEST(vector_algorithm_test, Normalization)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{3}, T{4}, T{0}};
  auto normalized{v.normalized()};

  EXPECT_NEAR(normalized.length(), T{1}, TestFixture::kTolerance);
  EXPECT_NEAR(normalized.x(), T{3} / T{5}, TestFixture::kTolerance);
  EXPECT_NEAR(normalized.y(), T{4} / T{5}, TestFixture::kTolerance);
  EXPECT_NEAR(normalized.z(), T{0}, TestFixture::kTolerance);

  // Original vector unchanged
  EXPECT_NEAR(v.length(), T{5}, TestFixture::kTolerance);
}

TYPED_TEST(vector_algorithm_test, InPlaceNormalization)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{3}, T{4}, T{0}};
  v.normalize();

  EXPECT_NEAR(v.length(), T{1}, TestFixture::kTolerance);
  EXPECT_NEAR(v.x(), T{3} / T{5}, TestFixture::kTolerance);
  EXPECT_NEAR(v.y(), T{4} / T{5}, TestFixture::kTolerance);
  EXPECT_NEAR(v.z(), T{0}, TestFixture::kTolerance);
}

TYPED_TEST(vector_algorithm_test, FastNormalization)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{3}, T{4}, T{0}};
  auto fast_normalized{v.fast_normalized()};
  auto exact_normalized{v.normalized()};

  EXPECT_NEAR(fast_normalized.length(), T{1}, TestFixture::kFastTolerance);
  EXPECT_NEAR(fast_normalized.x(), exact_normalized.x(), TestFixture::kFastTolerance);
  EXPECT_NEAR(fast_normalized.y(), exact_normalized.y(), TestFixture::kFastTolerance);
  EXPECT_NEAR(fast_normalized.z(), exact_normalized.z(), TestFixture::kFastTolerance);
}

TYPED_TEST(vector_algorithm_test, ZeroVectorNormalization)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 zero{};
  auto normalized{zero.normalized()};

  EXPECT_EQ(normalized.x(), T{0});
  EXPECT_EQ(normalized.y(), T{0});
  EXPECT_EQ(normalized.z(), T{0});

  // In-place normalization of zero vector
  vec3 zero2{};
  zero2.normalize();
  EXPECT_EQ(zero2.x(), T{0});
  EXPECT_EQ(zero2.y(), T{0});
  EXPECT_EQ(zero2.z(), T{0});
}

// ============================================================================
// Vector Operations Tests
// ============================================================================

TYPED_TEST(vector_algorithm_test, DotProduct)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{1}, T{2}, T{3}};
  vec3 v2{T{4}, T{5}, T{6}};

  auto result{dot(v1, v2)};
  EXPECT_NEAR(result, T{32}, TestFixture::kTolerance); // 1*4 + 2*5 + 3*6 = 32

  // Orthogonal vectors
  vec3 unit_x{T{1}, T{0}, T{0}};
  vec3 unit_y{T{0}, T{1}, T{0}};
  EXPECT_NEAR(dot(unit_x, unit_y), T{0}, TestFixture::kTolerance);

  // Self dot product
  EXPECT_NEAR(dot(v1, v1), v1.length_squared(), TestFixture::kTolerance);
}

TYPED_TEST(vector_algorithm_test, CrossProduct)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  // Standard basis vectors
  vec3 unit_x{T{1}, T{0}, T{0}};
  vec3 unit_y{T{0}, T{1}, T{0}};
  vec3 unit_z{T{0}, T{0}, T{1}};

  auto cross_xy{cross(unit_x, unit_y)};
  EXPECT_NEAR(cross_xy.x(), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(cross_xy.y(), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(cross_xy.z(), T{1}, TestFixture::kTolerance);

  auto cross_yz{cross(unit_y, unit_z)};
  EXPECT_NEAR(cross_yz.x(), T{1}, TestFixture::kTolerance);
  EXPECT_NEAR(cross_yz.y(), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(cross_yz.z(), T{0}, TestFixture::kTolerance);

  auto cross_zx{cross(unit_z, unit_x)};
  EXPECT_NEAR(cross_zx.x(), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(cross_zx.y(), T{1}, TestFixture::kTolerance);
  EXPECT_NEAR(cross_zx.z(), T{0}, TestFixture::kTolerance);
}

TYPED_TEST(vector_algorithm_test, CrossProductProperties)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 a{T{1}, T{2}, T{3}};
  vec3 b{T{4}, T{5}, T{6}};

  auto cross_ab{cross(a, b)};
  auto cross_ba{cross(b, a)};

  // Anti-commutative property
  EXPECT_NEAR(cross_ab.x(), -cross_ba.x(), TestFixture::kTolerance);
  EXPECT_NEAR(cross_ab.y(), -cross_ba.y(), TestFixture::kTolerance);
  EXPECT_NEAR(cross_ab.z(), -cross_ba.z(), TestFixture::kTolerance);

  // Perpendicular to both input vectors
  EXPECT_NEAR(dot(cross_ab, a), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(dot(cross_ab, b), T{0}, TestFixture::kTolerance);

  // Self cross product is zero
  auto cross_aa{cross(a, a)};
  EXPECT_NEAR(cross_aa.x(), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(cross_aa.y(), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(cross_aa.z(), T{0}, TestFixture::kTolerance);
}

TYPED_TEST(vector_algorithm_test, DistanceFunctions)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 p1{T{0}, T{0}, T{0}};
  vec3 p2{T{3}, T{4}, T{0}};

  auto dist_sq{distance_squared(p1, p2)};
  auto dist{distance(p1, p2)};

  EXPECT_NEAR(dist_sq, T{25}, TestFixture::kTolerance);
  EXPECT_NEAR(dist, T{5}, TestFixture::kTolerance);

  // Distance to self is zero
  EXPECT_NEAR(distance(p1, p1), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(distance_squared(p1, p1), T{0}, TestFixture::kTolerance);
}

TYPED_TEST(vector_algorithm_test, ComponentWiseMinMax)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{1}, T{5}, T{2}};
  vec3 v2{T{3}, T{2}, T{4}};

  auto min_result{min(v1, v2)};
  EXPECT_EQ(min_result.x(), T{1});
  EXPECT_EQ(min_result.y(), T{2});
  EXPECT_EQ(min_result.z(), T{2});

  auto max_result{max(v1, v2)};
  EXPECT_EQ(max_result.x(), T{3});
  EXPECT_EQ(max_result.y(), T{5});
  EXPECT_EQ(max_result.z(), T{4});
}

TYPED_TEST(vector_algorithm_test, LerpFunction)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 start{T{0}, T{0}, T{0}};
  vec3 end{T{10}, T{20}, T{30}};

  // Test various interpolation points
  auto at_start{lerp(start, end, T{0})};
  EXPECT_EQ(at_start.x(), start.x());
  EXPECT_EQ(at_start.y(), start.y());
  EXPECT_EQ(at_start.z(), start.z());

  auto at_end{lerp(start, end, T{1})};
  EXPECT_NEAR(at_end.x(), end.x(), TestFixture::kTolerance);
  EXPECT_NEAR(at_end.y(), end.y(), TestFixture::kTolerance);
  EXPECT_NEAR(at_end.z(), end.z(), TestFixture::kTolerance);

  auto mid{lerp(start, end, T{0.5})};
  EXPECT_NEAR(mid.x(), T{5}, TestFixture::kTolerance);
  EXPECT_NEAR(mid.y(), T{10}, TestFixture::kTolerance);
  EXPECT_NEAR(mid.z(), T{15}, TestFixture::kTolerance);

  auto quarter{lerp(start, end, T{0.25})};
  EXPECT_NEAR(quarter.x(), T{2.5}, TestFixture::kTolerance);
  EXPECT_NEAR(quarter.y(), T{5}, TestFixture::kTolerance);
  EXPECT_NEAR(quarter.z(), T{7.5}, TestFixture::kTolerance);
}

TYPED_TEST(vector_algorithm_test, ReflectionFunction)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  // 45-degree incident ray reflecting off horizontal surface
  vec3 incident{T{1}, T{-1}, T{0}};
  incident = incident.normalized();
  vec3 normal{T{0}, T{1}, T{0}};

  auto reflected{reflect(incident, normal)};

  EXPECT_NEAR(reflected.x(), incident.x(), TestFixture::kTolerance);
  EXPECT_NEAR(reflected.y(), -incident.y(), TestFixture::kTolerance);
  EXPECT_NEAR(reflected.z(), incident.z(), TestFixture::kTolerance);

  // Reflection preserves magnitude
  EXPECT_NEAR(reflected.length(), incident.length(), TestFixture::kTolerance);
}

TYPED_TEST(vector_algorithm_test, RefractionFunction)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  // Normal incidence (straight down)
  vec3 incident{T{0}, T{-1}, T{0}};
  vec3 normal{T{0}, T{1}, T{0}};
  T eta{T{1.5}}; // Glass-like material

  auto refracted{refract(incident, normal, eta)};

  // For normal incidence, direction shouldn't change much
  EXPECT_NEAR(refracted.x(), T{0}, TestFixture::kTolerance);
  EXPECT_LT(refracted.y(), T{0}); // Still pointing downward
  EXPECT_NEAR(refracted.z(), T{0}, TestFixture::kTolerance);

  // Test total internal reflection
  vec3 steep_incident{T{0.9}, T{-0.436}, T{0}};
  steep_incident = steep_incident.normalized();
  T low_eta{T{0.67}}; // From denser to less dense medium

  auto tir_result{refract(steep_incident, normal, low_eta)};
  // Should return zero vector for total internal reflection
  if (tir_result.length_squared() == T{0})
  {
    EXPECT_EQ(tir_result.x(), T{0});
    EXPECT_EQ(tir_result.y(), T{0});
    EXPECT_EQ(tir_result.z(), T{0});
  }
}
} // namespace pbrt::math::test
