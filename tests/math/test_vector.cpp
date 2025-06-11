#include <gtest/gtest.h>
#include <pbrt/math/vector.hpp>

#include <type_traits>

namespace pbrt::math::test
{
using scalar_t = ::testing::Types<i8, i16, i32, i64, u8, u16, u32, u64, f32, f64>;
using floating_point_t = ::testing::Types<f32, f64>;
using integral_t = ::testing::Types<i8, i16, i32, i64, u8, u16, u32, u64>;
using simd_compatible_t = ::testing::Types<f32, f64>;

template <typename T>
class vector_scalar_test : public ::testing::Test
{
public:
  using scalar_type = T;
};

template <typename T>
class vector_floating_test : public ::testing::Test
{
public:
  using float_type = T;
  static constexpr T kTolerance = std::is_same_v<T, f32> ? T{1e-6F} : T{1e-15};
};

template <typename T>
class vector_simd_test : public ::testing::Test
{
public:
  using float_type = T;
  static constexpr T kTolerance = std::is_same_v<T, f32> ? T{1e-6F} : T{1e-15};

  static constexpr auto get_simd_size() -> usize
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

  static constexpr auto get_avx_size() -> usize
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

TYPED_TEST_SUITE(vector_scalar_test, scalar_t);
TYPED_TEST_SUITE(vector_floating_test, floating_point_t);
TYPED_TEST_SUITE(vector_simd_test, simd_compatible_t);

TYPED_TEST(vector_scalar_test, DefaultConstructor)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;
  using vec4 = vector<T, 4>;

  vec3 v3{};
  vec4 v4{};

  EXPECT_EQ(v3.x(), T{0});
  EXPECT_EQ(v3.y(), T{0});
  EXPECT_EQ(v3.z(), T{0});

  EXPECT_EQ(v4.x(), T{0});
  EXPECT_EQ(v4.y(), T{0});
  EXPECT_EQ(v4.z(), T{0});
  EXPECT_EQ(v4.w(), T{0});
}

TYPED_TEST(vector_scalar_test, ScalarConstructor)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;
  using vec4 = vector<T, 4>;

  constexpr T test_value{5};
  vec3 v3{test_value};
  vec4 v4{test_value};

  EXPECT_EQ(v3.x(), test_value);
  EXPECT_EQ(v3.y(), test_value);
  EXPECT_EQ(v3.z(), test_value);

  EXPECT_EQ(v4.x(), test_value);
  EXPECT_EQ(v4.y(), test_value);
  EXPECT_EQ(v4.z(), test_value);
  EXPECT_EQ(v4.w(), test_value);
}

TYPED_TEST(vector_scalar_test, ComponentConstructor)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;
  using vec4 = vector<T, 4>;

  vec3 v3{T{1}, T{2}, T{3}};
  vec4 v4{T{1}, T{2}, T{3}, T{4}};

  EXPECT_EQ(v3.x(), T{1});
  EXPECT_EQ(v3.y(), T{2});
  EXPECT_EQ(v3.z(), T{3});

  EXPECT_EQ(v4.x(), T{1});
  EXPECT_EQ(v4.y(), T{2});
  EXPECT_EQ(v4.z(), T{3});
  EXPECT_EQ(v4.w(), T{4});
}

TYPED_TEST(vector_scalar_test, ElementAccess)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{1}, T{2}, T{3}};

  EXPECT_EQ(v[0], T{1});
  EXPECT_EQ(v[1], T{2});
  EXPECT_EQ(v[2], T{3});

  v[0] = T{10};
  v[1] = T{20};
  v[2] = T{30};

  EXPECT_EQ(v[0], T{10});
  EXPECT_EQ(v[1], T{20});
  EXPECT_EQ(v[2], T{30});
}

TYPED_TEST(vector_scalar_test, NamedAccessors)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{1}, T{2}, T{3}};

  EXPECT_EQ(v.x(), T{1});
  EXPECT_EQ(v.y(), T{2});
  EXPECT_EQ(v.z(), T{3});

  v.x() = T{10};
  v.y() = T{20};
  v.z() = T{30};

  EXPECT_EQ(v.x(), T{10});
  EXPECT_EQ(v.y(), T{20});
  EXPECT_EQ(v.z(), T{30});
}

TYPED_TEST(vector_scalar_test, SizeAndDataAccess)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;
  using vec4 = vector<T, 4>;

  vec3 v3{T{1}, T{2}, T{3}};
  vec4 v4{T{1}, T{2}, T{3}, T{4}};

  EXPECT_EQ(v3.size(), 3);
  EXPECT_EQ(v4.size(), 4);

  EXPECT_NE(v3.data(), nullptr);
  EXPECT_NE(v4.data(), nullptr);
}

TYPED_TEST(vector_scalar_test, AdditionAssignment)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{1}, T{2}, T{3}};
  vec3 v2{T{4}, T{5}, T{6}};

  v1 += v2;

  EXPECT_EQ(v1.x(), T{5});
  EXPECT_EQ(v1.y(), T{7});
  EXPECT_EQ(v1.z(), T{9});
}

TYPED_TEST(vector_scalar_test, SubtractionAssignment)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{4}, T{5}, T{6}};
  vec3 v2{T{1}, T{2}, T{3}};

  v1 -= v2;

  EXPECT_EQ(v1.x(), T{3});
  EXPECT_EQ(v1.y(), T{3});
  EXPECT_EQ(v1.z(), T{3});
}

TYPED_TEST(vector_scalar_test, ScalarMultiplicationAssignment)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{1}, T{2}, T{3}};

  v *= T{2};

  EXPECT_EQ(v.x(), T{2});
  EXPECT_EQ(v.y(), T{4});
  EXPECT_EQ(v.z(), T{6});
}

