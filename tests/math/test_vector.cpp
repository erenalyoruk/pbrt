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

TYPED_TEST(vector_scalar_test, DivisionAssignment)
{
  using T = typename TestFixture::scalar_type;
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

TYPED_TEST(vector_scalar_test, VectorDivisionAssignment)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;

  if constexpr (std::floating_point<T>)
  {
    vec3 v1{T{8}, T{12}, T{15}};
    vec3 v2{T{2}, T{3}, T{5}};

    v1 /= v2;

    EXPECT_EQ(v1.x(), T{4});
    EXPECT_EQ(v1.y(), T{4});
    EXPECT_EQ(v1.z(), T{3});
  }
}

TYPED_TEST(vector_scalar_test, EqualityComparison)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{1}, T{2}, T{3}};
  vec3 v2{T{1}, T{2}, T{3}};
  vec3 v3{T{1}, T{2}, T{4}};

  EXPECT_TRUE(v1 == v2);
  EXPECT_FALSE(v1 == v3);
  EXPECT_FALSE(v1 != v2);
  EXPECT_TRUE(v1 != v3);
}

TYPED_TEST(vector_scalar_test, FreestandingArithmetic)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{1}, T{2}, T{3}};
  vec3 v2{T{4}, T{5}, T{6}};

  auto result_add{v1 + v2};
  EXPECT_EQ(result_add.x(), T{5});
  EXPECT_EQ(result_add.y(), T{7});
  EXPECT_EQ(result_add.z(), T{9});

  auto result_sub{v2 - v1};
  EXPECT_EQ(result_sub.x(), T{3});
  EXPECT_EQ(result_sub.y(), T{3});
  EXPECT_EQ(result_sub.z(), T{3});

  auto result_mul{v1 * v2};
  EXPECT_EQ(result_mul.x(), T{4});
  EXPECT_EQ(result_mul.y(), T{10});
  EXPECT_EQ(result_mul.z(), T{18});

  auto result_scalar_mul{v1 * T{2}};
  EXPECT_EQ(result_scalar_mul.x(), T{2});
  EXPECT_EQ(result_scalar_mul.y(), T{4});
  EXPECT_EQ(result_scalar_mul.z(), T{6});
}

TYPED_TEST(vector_scalar_test, UnaryNegation)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;

  if constexpr (std::is_signed_v<T>)
  {
    vec3 v{T{1}, T{-2}, T{3}};
    auto negated{-v};

    EXPECT_EQ(negated.x(), T{-1});
    EXPECT_EQ(negated.y(), T{2});
    EXPECT_EQ(negated.z(), T{-3});
  }
}

TYPED_TEST(vector_floating_test, LengthFunctions)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{3}, T{4}, T{0}};
  auto length_sq{v.length_squared()};
  auto length{v.length()};

  EXPECT_NEAR(length_sq, T{25}, TestFixture::kTolerance);
  EXPECT_NEAR(length, T{5}, TestFixture::kTolerance);

  vec3 unit{T{1}, T{0}, T{0}};
  EXPECT_NEAR(unit.length(), T{1}, TestFixture::kTolerance);
  EXPECT_NEAR(unit.length_squared(), T{1}, TestFixture::kTolerance);
}

TYPED_TEST(vector_floating_test, Normalization)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{3}, T{4}, T{0}};
  auto normalized{v.normalized()};

  EXPECT_NEAR(normalized.length(), T{1}, TestFixture::kTolerance);
  EXPECT_NEAR(normalized.x(), T{3} / T{5}, TestFixture::kTolerance);
  EXPECT_NEAR(normalized.y(), T{4} / T{5}, TestFixture::kTolerance);
  EXPECT_NEAR(normalized.z(), T{0}, TestFixture::kTolerance);

  vec3 v2{T{3}, T{4}, T{0}};
  v2.normalize();
  EXPECT_NEAR(v2.length(), T{1}, TestFixture::kTolerance);
}

TYPED_TEST(vector_floating_test, ZeroVectorNormalization)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;

  vec3 zero{T{0}, T{0}, T{0}};
  auto normalized{zero.normalized()};

  EXPECT_EQ(normalized.x(), T{0});
  EXPECT_EQ(normalized.y(), T{0});
  EXPECT_EQ(normalized.z(), T{0});

  zero.normalize();
  EXPECT_EQ(zero.x(), T{0});
  EXPECT_EQ(zero.y(), T{0});
  EXPECT_EQ(zero.z(), T{0});
}

