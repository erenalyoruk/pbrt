#include <gtest/gtest.h>
#include <pbrt/math/vector.hpp>

namespace pbrt::math::test
{
using test_t = ::testing::Types<i32, f32, f64>;

template <typename T>
class vector_access_test : public ::testing::Test
{
public:
  using value_type = T;
};

TYPED_TEST_SUITE(vector_access_test, test_t);

// ============================================================================
// Element Access Tests
// ============================================================================

TYPED_TEST(vector_access_test, IndexOperator)
{
  using T = typename TestFixture::value_type;
  using vec4 = vector<T, 4>;

  vec4 v{T{1}, T{2}, T{3}, T{4}};

  // Read access
  EXPECT_EQ(v[0], T{1});
  EXPECT_EQ(v[1], T{2});
  EXPECT_EQ(v[2], T{3});
  EXPECT_EQ(v[3], T{4});

  // Write access
  v[0] = T{10};
  v[1] = T{20};
  v[2] = T{30};
  v[3] = T{40};

  EXPECT_EQ(v[0], T{10});
  EXPECT_EQ(v[1], T{20});
  EXPECT_EQ(v[2], T{30});
  EXPECT_EQ(v[3], T{40});
}

TYPED_TEST(vector_access_test, ConstIndexOperator)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  const vec3 v{T{1}, T{2}, T{3}};

  EXPECT_EQ(v[0], T{1});
  EXPECT_EQ(v[1], T{2});
  EXPECT_EQ(v[2], T{3});
}

TYPED_TEST(vector_access_test, NamedAccessors)
{
  using T = typename TestFixture::value_type;
  using vec4 = vector<T, 4>;

  vec4 v{T{1}, T{2}, T{3}, T{4}};

  // Read access
  EXPECT_EQ(v.x(), T{1});
  EXPECT_EQ(v.y(), T{2});
  EXPECT_EQ(v.z(), T{3});
  EXPECT_EQ(v.w(), T{4});

  // Write access
  v.x() = T{10};
  v.y() = T{20};
  v.z() = T{30};
  v.w() = T{40};

  EXPECT_EQ(v.x(), T{10});
  EXPECT_EQ(v.y(), T{20});
  EXPECT_EQ(v.z(), T{30});
  EXPECT_EQ(v.w(), T{40});
}

TYPED_TEST(vector_access_test, ConstNamedAccessors)
{
  using T = typename TestFixture::value_type;
  using vec4 = vector<T, 4>;

  const vec4 v{T{1}, T{2}, T{3}, T{4}};

  EXPECT_EQ(v.x(), T{1});
  EXPECT_EQ(v.y(), T{2});
  EXPECT_EQ(v.z(), T{3});
  EXPECT_EQ(v.w(), T{4});
}

TYPED_TEST(vector_access_test, DataPointer)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{1}, T{2}, T{3}};

  ASSERT_NE(v.data(), nullptr);
  EXPECT_EQ(v.data()[0], T{1});
  EXPECT_EQ(v.data()[1], T{2});
  EXPECT_EQ(v.data()[2], T{3});

  // Modify through data pointer
  v.data()[0] = T{10};
  EXPECT_EQ(v.x(), T{10});
}

TYPED_TEST(vector_access_test, ConstDataPointer)
{
  using T = typename TestFixture::value_type;
  using vec3 = vector<T, 3>;

  const vec3 v{T{1}, T{2}, T{3}};

  ASSERT_NE(v.data(), nullptr);
  EXPECT_EQ(v.data()[0], T{1});
  EXPECT_EQ(v.data()[1], T{2});
  EXPECT_EQ(v.data()[2], T{3});
}

TYPED_TEST(vector_access_test, SizeProperty)
{
  using T = typename TestFixture::value_type;

  EXPECT_EQ((vector<T, 1>{}.size()), 1);
  EXPECT_EQ((vector<T, 2>{}.size()), 2);
  EXPECT_EQ((vector<T, 3>{}.size()), 3);
  EXPECT_EQ((vector<T, 4>{}.size()), 4);
  EXPECT_EQ((vector<T, 16>{}.size()), 16);
}
} // namespace pbrt::math::test
