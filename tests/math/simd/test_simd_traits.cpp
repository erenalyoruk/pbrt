#include <gtest/gtest.h>
#include <pbrt/math/simd/simd_traits.hpp>
#include <pbrt/simd.hpp>

namespace pbrt::math::simd::test
{
class simd_traits_test : public ::testing::Test
{
};

TEST_F(simd_traits_test, BasicTraits)
{
  EXPECT_FALSE((simd_traits<f32, 3>::kIsSimdCompatible));
  EXPECT_FALSE((simd_traits<i32, 4>::kIsSimdCompatible));
  EXPECT_FALSE((simd_traits<f32, 5>::kIsSimdCompatible));

  EXPECT_EQ((simd_traits<f32, 3>::kAlignment), alignof(f32));
  EXPECT_EQ((simd_traits<i32, 4>::kAlignment), alignof(i32));
}

TEST_F(simd_traits_test, Float4SimdTraits)
{
  using traits = simd_traits<f32, 4>;

  EXPECT_TRUE(traits::kIsSimdCompatible);
  EXPECT_EQ(traits::kAlignment, 16);
  EXPECT_TRUE((std::same_as<traits::simd_type, __m128>));

  typename traits::storage_type storage{};
  storage.scalar_data[0] = 1.0F;
  storage.scalar_data[1] = 2.0F;
  storage.scalar_data[2] = 3.0F;
  storage.scalar_data[3] = 4.0F;

  alignas(16) std::array<f32, 4> test_data{};
  simde_mm_store_ps(test_data.data(), storage.simd_data);

  EXPECT_FLOAT_EQ(test_data[0], 1.0F);
  EXPECT_FLOAT_EQ(test_data[1], 2.0F);
  EXPECT_FLOAT_EQ(test_data[2], 3.0F);
  EXPECT_FLOAT_EQ(test_data[3], 4.0F);
}

TEST_F(simd_traits_test, Float8SimdTraits)
{
  using traits = simd_traits<f32, 8>;

  EXPECT_TRUE(traits::kIsSimdCompatible);
  EXPECT_EQ(traits::kAlignment, 32);
  EXPECT_TRUE((std::same_as<traits::simd_type, __m256>));
}

TEST_F(simd_traits_test, Double2SimdTraits)
{
  using traits = simd_traits<f64, 2>;

  EXPECT_TRUE(traits::kIsSimdCompatible);
  EXPECT_EQ(traits::kAlignment, 16);
  EXPECT_TRUE((std::same_as<traits::simd_type, __m128d>));
}

TEST_F(simd_traits_test, Double4SimdTraits)
{
  using traits = simd_traits<f64, 4>;

  EXPECT_TRUE(traits::kIsSimdCompatible);
  EXPECT_EQ(traits::kAlignment, 32);
  EXPECT_TRUE((std::same_as<traits::simd_type, __m256d>));
}

TEST_F(simd_traits_test, Concepts)
{
  EXPECT_TRUE((simd_compatible<f32, 4>));
  EXPECT_TRUE((simd_compatible<f32, 8>));
  EXPECT_TRUE((simd_compatible<f64, 2>));
  EXPECT_TRUE((simd_compatible<f64, 4>));

  EXPECT_FALSE((simd_compatible<f32, 3>));
  EXPECT_FALSE((simd_compatible<i32, 4>));

  EXPECT_TRUE((sse_compatible<f32, 4>));
  EXPECT_TRUE((sse_compatible<f64, 2>));
  EXPECT_FALSE((sse_compatible<f32, 8>));

  EXPECT_TRUE((avx_compatible<f32, 8>));
  EXPECT_TRUE((avx_compatible<f64, 4>));
  EXPECT_FALSE((avx_compatible<f32, 4>));
}

TEST_F(simd_traits_test, HelperAliases)
{
  EXPECT_TRUE((std::same_as<storage_t<f32, 4>, simd_traits<f32, 4>::storage_type>));
  EXPECT_TRUE((std::same_as<simd_t<f32, 4>, __m128>));
  EXPECT_TRUE((std::same_as<simd_t<f32, 8>, __m256>));

  EXPECT_EQ((kAlignmentV<f32, 4>), 16);
  EXPECT_EQ((kAlignmentV<f32, 8>), 32);
  EXPECT_EQ((kAlignmentV<f32, 3>), alignof(f32));
}

TEST_F(simd_traits_test, StorageAlignment)
{
  typename simd_traits<f32, 4>::storage_type storage4f{};
  EXPECT_EQ(std::bit_cast<uintptr>(&storage4f) % 16, 0);

  typename simd_traits<f32, 8>::storage_type storage8f{};
  EXPECT_EQ(std::bit_cast<uintptr>(&storage8f) % 32, 0);

  typename simd_traits<f64, 4>::storage_type storage2d{};
  EXPECT_EQ(std::bit_cast<uintptr>(&storage2d) % 16, 0);

  typename simd_traits<f64, 8>::storage_type storage4d{};
  EXPECT_EQ(std::bit_cast<uintptr>(&storage4d) % 32, 0);
}
} // namespace pbrt::math::simd::test