TYPED_TEST(vector_floating_test, DotProduct)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{1}, T{2}, T{3}};
  vec3 v2{T{4}, T{5}, T{6}};

  auto result{dot(v1, v2)};
  EXPECT_NEAR(result, T{32}, TestFixture::kTolerance);

  vec3 unit_x{T{1}, T{0}, T{0}};
  vec3 unit_y{T{0}, T{1}, T{0}};
  EXPECT_NEAR(dot(unit_x, unit_y), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(dot(unit_x, unit_x), T{1}, TestFixture::kTolerance);
}

TYPED_TEST(vector_floating_test, CrossProduct)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;

  vec3 unit_x{T{1}, T{0}, T{0}};
  vec3 unit_y{T{0}, T{1}, T{0}};
  vec3 unit_z{T{0}, T{0}, T{1}};

  auto result{cross(unit_x, unit_y)};
  EXPECT_NEAR(result.x(), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(result.y(), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(result.z(), T{1}, TestFixture::kTolerance);

  auto result2{cross(unit_y, unit_z)};
  EXPECT_NEAR(result2.x(), T{1}, TestFixture::kTolerance);
  EXPECT_NEAR(result2.y(), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(result2.z(), T{0}, TestFixture::kTolerance);

  vec3 v1{T{1}, T{2}, T{3}};
  vec3 v2{T{4}, T{5}, T{6}};
  auto cross_result{cross(v1, v2)};
  EXPECT_NEAR(cross_result.x(), T{-3}, TestFixture::kTolerance); // 2*6 - 3*5
  EXPECT_NEAR(cross_result.y(), T{6}, TestFixture::kTolerance);  // 3*4 - 1*6
  EXPECT_NEAR(cross_result.z(), T{-3}, TestFixture::kTolerance); // 1*5 - 2*4
}

TYPED_TEST(vector_floating_test, DistanceFunctions)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;

  vec3 v1{T{0}, T{0}, T{0}};
  vec3 v2{T{3}, T{4}, T{0}};

  auto dist_sq{distance_squared(v1, v2)};
  auto dist{distance(v1, v2)};

  EXPECT_NEAR(dist_sq, T{25}, TestFixture::kTolerance);
  EXPECT_NEAR(dist, T{5}, TestFixture::kTolerance);

  EXPECT_NEAR(distance(v1, v1), T{0}, TestFixture::kTolerance);
}

TYPED_TEST(vector_floating_test, ReflectionFunction)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;

  vec3 incident{T{1}, T{-1}, T{0}};
  vec3 normal{T{0}, T{1}, T{0}};

  auto reflected{reflect(incident, normal)};

  EXPECT_NEAR(reflected.x(), T{1}, TestFixture::kTolerance);
  EXPECT_NEAR(reflected.y(), T{1}, TestFixture::kTolerance);
  EXPECT_NEAR(reflected.z(), T{0}, TestFixture::kTolerance);
}

TYPED_TEST(vector_floating_test, RefractionFunction)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;

  vec3 incident{T{1}, T{-1}, T{0}};
  incident = incident.normalized();
  vec3 normal{T{0}, T{1}, T{0}};
  T eta{T{0.5}};

  auto refracted{refract(incident, normal, eta)};

  EXPECT_NE(refracted.x(), T{0});
  EXPECT_LT(refracted.y(), T{0});
  EXPECT_NEAR(refracted.z(), T{0}, TestFixture::kTolerance);
}

TYPED_TEST(vector_floating_test, LerpFunction)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;

  vec3 start{T{0}, T{0}, T{0}};
  vec3 end{T{10}, T{20}, T{30}};

  auto mid{lerp(start, end, T{0.5})};
  EXPECT_NEAR(mid.x(), T{5}, TestFixture::kTolerance);
  EXPECT_NEAR(mid.y(), T{10}, TestFixture::kTolerance);
  EXPECT_NEAR(mid.z(), T{15}, TestFixture::kTolerance);

  auto at_start{lerp(start, end, T{0})};
  EXPECT_EQ(at_start.x(), start.x());
  EXPECT_EQ(at_start.y(), start.y());
  EXPECT_EQ(at_start.z(), start.z());

  auto at_end{lerp(start, end, T{1})};
  EXPECT_EQ(at_end.x(), end.x());
  EXPECT_EQ(at_end.y(), end.y());
  EXPECT_EQ(at_end.z(), end.z());
}

