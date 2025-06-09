#include "gtest/gtest.h"
#include "pbrt/math/utility.hpp"

#include <gtest/gtest.h>

#include <type_traits>

namespace pbrt::math::test
{
using ScalarTypes = ::testing::Types<i8, i16, i32, i64, u8, u16, u32, u64, f32, f64>;
using FloatingPointTypes = ::testing::Types<f32, f64, long double>;
using IntegralTypes = ::testing::Types<i8, i16, i32, i64, u8, u16, u32, u64>;

template <typename T>
class UtilityScalarTest : public ::testing::Test
{
public:
  using ScalarType = T;
};

template <typename T>
class UtilityFloatingPointTest : public ::testing::Test
{
public:
  using FloatType = T;
  static constexpr T kTolerance = std::is_same_v<T, f32> ? T{1e-6f} : T{1e-12};
};

template <typename T>
class UtilityIntegralTest : public ::testing::Test
{
public:
  using IntegralType = T;
};

TYPED_TEST_SUITE(UtilityScalarTest, ScalarTypes);
TYPED_TEST_SUITE(UtilityFloatingPointTest, FloatingPointTypes);
TYPED_TEST_SUITE(UtilityIntegralTest, IntegralTypes);

TYPED_TEST(UtilityScalarTest, MinFunction)
{
  using T = typename TestFixture::ScalarType;

  EXPECT_EQ(min(T{1}, T{2}), T{1});
  EXPECT_EQ(min(T{2}, T{1}), T{1});

  if constexpr (std::is_signed_v<T>)
  {
    EXPECT_EQ(min(T{-1}, T{1}), T{-1});
    EXPECT_EQ(min(T{-5}, T{-3}), T{-5});
  }

  EXPECT_EQ(min(T{0}, T{0}), T{0});
}

TYPED_TEST(UtilityScalarTest, MaxFunction)
{
  using T = typename TestFixture::ScalarType;

  EXPECT_EQ(max(T{1}, T{2}), T{2});
  EXPECT_EQ(max(T{2}, T{1}), T{2});

  if constexpr (std::is_signed_v<T>)
  {
    EXPECT_EQ(max(T{-1}, T{1}), T{1});
    EXPECT_EQ(max(T{-5}, T{-3}), T{-3});
  }

  EXPECT_EQ(max(T{0}, T{0}), T{0});
}

TYPED_TEST(UtilityScalarTest, ClampFunction)
{
  using T = typename TestFixture::ScalarType;

  EXPECT_EQ(clamp(T{5}, T{1}, T{10}), T{5});
  EXPECT_EQ(clamp(T{0}, T{1}, T{10}), T{1});
  EXPECT_EQ(clamp(T{15}, T{1}, T{10}), T{10});
  EXPECT_EQ(clamp(T{1}, T{1}, T{10}), T{1});
  EXPECT_EQ(clamp(T{10}, T{1}, T{10}), T{10});

  if constexpr (std::is_signed_v<T>)
  {
    EXPECT_EQ(clamp(T{-5}, T{-10}, T{-1}), T{-5});
    EXPECT_EQ(clamp(T{-15}, T{-10}, T{-1}), T{-10});
    EXPECT_EQ(clamp(T{5}, T{-10}, T{-1}), T{-1});
  }
}

TYPED_TEST(UtilityFloatingPointTest, LerpFunction)
{
  using T = typename TestFixture::FloatType;

  EXPECT_EQ(lerp(T{0}, T{10}, T{0}), T{0});
  EXPECT_EQ(lerp(T{0}, T{10}, T{1}), T{10});
  EXPECT_NEAR(lerp(T{0}, T{10}, T{0.5}), T{5}, TestFixture::kTolerance);
  EXPECT_NEAR(lerp(T{-5}, T{5}, T{0.5}), T{0}, TestFixture::kTolerance);

  EXPECT_NEAR(lerp(T{0}, T{10}, T{2}), T{20}, TestFixture::kTolerance);
  EXPECT_NEAR(lerp(T{0}, T{10}, T{-1}), T{-10}, TestFixture::kTolerance);
}

TYPED_TEST(UtilityFloatingPointTest, RadiansFunction)
{
  using T = typename TestFixture::FloatType;

  EXPECT_EQ(radians(T{0}), T{0});
  EXPECT_NEAR(radians(T{180}), kPiV<T>, TestFixture::kTolerance);
  EXPECT_NEAR(radians(T{90}), kPiV<T> / T{2}, TestFixture::kTolerance);
  EXPECT_NEAR(radians(T{360}), T{2} * kPiV<T>, TestFixture::kTolerance);
  EXPECT_NEAR(radians(T{45}), kPiV<T> / T{4}, TestFixture::kTolerance);

  if constexpr (std::is_signed_v<T>)
  {
    EXPECT_NEAR(radians(T{-180}), -kPiV<T>, TestFixture::kTolerance);
    EXPECT_NEAR(radians(T{-90}), -kPiV<T> / T{2}, TestFixture::kTolerance);
  }
}

TYPED_TEST(UtilityFloatingPointTest, DegreesFunction)
{
  using T = typename TestFixture::FloatType;

  EXPECT_EQ(degrees(T{0}), T{0});
  EXPECT_NEAR(degrees(kPiV<T>), T{180}, TestFixture::kTolerance);
  EXPECT_NEAR(degrees(kPiV<T> / T{2}), T{90}, TestFixture::kTolerance);
  EXPECT_NEAR(degrees(T{2} * kPiV<T>), T{360}, TestFixture::kTolerance);
  EXPECT_NEAR(degrees(kPiV<T> / T{4}), T{45}, TestFixture::kTolerance);

  if constexpr (std::is_signed_v<T>)
  {
    EXPECT_NEAR(degrees(-kPiV<T>), T{-180}, TestFixture::kTolerance);
    EXPECT_NEAR(degrees(-kPiV<T> / T{2}), T{-90}, TestFixture::kTolerance);
  }
}

TYPED_TEST(UtilityFloatingPointTest, RadiansDegreesRoundTrip)
{
  using T = typename TestFixture::FloatType;

  std::vector<T> test_values{T{0}, T{30}, T{45}, T{90}, T{180}, T{270}, T{360}};

  if constexpr (std::is_signed_v<T>)
  {
    test_values.insert(test_values.end(), {T{-30}, T{-90}, T{-180}});
  }

  for (auto const deg : test_values)
  {
    EXPECT_NEAR(degrees(radians(deg)), deg, TestFixture::kTolerance);
  }

  std::vector<T> radian_values{T{0}, kPiV<T> / T{6}, kPiV<T> / T{4}, kPiV<T> / T{2}, kPiV<T>};
  for (auto const rad : radian_values)
  {
    EXPECT_NEAR(radians(degrees(rad)), rad, TestFixture::kTolerance);
  }
}

TYPED_TEST(UtilityFloatingPointTest, SqrtFunction)
{
  using T = typename TestFixture::FloatType;

  EXPECT_EQ(safe_sqrt(T{0}), T{0});
  EXPECT_EQ(safe_sqrt(T{1}), T{1});
  EXPECT_NEAR(safe_sqrt(T{4}), T{2}, TestFixture::kTolerance);
  EXPECT_NEAR(safe_sqrt(T{9}), T{3}, TestFixture::kTolerance);
  EXPECT_NEAR(safe_sqrt(T{2}), kSqrt2V<T>, TestFixture::kTolerance);

  EXPECT_EQ(safe_sqrt(T{-1}), T{0});
  EXPECT_EQ(safe_sqrt(T{-10}), T{0});

  EXPECT_GE(safe_sqrt(kEpsilonV<T>), T{0});
}

TYPED_TEST(UtilityScalarTest, ConstexprEvaluation)
{
  using T = typename TestFixture::ScalarType;

  constexpr auto min_result{min(T{1}, T{2})};
  constexpr auto max_result{max(T{1}, T{2})};
  constexpr auto clamp_result{clamp(T{5}, T{1}, T{10})};

  EXPECT_EQ(min_result, T{1});
  EXPECT_EQ(max_result, T{2});
  EXPECT_EQ(clamp_result, T{5});
}

TYPED_TEST(UtilityFloatingPointTest, ConstexprFloatEvaluation)
{
  using T = typename TestFixture::FloatType;

  constexpr auto lerp_result{lerp(T{0}, T{10}, T{0.5})};
  constexpr auto radians_result{radians(T{180})};
  constexpr auto degrees_result{degrees(kPiV<T>)};

  EXPECT_NEAR(lerp_result, T{5}, TestFixture::kTolerance);
  EXPECT_NEAR(radians_result, kPiV<T>, TestFixture::kTolerance);
  EXPECT_NEAR(degrees_result, T{180}, TestFixture::kTolerance);
}

TYPED_TEST(UtilityIntegralTest, IntegralSpecificTests)
{
  using T = typename TestFixture::IntegralType;

  constexpr auto max_val{std::numeric_limits<T>::max()};
  constexpr auto min_val{std::numeric_limits<T>::min()};

  EXPECT_EQ(max(min_val, max_val), max_val);
  EXPECT_EQ(min(min_val, max_val), min_val);
  EXPECT_EQ(clamp(max_val, min_val, max_val), max_val);
  EXPECT_EQ(clamp(min_val, min_val, max_val), min_val);
}
} // namespace pbrt::math::test
