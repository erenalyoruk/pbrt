#include <gtest/gtest.h>
#include <pbrt/math/vector.hpp>
#include <pbrt/simd.hpp>

#include <array>

namespace pbrt::math::test
{
using simd_types = ::testing::Types<f32, f64>;

template <typename T>
class vector_simd_test : public ::testing::Test
{
public:
  using value_type = T;
  static constexpr T kTolerance = std::is_same_v<T, f32> ? T{1e-6F} : T{1e-15};
  static constexpr T kFastTolerance = std::is_same_v<T, f32> ? T{1e-2F} : T{1e-10};

  static constexpr auto sse_size() noexcept -> usize
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

TYPED_TEST_SUITE(vector_simd_test, simd_types);

// ============================================================================
// SIMD Compatibility Tests
// ============================================================================

TYPED_TEST(vector_simd_test, SimdCompatibilityFlags)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};
  constexpr auto avx_size{TestFixture::avx_size()};

  using sse_vec = vector<T, sse_size>;
  using avx_vec = vector<T, avx_size>;
  using non_simd_vec = vector<T, 3>;

  EXPECT_TRUE(sse_vec::kIsSimdCompatible);
  EXPECT_TRUE(avx_vec::kIsSimdCompatible);
  EXPECT_FALSE(non_simd_vec::kIsSimdCompatible);

  // Static assertions for compile-time verification
  static_assert(sse_vec::kIsSimdCompatible);
  static_assert(avx_vec::kIsSimdCompatible);
  static_assert(!non_simd_vec::kIsSimdCompatible);
}

TYPED_TEST(vector_simd_test, SimdAlignment)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};
  constexpr auto avx_size{TestFixture::avx_size()};

  using sse_vec = vector<T, sse_size>;
  using avx_vec = vector<T, avx_size>;

  sse_vec sse_v{};
  avx_vec avx_v{};

  auto sse_ptr{std::bit_cast<uintptr>(&sse_v)};
  auto avx_ptr{std::bit_cast<uintptr>(&avx_v)};

  EXPECT_EQ(sse_ptr % 16, 0) << "SSE vector not aligned to 16 bytes";
  EXPECT_EQ(avx_ptr % 32, 0) << "AVX vector not aligned to 32 bytes";

  // Test alignment for stack-allocated arrays
  alignas(32) std::array<sse_vec, 10> sse_array{};
  alignas(32) std::array<avx_vec, 10> avx_array{};

  for (usize i{}; i < 10; ++i)
  {
    auto sse_elem_ptr{std::bit_cast<uintptr>(&sse_array[i])};
    auto avx_elem_ptr{std::bit_cast<uintptr>(&avx_array[i])};

    EXPECT_EQ(sse_elem_ptr % 16, 0) << "SSE array element " << i << " not aligned";
    EXPECT_EQ(avx_elem_ptr % 32, 0) << "AVX array element " << i << " not aligned";
  }
}

// ============================================================================
// SIMD Construction Tests
// ============================================================================

TYPED_TEST(vector_simd_test, SimdDefaultConstruction)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  sse_vec v{};

  for (usize i{}; i < sse_size; ++i)
  {
    EXPECT_EQ(v[i], T{0});
  }

  // Test SIMD data access
  auto simd_data{v.simd()};
  alignas(16) std::array<T, sse_size> extracted{};

  if constexpr (std::same_as<T, f32>)
  {
    simde_mm_store_ps(extracted.data(), simd_data);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    simde_mm_store_pd(extracted.data(), simd_data);
  }

  for (usize i{}; i < sse_size; ++i)
  {
    EXPECT_EQ(extracted[i], T{0});
  }
}