TYPED_TEST(vector_scalar_test, ComponentWiseMinMax)
{
  using T = typename TestFixture::scalar_type;
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

TYPED_TEST(vector_simd_test, SimdDivision)
{
  using T = typename TestFixture::float_type;
  constexpr usize simd_size{TestFixture::get_simd_size()};
  using vec_simd = vector<T, simd_size>;

  if constexpr (std::same_as<T, f32>)
  {
    vec_simd v{T{8}, T{12}, T{16}, T{20}};
    v /= T{4};

    EXPECT_NEAR(v.x(), T{2}, TestFixture::kTolerance);
    EXPECT_NEAR(v.y(), T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(v.z(), T{4}, TestFixture::kTolerance);
    EXPECT_NEAR(v.w(), T{5}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    vec_simd v{T{8}, T{12}};
    v /= T{4};

    EXPECT_NEAR(v.x(), T{2}, TestFixture::kTolerance);
    EXPECT_NEAR(v.y(), T{3}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdComparison)
{
  using T = typename TestFixture::float_type;
  constexpr usize simd_size{TestFixture::get_simd_size()};
  using vec_simd = vector<T, simd_size>;

  if constexpr (std::same_as<T, f32>)
  {
    vec_simd v1{T{1}, T{2}, T{3}, T{4}};
    vec_simd v2{T{1}, T{2}, T{3}, T{4}};
    vec_simd v3{T{1}, T{2}, T{3}, T{5}};

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_FALSE(v1 != v2);
    EXPECT_TRUE(v1 != v3);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    vec_simd v1{T{1}, T{2}};
    vec_simd v2{T{1}, T{2}};
    vec_simd v3{T{1}, T{3}};

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
  }
}

TYPED_TEST(vector_simd_test, SimdDotProduct)
{
  using T = typename TestFixture::float_type;
  constexpr usize simd_size{TestFixture::get_simd_size()};
  using vec_simd = vector<T, simd_size>;

  if constexpr (std::same_as<T, f32>)
  {
    vec_simd v1{T{1}, T{2}, T{3}, T{4}};
    vec_simd v2{T{5}, T{6}, T{7}, T{8}};

    auto result{dot(v1, v2)};
    EXPECT_NEAR(result, T{70}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    vec_simd v1{T{1}, T{2}};
    vec_simd v2{T{3}, T{4}};

    auto result{dot(v1, v2)};
    EXPECT_NEAR(result, T{11}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdNegation)
{
  using T = typename TestFixture::float_type;
  constexpr usize simd_size{TestFixture::get_simd_size()};
  using vec_simd = vector<T, simd_size>;

  if constexpr (std::same_as<T, f32>)
  {
    vec_simd v{T{1}, T{-2}, T{3}, T{-4}};
    auto negated{-v};

    EXPECT_NEAR(negated.x(), T{-1}, TestFixture::kTolerance);
    EXPECT_NEAR(negated.y(), T{2}, TestFixture::kTolerance);
    EXPECT_NEAR(negated.z(), T{-3}, TestFixture::kTolerance);
    EXPECT_NEAR(negated.w(), T{4}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    vec_simd v{T{1}, T{-2}};
    auto negated{-v};

    EXPECT_NEAR(negated.x(), T{-1}, TestFixture::kTolerance);
    EXPECT_NEAR(negated.y(), T{2}, TestFixture::kTolerance);
  }
}

// Edge case tests
TYPED_TEST(vector_floating_test, EdgeCases)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;

  vec3 tiny{T{1e-10}, T{1e-10}, T{1e-10}};
  auto tiny_length{tiny.length()};
  EXPECT_GT(tiny_length, T{0});

  vec3 orthogonal1{T{1}, T{0}, T{0}};
  vec3 orthogonal2{T{0}, T{1}, T{0}};
  EXPECT_NEAR(dot(orthogonal1, orthogonal2), T{0}, TestFixture::kTolerance);

  vec3 a{T{1}, T{2}, T{3}};
  vec3 b{T{4}, T{5}, T{6}};
  auto cross_ab{cross(a, b)};
  auto cross_ba{cross(b, a)};

  EXPECT_NEAR(cross_ab.x(), -cross_ba.x(), TestFixture::kTolerance);
  EXPECT_NEAR(cross_ab.y(), -cross_ba.y(), TestFixture::kTolerance);
  EXPECT_NEAR(cross_ab.z(), -cross_ba.z(), TestFixture::kTolerance);

  EXPECT_NEAR(dot(cross_ab, a), T{0}, TestFixture::kTolerance);
  EXPECT_NEAR(dot(cross_ab, b), T{0}, TestFixture::kTolerance);
}

TYPED_TEST(vector_simd_test, LargeVectorOperations)
{
  using T = typename TestFixture::float_type;
  using vec16 = vector<T, 16>;

  vec16 v1{T{1}};
  vec16 v2{T{2}};

  auto sum{v1 + v2};
  auto diff{v2 - v1};
  auto prod{v1 * v2};

  for (usize i{}; i < 16; ++i)
  {
    EXPECT_EQ(sum[i], T{3});
    EXPECT_EQ(diff[i], T{1});
    EXPECT_EQ(prod[i], T{2});
  }

  auto dot_result{dot(v1, v2)};
  EXPECT_NEAR(dot_result, T{32}, TestFixture::kTolerance);
}

TYPED_TEST(vector_floating_test, MixedTypeOperations)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{1.5}, T{2.5}, T{3.5}};

  auto result{v * T{2}};
  EXPECT_NEAR(result.x(), T{3}, TestFixture::kTolerance);
  EXPECT_NEAR(result.y(), T{5}, TestFixture::kTolerance);
  EXPECT_NEAR(result.z(), T{7}, TestFixture::kTolerance);
}

TYPED_TEST(vector_floating_test, FastLengthAndNormalize)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;
  constexpr T approx_tolerance{1e-3};

  vec3 v{T{3}, T{4}, T{0}};

  auto fast_len = v.fast_length();
  EXPECT_NEAR(fast_len, T{5}, approx_tolerance * T{10});

  vec3 v2{T{3}, T{4}, T{0}};
  v2.fast_normalize();
  EXPECT_NEAR(v2.length(), T{1}, approx_tolerance * T{10});

  auto fast_norm = v.fast_normalized();
  EXPECT_NEAR(fast_norm.length(), T{1}, approx_tolerance * T{10});
  EXPECT_NEAR(fast_norm.x(), T{3} / T{5}, approx_tolerance * T{10});
  EXPECT_NEAR(fast_norm.y(), T{4} / T{5}, approx_tolerance * T{10});
}

TYPED_TEST(vector_scalar_test, IteratorSupport)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;

  vec3 v{T{1}, T{2}, T{3}};

  T sum{0};
  for (auto value : v)
  {
    sum += value;
  }
  EXPECT_EQ(sum, T{6});

  auto it = v.begin();
  EXPECT_EQ(*it, T{1});
  ++it;
  EXPECT_EQ(*it, T{2});
  ++it;
  EXPECT_EQ(*it, T{3});
  EXPECT_EQ(++it, v.end());
}

TYPED_TEST(vector_scalar_test, ConstexprEvaluation)
{
  using T = typename TestFixture::scalar_type;
  using vec3 = vector<T, 3>;

  constexpr vec3 v1{T{1}, T{2}, T{3}};
  constexpr vec3 v2{T{4}, T{5}, T{6}};
  constexpr vec3 sum = v1 + v2;

  EXPECT_EQ(sum.x(), T{5});
  EXPECT_EQ(sum.y(), T{7});
  EXPECT_EQ(sum.z(), T{9});

  if constexpr (std::floating_point<T>)
  {
    constexpr auto len_sq = v1.length_squared();
    EXPECT_EQ(len_sq, T{14});
  }
}

TYPED_TEST(vector_floating_test, RefractionBoundaryCases)
{
  using T = typename TestFixture::float_type;
  using vec3 = vector<T, 3>;

  vec3 incident{T{0.1}, T{-0.99}, T{0}};
  incident.normalize();
  vec3 normal{T{0}, T{1}, T{0}};

  T eta_refract{T{0.8}};
  auto refracted = refract(incident, normal, eta_refract);
  EXPECT_NE(refracted.length_squared(), T{0});

  T eta_reflect{T{2.0}};
  auto reflected = refract(incident, normal, eta_reflect);

  EXPECT_LT(dot(reflected, normal), T{0});
  EXPECT_GT(reflected.y() * incident.y(), T{0});
}

TYPED_TEST(vector_simd_test, SimdEdgeCases)
{
  using T = typename TestFixture::float_type;
  constexpr usize simd_size = TestFixture::get_simd_size();
  using vec_simd = vector<T, simd_size>;

  T small_val = kEpsilonV<T> * T{2};
  vec_simd tiny{small_val};

  EXPECT_GT(tiny.length(), T{0});

  auto normalized = tiny.normalized();
  if (small_val > kEpsilonV<T>)
  {
    EXPECT_NEAR(normalized.length(), T{1}, TestFixture::kTolerance * T{10});
  }
  else
  {
    EXPECT_EQ(normalized.length(), T{0});
  }
}
} // namespace pbrt::math::test