TYPED_TEST(vector_simd_test, SimdCompatibility)
{
  using T = typename TestFixture::float_type;
  constexpr usize simd_size = TestFixture::get_simd_size();
  constexpr usize avx_size = TestFixture::get_avx_size();

  using vec_simd = vector<T, simd_size>;
  using vec_avx = vector<T, avx_size>;
  using vec3 = vector<T, 3>;

  EXPECT_TRUE(vec_simd::kIsSimdCompatible);
  EXPECT_TRUE(vec_avx::kIsSimdCompatible);
  EXPECT_FALSE(vec3::kIsSimdCompatible);
}

TYPED_TEST(vector_simd_test, SimdAlignment)
{
  using T = typename TestFixture::float_type;
  constexpr usize simd_size = TestFixture::get_simd_size();
  constexpr usize avx_size = TestFixture::get_avx_size();

  using vec_simd = vector<T, simd_size>;
  using vec_avx = vector<T, avx_size>;

  vec_simd v_simd{};
  vec_avx v_avx{};

  auto ptr_simd{std::bit_cast<uintptr>(&v_simd)};
  auto ptr_avx{std::bit_cast<uintptr>(&v_avx)};

  if constexpr (std::same_as<T, f32>)
  {
    EXPECT_EQ(ptr_simd % 16, 0) << "ptr_simd is not aligned to 16 bytes";
    EXPECT_EQ(ptr_avx % 32, 0) << "ptr_avx is not aligned to 32 bytes";
  }
  else if constexpr (std::same_as<T, f64>)
  {
    EXPECT_EQ(ptr_simd % 16, 0) << "ptr_simd is not aligned to 16 bytes";
    EXPECT_EQ(ptr_avx % 32, 0) << "ptr_avx is not aligned to 32 bytes";
  }
}

TYPED_TEST(vector_simd_test, SimdDataAccess)
{
  using T = typename TestFixture::float_type;
  constexpr usize simd_size = TestFixture::get_simd_size();
  using vec_simd = vector<T, simd_size>;

  if constexpr (std::same_as<T, f32>)
  {
    vec_simd vec{T{1}, T{2}, T{3}, T{4}};
    auto simd_data{vec.simd()};

    alignas(16) std::array<T, 4> test_data{};
    simde_mm_store_ps(test_data.data(), simd_data);

    EXPECT_NEAR(test_data[0], T{1}, TestFixture::kTolerance);
    EXPECT_NEAR(test_data[1], T{2}, TestFixture::kTolerance);
    EXPECT_NEAR(test_data[2], T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(test_data[3], T{4}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    vec_simd vec{T{1}, T{2}};
    auto simd_data{vec.simd()};

    alignas(16) std::array<T, 2> test_data{};
    simde_mm_store_pd(test_data.data(), simd_data);

    EXPECT_NEAR(test_data[0], T{1}, TestFixture::kTolerance);
    EXPECT_NEAR(test_data[1], T{2}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdArithmetic)
{
  using T = typename TestFixture::float_type;
  constexpr usize simd_size = TestFixture::get_simd_size();
  using vec_simd = vector<T, simd_size>;

  if constexpr (std::same_as<T, f32>)
  {
    vec_simd v1{T{1}, T{2}, T{3}, T{4}};
    vec_simd v2{T{5}, T{6}, T{7}, T{8}};

    v1 += v2;

    EXPECT_NEAR(v1.x(), T{6}, TestFixture::kTolerance);
    EXPECT_NEAR(v1.y(), T{8}, TestFixture::kTolerance);
    EXPECT_NEAR(v1.z(), T{10}, TestFixture::kTolerance);
    EXPECT_NEAR(v1.w(), T{12}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    vec_simd v1{T{1}, T{2}};
    vec_simd v2{T{3}, T{4}};

    v1 += v2;

    EXPECT_NEAR(v1.x(), T{4}, TestFixture::kTolerance);
    EXPECT_NEAR(v1.y(), T{6}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdScalarMultiplication)
{
  using T = typename TestFixture::float_type;
  constexpr usize simd_size{TestFixture::get_simd_size()};
  using vec_simd = vector<T, simd_size>;

  if constexpr (std::same_as<T, f32>)
  {
    vec_simd v{T{1}, T{2}, T{3}, T{4}};
    v *= T{3};

    EXPECT_NEAR(v.x(), T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(v.y(), T{6}, TestFixture::kTolerance);
    EXPECT_NEAR(v.z(), T{9}, TestFixture::kTolerance);
    EXPECT_NEAR(v.w(), T{12}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    vec_simd v{T{1}, T{2}};
    v *= T{3};

    EXPECT_NEAR(v.x(), T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(v.y(), T{6}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, LargeVectors)
{
  using T = typename TestFixture::float_type;
  using vec16 = vector<T, 16>;

  constexpr T test_value{1};
  vec16 vec{test_value};

  for (usize i{}; i < 16; ++i)
  {
    EXPECT_EQ(vec[i], test_value);
  }
}

TYPED_TEST(vector_simd_test, SingleElementVector)
{
  using T = typename TestFixture::float_type;
  using vec1 = vector<T, 1>;

  constexpr T test_value{42};
  vec1 vec{test_value};

  EXPECT_EQ(vec[0], test_value);
  EXPECT_EQ(vec.x(), test_value);
}

// Compile-time tests for different types
TYPED_TEST(vector_simd_test, CompileTimeProperties)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;
  using vec4 = vector<T, 4>;

  static_assert(std::is_same_v<typename vec3::value_type, T>);
  static_assert(std::is_same_v<typename vec4::value_type, T>);
}
} // namespace pbrt::math::test
