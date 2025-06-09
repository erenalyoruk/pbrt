#include "gtest/gtest.h"
#include "pbrt/math/constants.hpp"
#include "pbrt/types.hpp"

#include <gtest/gtest.h>

#include <numbers>

namespace pbrt::math::test
{
using FloatingPointTypes = ::testing::Types<f32, f64, long double>;

template <typename T>
class ConstantsTypedTest : public ::testing::Test
{
public:
  using FloatType = T;
  static constexpr T kTolerance = std::is_same_v<T, f32> ? T{1e-6f} : T{1e-12};
};

TYPED_TEST_SUITE(ConstantsTypedTest, FloatingPointTypes);

TYPED_TEST(ConstantsTypedTest, PiConstants)
{
  using T = typename TestFixture::FloatType;

  EXPECT_EQ(kPiV<T>, std::numbers::pi_v<T>);
  EXPECT_EQ(kInvPiV<T>, std::numbers::inv_pi_v<T>);

  EXPECT_NEAR(kPiV<T> * kInvPiV<T>, T{1}, TestFixture::kTolerance);
}

TYPED_TEST(ConstantsTypedTest, Sqrt2Constants)
{
  using T = typename TestFixture::FloatType;

  EXPECT_EQ(kSqrt2V<T>, std::numbers::sqrt2_v<T>);
  EXPECT_NEAR(kInvSqrt2V<T>, T{1} / std::numbers::sqrt2_v<T>, TestFixture::kTolerance);

  EXPECT_NEAR(kSqrt2V<T> * kInvSqrt2V<T>, T{1}, TestFixture::kTolerance);
  EXPECT_NEAR(kSqrt2V<T> * kSqrt2V<T>, T{2}, TestFixture::kTolerance);
}

TYPED_TEST(ConstantsTypedTest, EulerConstant)
{
  using T = typename TestFixture::FloatType;

  EXPECT_EQ(kEulerV<T>, std::numbers::e_v<T>);
  EXPECT_GT(kEulerV<T>, T{2.7});
  EXPECT_LT(kEulerV<T>, T{2.8});
}

TYPED_TEST(ConstantsTypedTest, EpsilonValues)
{
  using T = typename TestFixture::FloatType;

  EXPECT_EQ(kEpsilonScaling<T>, T{16});
  EXPECT_EQ(kEpsilonV<T>, std::numeric_limits<T>::epsilon() * kEpsilonScaling<T>);
  EXPECT_GT(kEpsilonV<T>, T{0});
  EXPECT_LT(kEpsilonV<T>, T{1});
}

TYPED_TEST(ConstantsTypedTest, ShadowEpsilon)
{
  using T = typename TestFixture::FloatType;

  EXPECT_EQ(kShadowEpsilonV<T>, T{0.0001});
  EXPECT_GT(kShadowEpsilonV<T>, kEpsilonV<T>);
}

TYPED_TEST(ConstantsTypedTest, ConstexprEvaluation)
{
  using T = typename TestFixture::FloatType;

  constexpr auto pi{kPiV<T>};
  constexpr auto inv_pi{kInvPiV<T>};
  constexpr auto sqrt2{kSqrt2V<T>};
  constexpr auto inv_sqrt2{kInvSqrt2V<T>};
  constexpr auto euler{kEulerV<T>};
  constexpr auto epsilon{kEpsilonV<T>};
  constexpr auto shadow_epsilon{kShadowEpsilonV<T>};

  EXPECT_GT(pi, T{3});
  EXPECT_LT(inv_pi, T{1});
  EXPECT_GT(sqrt2, T{1});
  EXPECT_LT(inv_sqrt2, T{1});
  EXPECT_GT(euler, T{2});
  EXPECT_GT(epsilon, T{0});
  EXPECT_GT(shadow_epsilon, T{0});
}

TYPED_TEST(ConstantsTypedTest, PrecisionSpecificValues)
{
  using T = typename TestFixture::FloatType;

  if constexpr (std::is_same_v<T, f32>)
  {
    EXPECT_LT(kEpsilonV<T>, 1e-5f);
    EXPECT_GT(kEpsilonV<T>, 1e-8f);
  }
  else if constexpr (std::is_same_v<T, f64>)
  {
    EXPECT_LT(kEpsilonV<T>, 1e-14);
    EXPECT_GT(kEpsilonV<T>, 1e-17);
  }
}
} // namespace pbrt::math::test