TYPED_TEST(vector_simd_test, SimdScalarConstruction)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  constexpr T test_value{T{42.5}};
  sse_vec v{test_value};

  for (usize i{}; i < sse_size; ++i)
  {
    EXPECT_EQ(v[i], test_value);
  }

  // Verify SIMD data consistency
  auto simd_data{v.simd()};
  alignas(16) std::array<T, sse_size> extracted{};

  if constexpr (std::same_as<T, f32>)
  {
    simde_mm_store_ps(extracted.data(), simd_data);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    simde_mm_store_pd(extracted.data(), simd_data);
  }

  for (usize i{}; i < sse_size; ++i)
  {
    EXPECT_NEAR(extracted[i], test_value, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdComponentConstruction)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  if constexpr (std::same_as<T, f32>)
  {
    sse_vec v{T{1}, T{2}, T{3}, T{4}};

    EXPECT_EQ(v.x(), T{1});
    EXPECT_EQ(v.y(), T{2});
    EXPECT_EQ(v.z(), T{3});
    EXPECT_EQ(v.w(), T{4});

    // Verify SIMD data
    auto simd_data{v.simd()};
    alignas(16) std::array<T, 4> extracted{};
    simde_mm_store_ps(extracted.data(), simd_data);

    EXPECT_NEAR(extracted[0], T{1}, TestFixture::kTolerance);
    EXPECT_NEAR(extracted[1], T{2}, TestFixture::kTolerance);
    EXPECT_NEAR(extracted[2], T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(extracted[3], T{4}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    sse_vec v{T{1}, T{2}};

    EXPECT_EQ(v.x(), T{1});
    EXPECT_EQ(v.y(), T{2});

    // Verify SIMD data
    auto simd_data{v.simd()};
    alignas(16) std::array<T, 2> extracted{};
    simde_mm_store_pd(extracted.data(), simd_data);

    EXPECT_NEAR(extracted[0], T{1}, TestFixture::kTolerance);
    EXPECT_NEAR(extracted[1], T{2}, TestFixture::kTolerance);
  }
}

// ============================================================================
// SIMD Arithmetic Tests
// ============================================================================

TYPED_TEST(vector_simd_test, SimdAddition)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  if constexpr (std::same_as<T, f32>)
  {
    sse_vec v1{T{1}, T{2}, T{3}, T{4}};
    sse_vec v2{T{5}, T{6}, T{7}, T{8}};

    auto result{v1 + v2};

    EXPECT_NEAR(result.x(), T{6}, TestFixture::kTolerance);
    EXPECT_NEAR(result.y(), T{8}, TestFixture::kTolerance);
    EXPECT_NEAR(result.z(), T{10}, TestFixture::kTolerance);
    EXPECT_NEAR(result.w(), T{12}, TestFixture::kTolerance);

    // Test in-place addition
    v1 += v2;
    EXPECT_NEAR(v1.x(), T{6}, TestFixture::kTolerance);
    EXPECT_NEAR(v1.y(), T{8}, TestFixture::kTolerance);
    EXPECT_NEAR(v1.z(), T{10}, TestFixture::kTolerance);
    EXPECT_NEAR(v1.w(), T{12}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    sse_vec v1{T{1}, T{2}};
    sse_vec v2{T{3}, T{4}};

    auto result{v1 + v2};

    EXPECT_NEAR(result.x(), T{4}, TestFixture::kTolerance);
    EXPECT_NEAR(result.y(), T{6}, TestFixture::kTolerance);

    // Test in-place addition
    v1 += v2;
    EXPECT_NEAR(v1.x(), T{4}, TestFixture::kTolerance);
    EXPECT_NEAR(v1.y(), T{6}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdSubtraction)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  if constexpr (std::same_as<T, f32>)
  {
    sse_vec v1{T{10}, T{8}, T{6}, T{4}};
    sse_vec v2{T{1}, T{2}, T{3}, T{4}};

    auto result{v1 - v2};

    EXPECT_NEAR(result.x(), T{9}, TestFixture::kTolerance);
    EXPECT_NEAR(result.y(), T{6}, TestFixture::kTolerance);
    EXPECT_NEAR(result.z(), T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(result.w(), T{0}, TestFixture::kTolerance);

    // Test in-place subtraction
    v1 -= v2;
    EXPECT_NEAR(v1.x(), T{9}, TestFixture::kTolerance);
    EXPECT_NEAR(v1.y(), T{6}, TestFixture::kTolerance);
    EXPECT_NEAR(v1.z(), T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(v1.w(), T{0}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    sse_vec v1{T{10}, T{8}};
    sse_vec v2{T{1}, T{2}};

    auto result{v1 - v2};

    EXPECT_NEAR(result.x(), T{9}, TestFixture::kTolerance);
    EXPECT_NEAR(result.y(), T{6}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdScalarMultiplication)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  if constexpr (std::same_as<T, f32>)
  {
    sse_vec v{T{1}, T{2}, T{3}, T{4}};
    T scalar{T{3}};

    auto result{v * scalar};

    EXPECT_NEAR(result.x(), T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(result.y(), T{6}, TestFixture::kTolerance);
    EXPECT_NEAR(result.z(), T{9}, TestFixture::kTolerance);
    EXPECT_NEAR(result.w(), T{12}, TestFixture::kTolerance);

    // Test commutative property
    auto result2{scalar * v};
    EXPECT_TRUE(result == result2);

    // Test in-place multiplication
    v *= scalar;
    EXPECT_NEAR(v.x(), T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(v.y(), T{6}, TestFixture::kTolerance);
    EXPECT_NEAR(v.z(), T{9}, TestFixture::kTolerance);
    EXPECT_NEAR(v.w(), T{12}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    sse_vec v{T{1}, T{2}};
    T scalar{T{3}};

    auto result{v * scalar};

    EXPECT_NEAR(result.x(), T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(result.y(), T{6}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdVectorMultiplication)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  if constexpr (std::same_as<T, f32>)
  {
    sse_vec v1{T{2}, T{3}, T{4}, T{5}};
    sse_vec v2{T{6}, T{7}, T{8}, T{9}};

    auto result{v1 * v2};

    EXPECT_NEAR(result.x(), T{12}, TestFixture::kTolerance);
    EXPECT_NEAR(result.y(), T{21}, TestFixture::kTolerance);
    EXPECT_NEAR(result.z(), T{32}, TestFixture::kTolerance);
    EXPECT_NEAR(result.w(), T{45}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    sse_vec v1{T{2}, T{3}};
    sse_vec v2{T{4}, T{5}};

    auto result{v1 * v2};

    EXPECT_NEAR(result.x(), T{8}, TestFixture::kTolerance);
    EXPECT_NEAR(result.y(), T{15}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdDivision)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  if constexpr (std::same_as<T, f32>)
  {
    sse_vec v{T{8}, T{12}, T{16}, T{20}};
    T divisor{T{4}};

    auto result{v / divisor};

    EXPECT_NEAR(result.x(), T{2}, TestFixture::kTolerance);
    EXPECT_NEAR(result.y(), T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(result.z(), T{4}, TestFixture::kTolerance);
    EXPECT_NEAR(result.w(), T{5}, TestFixture::kTolerance);

    // Test in-place division
    v /= divisor;
    EXPECT_NEAR(v.x(), T{2}, TestFixture::kTolerance);
    EXPECT_NEAR(v.y(), T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(v.z(), T{4}, TestFixture::kTolerance);
    EXPECT_NEAR(v.w(), T{5}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    sse_vec v{T{8}, T{12}};
    T divisor{T{4}};

    auto result{v / divisor};

    EXPECT_NEAR(result.x(), T{2}, TestFixture::kTolerance);
    EXPECT_NEAR(result.y(), T{3}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdNegation)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  if constexpr (std::same_as<T, f32>)
  {
    sse_vec v{T{1}, T{-2}, T{3}, T{-4}};
    auto negated{-v};

    EXPECT_NEAR(negated.x(), T{-1}, TestFixture::kTolerance);
    EXPECT_NEAR(negated.y(), T{2}, TestFixture::kTolerance);
    EXPECT_NEAR(negated.z(), T{-3}, TestFixture::kTolerance);
    EXPECT_NEAR(negated.w(), T{4}, TestFixture::kTolerance);

    // Original vector should be unchanged
    EXPECT_NEAR(v.x(), T{1}, TestFixture::kTolerance);
    EXPECT_NEAR(v.y(), T{-2}, TestFixture::kTolerance);
    EXPECT_NEAR(v.z(), T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(v.w(), T{-4}, TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    sse_vec v{T{1}, T{-2}};
    auto negated{-v};

    EXPECT_NEAR(negated.x(), T{-1}, TestFixture::kTolerance);
    EXPECT_NEAR(negated.y(), T{2}, TestFixture::kTolerance);
  }
}

// ============================================================================
// SIMD Comparison Tests
// ============================================================================

TYPED_TEST(vector_simd_test, SimdComparison)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  if constexpr (std::same_as<T, f32>)
  {
    sse_vec v1{T{1}, T{2}, T{3}, T{4}};
    sse_vec v2{T{1}, T{2}, T{3}, T{4}};
    sse_vec v3{T{1}, T{2}, T{3}, T{5}};

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_FALSE(v1 != v2);
    EXPECT_TRUE(v1 != v3);

    // Test with floating-point precision
    sse_vec v4{T{1.0000001}, T{2}, T{3}, T{4}};
    EXPECT_FALSE(v1 == v4); // Should be exact comparison
  }
  else if constexpr (std::same_as<T, f64>)
  {
    sse_vec v1{T{1}, T{2}};
    sse_vec v2{T{1}, T{2}};
    sse_vec v3{T{1}, T{3}};

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_FALSE(v1 != v2);
    EXPECT_TRUE(v1 != v3);
  }
}

// ============================================================================
// SIMD Algorithm Tests
// ============================================================================

TYPED_TEST(vector_simd_test, SimdDotProduct)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  if constexpr (std::same_as<T, f32>)
  {
    sse_vec v1{T{1}, T{2}, T{3}, T{4}};
    sse_vec v2{T{5}, T{6}, T{7}, T{8}};

    auto result{dot(v1, v2)};
    // 1*5 + 2*6 + 3*7 + 4*8 = 5 + 12 + 21 + 32 = 70
    EXPECT_NEAR(result, T{70}, TestFixture::kTolerance);

    // Test orthogonal vectors
    sse_vec unit_x{T{1}, T{0}, T{0}, T{0}};
    sse_vec unit_y{T{0}, T{1}, T{0}, T{0}};
    EXPECT_NEAR(dot(unit_x, unit_y), T{0}, TestFixture::kTolerance);

    // Test self dot product
    EXPECT_NEAR(dot(v1, v1), v1.length_squared(), TestFixture::kTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    sse_vec v1{T{1}, T{2}};
    sse_vec v2{T{3}, T{4}};

    auto result{dot(v1, v2)};
    // 1*3 + 2*4 = 3 + 8 = 11
    EXPECT_NEAR(result, T{11}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdLengthFunctions)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  if constexpr (std::same_as<T, f32>)
  {
    sse_vec v{T{1}, T{2}, T{2}, T{0}}; // length = sqrt(1 + 4 + 4 + 0) = 3

    auto length_sq{v.length_squared()};
    auto length{v.length()};
    auto fast_length{v.fast_length()};

    EXPECT_NEAR(length_sq, T{9}, TestFixture::kTolerance);
    EXPECT_NEAR(length, T{3}, TestFixture::kTolerance);
    EXPECT_NEAR(fast_length, T{3}, TestFixture::kFastTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    sse_vec v{T{3}, T{4}}; // length = sqrt(9 + 16) = 5

    auto length_sq{v.length_squared()};
    auto length{v.length()};

    EXPECT_NEAR(length_sq, T{25}, TestFixture::kTolerance);
    EXPECT_NEAR(length, T{5}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdNormalization)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  if constexpr (std::same_as<T, f32>)
  {
    sse_vec v{T{3}, T{4}, T{0}, T{0}};
    auto normalized{v.normalized()};

    EXPECT_NEAR(normalized.length(), T{1}, TestFixture::kTolerance);
    EXPECT_NEAR(normalized.x(), T{3} / T{5}, TestFixture::kTolerance);
    EXPECT_NEAR(normalized.y(), T{4} / T{5}, TestFixture::kTolerance);
    EXPECT_NEAR(normalized.z(), T{0}, TestFixture::kTolerance);
    EXPECT_NEAR(normalized.w(), T{0}, TestFixture::kTolerance);

    // Test in-place normalization
    sse_vec v2{v};
    v2.normalize();
    EXPECT_NEAR(v2.length(), T{1}, TestFixture::kTolerance);

    // Test fast normalization
    auto fast_normalized{v.fast_normalized()};
    EXPECT_NEAR(fast_normalized.length(), T{1}, TestFixture::kFastTolerance);
  }
  else if constexpr (std::same_as<T, f64>)
  {
    sse_vec v{T{3}, T{4}};
    auto normalized{v.normalized()};

    EXPECT_NEAR(normalized.length(), T{1}, TestFixture::kTolerance);
    EXPECT_NEAR(normalized.x(), T{3} / T{5}, TestFixture::kTolerance);
    EXPECT_NEAR(normalized.y(), T{4} / T{5}, TestFixture::kTolerance);
  }
}

// ============================================================================
// AVX Tests (for supported types)
// ============================================================================

TYPED_TEST(vector_simd_test, AvxOperations)
{
  using T = typename TestFixture::value_type;
  constexpr auto avx_size{TestFixture::avx_size()};

  using avx_vec = vector<T, avx_size>;

  if constexpr (std::same_as<T, f32>)
  {
    avx_vec v1{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}};
    avx_vec v2{T{8}, T{7}, T{6}, T{5}, T{4}, T{3}, T{2}, T{1}};

    // Test addition
    auto sum{v1 + v2};
    for (usize i{}; i < avx_size; ++i)
    {
      EXPECT_NEAR(sum[i], T{9}, TestFixture::kTolerance);
    }

    // Test dot product
    auto dot_result{dot(v1, v2)};
    // 1*8 + 2*7 + 3*6 + 4*5 + 5*4 + 6*3 + 7*2 + 8*1 = 8+14+18+20+20+18+14+8 = 120
    EXPECT_NEAR(dot_result, T{120}, TestFixture::kTolerance);

    // Test scalar multiplication
    auto scaled{v1 * T{2}};
    for (usize i{}; i < avx_size; ++i)
    {
      EXPECT_NEAR(scaled[i], T{2} * v1[i], TestFixture::kTolerance);
    }
  }
  else if constexpr (std::same_as<T, f64>)
  {
    avx_vec v1{T{1}, T{2}, T{3}, T{4}};
    avx_vec v2{T{4}, T{3}, T{2}, T{1}};

    // Test addition
    auto sum{v1 + v2};
    for (usize i{}; i < avx_size; ++i)
    {
      EXPECT_NEAR(sum[i], T{5}, TestFixture::kTolerance);
    }

    // Test dot product
    auto dot_result{dot(v1, v2)};
    // 1*4 + 2*3 + 3*2 + 4*1 = 4 + 6 + 6 + 4 = 20
    EXPECT_NEAR(dot_result, T{20}, TestFixture::kTolerance);
  }
}

// ============================================================================
// Performance and Edge Case Tests
// ============================================================================

TYPED_TEST(vector_simd_test, SimdEdgeCases)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  // Test with very small values
  T small_val{kEpsilonV<T> * T{2}};
  sse_vec tiny{small_val};

  EXPECT_GT(tiny.length(), T{0});

  auto normalized{tiny.normalized()};
  if (small_val > kEpsilonV<T>)
  {
    EXPECT_NEAR(normalized.length(), T{1}, TestFixture::kTolerance * T{10});
  }

  // Test with large values
  T large_val{T{1e6}};
  sse_vec large{large_val};

  auto large_normalized{large.normalized()};
  EXPECT_NEAR(large_normalized.length(), T{1}, TestFixture::kTolerance);

  // Test with mixed signs
  if constexpr (std::same_as<T, f32>)
  {
    sse_vec mixed{T{1}, T{-1}, T{1}, T{-1}};
    auto mixed_length{mixed.length()};
    EXPECT_NEAR(mixed_length, T{2}, TestFixture::kTolerance);
  }
}

TYPED_TEST(vector_simd_test, SimdConstexprOperations)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  // Test constexpr construction and basic operations
  if constexpr (std::same_as<T, f32>)
  {
    constexpr sse_vec v1{T{1}, T{2}, T{3}, T{4}};
    constexpr sse_vec v2{T{5}, T{6}, T{7}, T{8}};
    constexpr auto sum{v1 + v2};

    EXPECT_EQ(sum.x(), T{6});
    EXPECT_EQ(sum.y(), T{8});
    EXPECT_EQ(sum.z(), T{10});
    EXPECT_EQ(sum.w(), T{12});

    constexpr auto length_sq{v1.length_squared()};
    EXPECT_EQ(length_sq, T{30}); // 1 + 4 + 9 + 16
  }
  else if constexpr (std::same_as<T, f64>)
  {
    constexpr sse_vec v1{T{1}, T{2}};
    constexpr sse_vec v2{T{3}, T{4}};
    constexpr auto sum{v1 + v2};

    EXPECT_EQ(sum.x(), T{4});
    EXPECT_EQ(sum.y(), T{6});

    constexpr auto length_sq{v1.length_squared()};
    EXPECT_EQ(length_sq, T{5}); // 1 + 4
  }
}

TYPED_TEST(vector_simd_test, SimdMemoryLayout)
{
  using T = typename TestFixture::value_type;
  constexpr auto sse_size{TestFixture::sse_size()};

  using sse_vec = vector<T, sse_size>;

  sse_vec v{};

  // Verify that scalar and SIMD data views are consistent
  for (usize i{}; i < sse_size; ++i)
  {
    v[i] = static_cast<T>(i + 1);
  }

  // Extract SIMD data and verify it matches scalar data
  auto simd_data{v.simd()};
  alignas(32) std::array<T, sse_size> extracted{};

  if constexpr (std::same_as<T, f32> && sse_size == 4)
  {
    simde_mm_store_ps(extracted.data(), simd_data);
  }
  else if constexpr (std::same_as<T, f64> && sse_size == 2)
  {
    simde_mm_store_pd(extracted.data(), simd_data);
  }

  for (usize i{}; i < sse_size; ++i)
  {
    EXPECT_EQ(extracted[i], static_cast<T>(i + 1));
    EXPECT_EQ(v[i], extracted[i]);
  }
}
} // namespace pbrt::math::test
