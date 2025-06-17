#include <gtest/gtest.h>
#include <pbrt/math/vector.hpp>

#include <algorithm>
#include <numeric>

namespace pbrt::math::test
{
using test_t = ::testing::Types<i32, f32, f64>;

template <typename T>
class vector_iterator_test : public ::testing::Test
{
public:
  using value_type = T;
};

TYPED_TEST_SUITE(vector_iterator_test, test_t);

// ============================================================================
// Iterator Tests
// ============================================================================

TYPED_TEST(vector_iterator_test, ForwardIteration)
{
  using T = typename TestFixture::value_type;
  using vec4 = vector<T, 4>;

  vec4 v{T{1}, T{2}, T{3}, T{4}};

  auto it{v.begin()};
  EXPECT_EQ(*it, T{1});

  ++it;
  EXPECT_EQ(*it, T{2});

  ++it;
  EXPECT_EQ(*it, T{3});

  ++it;
  EXPECT_EQ(*it, T{4});

  ++it;
  EXPECT_EQ(it, v.end());
}

TYPED_TEST(vector_iterator_test, RangeBasedLoop)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{1}, T{2}, T{3}};

  T sum{0};
  for (auto value : v)
  {
    sum += value;
  }

  EXPECT_EQ(sum, T{6});
}

TYPED_TEST(vector_iterator_test, ConstIteration)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  const vec3 v{T{1}, T{2}, T{3}};

  auto it{v.cbegin()};
  EXPECT_EQ(*it, T{1});

  std::advance(it, 2);
  EXPECT_EQ(*it, T{3});

  ++it;
  EXPECT_EQ(it, v.cend());
}

TYPED_TEST(vector_iterator_test, ReverseIteration)
{
  using T = typename TestFixture::value_type;
  using vec4 = vector<T, 4>;

  vec4 v{T{1}, T{2}, T{3}, T{4}};

  auto rit{v.rbegin()};
  EXPECT_EQ(*rit, T{4});

  ++rit;
  EXPECT_EQ(*rit, T{3});

  ++rit;
  EXPECT_EQ(*rit, T{2});

  ++rit;
  EXPECT_EQ(*rit, T{1});

  ++rit;
  EXPECT_EQ(rit, v.rend());
}

TYPED_TEST(vector_iterator_test, StdAlgorithms)
{
  using T = typename TestFixture::value_type;
  using vec4 = vector<T, 4>;

  vec4 v{T{4}, T{2}, T{3}, T{1}};

  // Find algorithm
  auto found{std::find(v.begin(), v.end(), T{3})};
  EXPECT_NE(found, v.end());
  EXPECT_EQ(*found, T{3});

  // Accumulate
  auto sum{std::accumulate(v.begin(), v.end(), T{0})};
  EXPECT_EQ(sum, T{10});

  // Sort (modifies vector)
  std::sort(v.begin(), v.end());
  EXPECT_EQ(v[0], T{1});
  EXPECT_EQ(v[1], T{2});
  EXPECT_EQ(v[2], T{3});
  EXPECT_EQ(v[3], T{4});
}

TYPED_TEST(vector_iterator_test, IteratorTypes)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{};

  // Check iterator types
  static_assert(std::is_same_v<decltype(v.begin()), typename vec3::iterator>);
  static_assert(std::is_same_v<decltype(v.end()), typename vec3::iterator>);
  static_assert(std::is_same_v<decltype(v.cbegin()), typename vec3::const_iterator>);
  static_assert(std::is_same_v<decltype(v.cend()), typename vec3::const_iterator>);
  static_assert(std::is_same_v<decltype(v.rbegin()), typename vec3::reverse_iterator>);
  static_assert(std::is_same_v<decltype(v.rend()), typename vec3::reverse_iterator>);
}

TYPED_TEST(vector_iterator_test, IteratorDistance)
{
  using T = typename TestFixture::value_type;
  using vec5 = vector<T, 5>;

  vec5 v{};

  EXPECT_EQ(std::distance(v.begin(), v.end()), 5);
  EXPECT_EQ(std::distance(v.cbegin(), v.cend()), 5);
  EXPECT_EQ(std::distance(v.rbegin(), v.rend()), 5);
}
} // namespace pbrt::math::test
