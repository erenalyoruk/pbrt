#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <pbrt/math/constants.hpp>
#include <pbrt/math/matrix.hpp>
#include <pbrt/math/vector.hpp>
#include <pbrt/types.hpp>

#include <cmath>

using namespace pbrt;
using namespace pbrt::math;

namespace
{
template <typename T>
[[nodiscard]] constexpr Matrix<T, 4, 4> translate(T x, T y, T z) noexcept
{
  return {T{1}, T{0}, T{0}, x, T{0}, T{1}, T{0}, y, T{0}, T{0}, T{1}, z, T{0}, T{0}, T{0}, T{1}};
}

template <typename T>
[[nodiscard]] constexpr Matrix<T, 4, 4> scale(T x, T y, T z) noexcept
{
  return {x, T{0}, T{0}, T{0}, T{0}, y, T{0}, T{0}, T{0}, T{0}, z, T{0}, T{0}, T{0}, T{0}, T{1}};
}

template <typename T>
[[nodiscard]] Matrix<T, 4, 4> rotate_x(T angle) noexcept
{
  const T cosA{std::cos(angle)};
  const T sinA{std::sin(angle)};

  return {T{1}, T{0}, T{0}, T{0}, T{0}, cosA, -sinA, T{0},
          T{0}, sinA, cosA, T{0}, T{0}, T{0}, T{0},  T{1}};
}

template <typename T>
[[nodiscard]] Matrix<T, 4, 4> rotate_y(T angle) noexcept
{
  const T cosA{std::cos(angle)};
  const T sinA{std::sin(angle)};

  return {cosA,  T{0}, sinA, T{0}, T{0}, T{1}, T{0}, T{0},
          -sinA, T{0}, cosA, T{0}, T{0}, T{0}, T{0}, T{1}};
}

template <typename T>
[[nodiscard]] Matrix<T, 4, 4> rotate_z(T angle) noexcept
{
  const T cosA{std::cos(angle)};
  const T sinA{std::sin(angle)};

  return {cosA, -sinA, T{0}, T{0}, sinA, cosA, T{0}, T{0},
          T{0}, T{0},  T{1}, T{0}, T{0}, T{0}, T{0}, T{1}};
}

template <typename T>
[[nodiscard]] Matrix<T, 4, 4> perspective(T fov, T aspect, T nearPlane, T farPlane) noexcept
{
  const T tanHalfFov{std::tan(fov / T{2})};
  const T f{T{1} / tanHalfFov};
  const T range{farPlane - nearPlane};

  return {f / aspect,
          T{0},
          T{0},
          T{0},
          T{0},
          f,
          T{0},
          T{0},
          T{0},
          T{0},
          -(farPlane + nearPlane) / range,
          -(T{2} * farPlane * nearPlane) / range,
          T{0},
          T{0},
          T{-1},
          T{0}};
}

// Helper function to create orthographic projection matrix
template <typename T>
[[nodiscard]] Matrix<T, 4, 4> orthographic(T left, T right, T bottom, T top, T nearPlane,
                                           T farPlane) noexcept
{
  const T width{right - left};
  const T height{top - bottom};
  const T depth{farPlane - nearPlane};

  return Matrix<T, 4, 4>{
      T{2} / width, T{0},          T{0},          -(right + left) / width,
      T{0},         T{2} / height, T{0},          -(top + bottom) / height,
      T{0},         T{0},          T{-2} / depth, -(farPlane + nearPlane) / depth,
      T{0},         T{0},          T{0},          T{1}};
}
} // namespace

TEMPLATE_TEST_CASE("Matrix transformation utilities", "[math][matrix][transform][template]", f32,
                   f64)
{
  constexpr TestType epsilon{EPSILON<TestType>};

  SECTION("Translation matrix")
  {
    auto trans{translate<TestType>(TestType{5}, TestType{10}, TestType{15})};

    // Check translation components
    REQUIRE(trans(0, 3) == Catch::Approx(static_cast<TestType>(5)));
    REQUIRE(trans(1, 3) == Catch::Approx(static_cast<TestType>(10)));
    REQUIRE(trans(2, 3) == Catch::Approx(static_cast<TestType>(15)));

    // Check that it's otherwise identity
    REQUIRE(trans(0, 0) == Catch::Approx(static_cast<TestType>(1)));
    REQUIRE(trans(1, 1) == Catch::Approx(static_cast<TestType>(1)));
    REQUIRE(trans(2, 2) == Catch::Approx(static_cast<TestType>(1)));
    REQUIRE(trans(3, 3) == Catch::Approx(static_cast<TestType>(1)));

    // Test point transformation
    Vector<TestType, 4> point{TestType{1}, TestType{2}, TestType{3}, TestType{1}};
    auto transformed{trans * point};

    REQUIRE(transformed[0] == Catch::Approx(static_cast<TestType>(6)));  // 1 + 5
    REQUIRE(transformed[1] == Catch::Approx(static_cast<TestType>(12))); // 2 + 10
    REQUIRE(transformed[2] == Catch::Approx(static_cast<TestType>(18))); // 3 + 15
    REQUIRE(transformed[3] == Catch::Approx(static_cast<TestType>(1)));
  }

  SECTION("Scale matrix")
  {
    auto scaleMat{scale<TestType>(TestType{2}, TestType{3}, TestType{4})};

    // Check diagonal elements
    REQUIRE(scaleMat(0, 0) == Catch::Approx(static_cast<TestType>(2)));
    REQUIRE(scaleMat(1, 1) == Catch::Approx(static_cast<TestType>(3)));
    REQUIRE(scaleMat(2, 2) == Catch::Approx(static_cast<TestType>(4)));
    REQUIRE(scaleMat(3, 3) == Catch::Approx(static_cast<TestType>(1)));

    // Test vector transformation
    Vector<TestType, 4> vector{TestType{1}, TestType{1}, TestType{1}, TestType{1}};
    auto scaled{scaleMat * vector};

    REQUIRE(scaled[0] == Catch::Approx(static_cast<TestType>(2)));
    REQUIRE(scaled[1] == Catch::Approx(static_cast<TestType>(3)));
    REQUIRE(scaled[2] == Catch::Approx(static_cast<TestType>(4)));
    REQUIRE(scaled[3] == Catch::Approx(static_cast<TestType>(1)));
  }

  SECTION("Rotation matrices")
  {
    const TestType halfPi{PI<TestType> / TestType{2}};

    SECTION("X-axis rotation")
    {
      auto rotX{rotate_x<TestType>(halfPi)};
      Vector<TestType, 4> yAxis{TestType{0}, TestType{1}, TestType{0}, TestType{1}};
      auto rotated{rotX * yAxis};

      // Y should become Z after 90° rotation around X
      REQUIRE(rotated[0] == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
      REQUIRE(rotated[1] == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
      REQUIRE(rotated[2] == Catch::Approx(static_cast<TestType>(1)).margin(epsilon));
      REQUIRE(rotated[3] == Catch::Approx(static_cast<TestType>(1)));
    }

    SECTION("Y-axis rotation")
    {
      auto rotY{rotate_y<TestType>(halfPi)};
      Vector<TestType, 4> xAxis{TestType{1}, TestType{0}, TestType{0}, TestType{1}};
      auto rotated{rotY * xAxis};

      // X should become -Z after 90° rotation around Y
      REQUIRE(rotated[0] == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
      REQUIRE(rotated[1] == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
      REQUIRE(rotated[2] == Catch::Approx(static_cast<TestType>(-1)).margin(epsilon));
      REQUIRE(rotated[3] == Catch::Approx(static_cast<TestType>(1)));
    }

    SECTION("Z-axis rotation")
    {
      auto rotZ{rotate_z<TestType>(halfPi)};
      Vector<TestType, 4> xAxis{TestType{1}, TestType{0}, TestType{0}, TestType{1}};
      auto rotated{rotZ * xAxis};

      // X should become Y after 90° rotation around Z
      REQUIRE(rotated[0] == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
      REQUIRE(rotated[1] == Catch::Approx(static_cast<TestType>(1)).margin(epsilon));
      REQUIRE(rotated[2] == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
      REQUIRE(rotated[3] == Catch::Approx(static_cast<TestType>(1)));
    }

    SECTION("Full rotation cycles")
    {
      const TestType fullRotation{TestType{2} * PI<TestType>};

      auto rotFull{rotate_z<TestType>(fullRotation)};
      auto identity{Matrix<TestType, 4, 4>::identity()};

      REQUIRE(rotFull.approx_equal(
          identity, epsilon * TestType{10})); // Slightly larger epsilon for accumulated error
    }

    SECTION("Multiple rotation composition")
    {
      // Test Euler angle rotations (XYZ order)
      const TestType angle{PI<TestType> / TestType{6}}; // 30 degrees

      auto rotX{rotate_x<TestType>(angle)};
      auto rotY{rotate_y<TestType>(angle)};
      auto rotZ{rotate_z<TestType>(angle)};

      auto combined{rotZ * rotY * rotX};

      // Should still be orthogonal
      auto transpose{combined.transpose()};
      auto shouldBeIdentity{combined * transpose};

      REQUIRE(shouldBeIdentity.approx_equal(Matrix<TestType, 4, 4>::identity(),
                                            epsilon * TestType{10}));
      REQUIRE(combined.determinant() == Catch::Approx(static_cast<TestType>(1)).margin(epsilon));
    }
  }

  SECTION("Rotation matrix properties")
  {
    const TestType angle{PI<TestType> / TestType{4}}; // 45 degrees
    auto rot{rotate_z<TestType>(angle)};

    // Rotation matrices should be orthogonal
    auto rotTranspose{rot.transpose()};
    auto identityCheck{rot * rotTranspose};

    // Check if rot * rot^T ≈ I
    for (usize i = 0; i < 4; ++i)
    {
      for (usize j = 0; j < 4; ++j)
      {
        TestType expected{(i == j) ? TestType{1} : TestType{0}};
        REQUIRE(identityCheck(i, j) == Catch::Approx(expected).margin(epsilon));
      }
    }

    // Determinant should be 1
    REQUIRE(rot.determinant() == Catch::Approx(static_cast<TestType>(1)).margin(epsilon));
  }

  SECTION("Combined transformations")
  {
    // Create a combined transformation: translate then rotate then scale
    auto trans{translate<TestType>(TestType{1}, TestType{2}, TestType{3})};
    auto rot{rotate_z<TestType>(PI<TestType> / TestType{2})}; // 90 degrees
    auto scaleMat{scale<TestType>(TestType{2}, TestType{2}, TestType{2})};

    auto combined{scaleMat * rot * trans};

    // Test with a point
    Vector<TestType, 4> point{TestType{1}, TestType{0}, TestType{0}, TestType{1}};
    auto result{combined * point};

    // After translation: (2, 2, 3)
    // After rotation: (-2, 2, 3)
    // After scaling: (-4, 4, 6)
    REQUIRE(result[0] == Catch::Approx(static_cast<TestType>(-4)).margin(epsilon));
    REQUIRE(result[1] == Catch::Approx(static_cast<TestType>(4)).margin(epsilon));
    REQUIRE(result[2] == Catch::Approx(static_cast<TestType>(6)).margin(epsilon));
    REQUIRE(result[3] == Catch::Approx(static_cast<TestType>(1)));
  }

  SECTION("Inverse transformations")
  {
    auto trans{translate<TestType>(TestType{5}, TestType{10}, TestType{15})};
    auto invTrans{translate<TestType>(TestType{-5}, TestType{-10}, TestType{-15})};

    auto identityCheck{trans * invTrans};

    // Should be approximately identity
    REQUIRE(identityCheck.approx_equal(Matrix<TestType, 4, 4>::identity(), epsilon));

    // Test with matrix inverse function
    auto computedInverse{trans.inverse()};
    auto identityCheck2{trans * computedInverse};
    REQUIRE(identityCheck2.approx_equal(Matrix<TestType, 4, 4>::identity(), epsilon));
  }

  SECTION("Point vs direction vector transformation")
  {
    auto trans{translate<TestType>(TestType{5}, TestType{10}, TestType{15})};

    // Point (w=1) should be translated
    Vector<TestType, 4> point{TestType{1}, TestType{2}, TestType{3}, TestType{1}};
    auto transPoint{trans * point};

    // Direction vector (w=0) should not be translated
    Vector<TestType, 4> direction{TestType{1}, TestType{2}, TestType{3}, TestType{0}};
    auto transDirection{trans * direction};

    REQUIRE(transPoint[0] == Catch::Approx(static_cast<TestType>(6)));
    REQUIRE(transDirection[0] == Catch::Approx(static_cast<TestType>(1))); // No translation
    REQUIRE(transDirection[3] == Catch::Approx(static_cast<TestType>(0))); // w component preserved
  }
}

TEMPLATE_TEST_CASE("Projection matrices", "[math][matrix][projection][template]", f32, f64)
{
  constexpr TestType epsilon{EPSILON<TestType>};

  SECTION("Perspective projection")
  {
    const auto fov{PI<TestType> / TestType{4}};    // 45 degrees
    const auto aspect{TestType{16} / TestType{9}}; // 16:9 aspect ratio
    const auto nearPlane{TestType{0.1}};
    const auto farPlane{TestType{100}};

    auto proj{perspective<TestType>(fov, aspect, nearPlane, farPlane)};

    // Test that points on the near plane map correctly
    Vector<TestType, 4> nearPoint{TestType{0}, TestType{0}, -nearPlane, TestType{1}};
    auto projected{proj * nearPoint};

    // After perspective divide, z should be -1 (normalized device coordinates)
    TestType w{projected[3]};
    REQUIRE(w != TestType{0}); // Should not be zero

    TestType ndcZ{projected[2] / w};
    REQUIRE(ndcZ == Catch::Approx(static_cast<TestType>(-1)).margin(epsilon));
  }

  SECTION("Orthographic projection")
  {
    const auto left{TestType{-10}};
    const auto right{TestType{10}};
    const auto bottom{TestType{-5}};
    const auto top{TestType{5}};
    const auto nearPlane{TestType{0.1}};
    const auto farPlane{TestType{100}};

    auto proj{orthographic<TestType>(left, right, bottom, top, nearPlane, farPlane)};

    // Test corner points
    Vector<TestType, 4> corner{right, top, -nearPlane, TestType{1}};
    auto projected{proj * corner};

    // In NDC, this should be at (1, 1, -1)
    REQUIRE(projected[0] == Catch::Approx(static_cast<TestType>(1)).margin(epsilon));
    REQUIRE(projected[1] == Catch::Approx(static_cast<TestType>(1)).margin(epsilon));
    REQUIRE(projected[2] == Catch::Approx(static_cast<TestType>(-1)).margin(epsilon));
    REQUIRE(projected[3] == Catch::Approx(static_cast<TestType>(1)));
  }

  SECTION("Projection matrix properties")
  {
    auto proj{
        perspective<TestType>(PI<TestType> / TestType{4}, TestType{1}, TestType{1}, TestType{100})};

    // The bottom-right element should be 0 for perspective projection
    REQUIRE(proj(3, 3) == Catch::Approx(static_cast<TestType>(0)));

    // The (3,2) element should be -1 for perspective projection
    REQUIRE(proj(3, 2) == Catch::Approx(static_cast<TestType>(-1)));
  }

  SECTION("Perspective projection edge cases")
  {
    // Very small FOV
    const TestType smallFov{PI<TestType> / TestType{180}}; // 1 degree
    auto narrowProj{perspective<TestType>(smallFov, TestType{1}, TestType{1}, TestType{100})};

    // Should have very large f value
    REQUIRE(narrowProj(0, 0) > TestType{50}); // f should be large for small FOV

    // Very wide FOV
    const TestType wideFov{PI<TestType> * TestType{0.9}}; // 162 degrees
    auto wideProj{perspective<TestType>(wideFov, TestType{1}, TestType{1}, TestType{100})};

    // Should have small f value
    REQUIRE(wideProj(0, 0) < TestType{1}); // f should be small for wide FOV
  }

  SECTION("Orthographic with extreme ratios")
  {
    // Very wide orthographic projection
    auto wideOrtho{orthographic<TestType>(TestType{-100}, TestType{100}, TestType{-1}, TestType{1},
                                          TestType{1}, TestType{100})};

    Vector<TestType, 4> point{TestType{50}, TestType{0}, TestType{-1}, TestType{1}};
    auto projected{wideOrtho * point};

    REQUIRE(projected[0] == Catch::Approx(static_cast<TestType>(0.5))); // Should be in [-1,1]
    REQUIRE(projected[1] == Catch::Approx(static_cast<TestType>(0)));
  }
}

TEST_CASE("Matrix transform composition", "[math][matrix][transform]")
{
  SECTION("Transform decomposition")
  {
    // Create a known transformation
    auto scaleMat{scale<f32>(2.0F, 3.0F, 4.0F)};
    auto rotation{rotate_z<f32>(PI<f32> / 4.0F)}; // 45 degrees
    auto translation{translate<f32>(10.0F, 20.0F, 30.0F)};

    // Combine: T * R * S (standard order)
    auto combined{translation * rotation * scaleMat};

    // The translation part should be preserved in the last column
    REQUIRE(combined(0, 3) == Catch::Approx(10.0F));
    REQUIRE(combined(1, 3) == Catch::Approx(20.0F));
    REQUIRE(combined(2, 3) == Catch::Approx(30.0F));
    REQUIRE(combined(3, 3) == Catch::Approx(1.0F));
  }

  SECTION("Look-at transformation simulation")
  {
    // Simulate a simple look-at transformation by combining rotations
    Vector<f32, 3> eye{0.0F, 0.0F, 5.0F};
    Vector<f32, 3> target{0.0F, 0.0F, 0.0F};
    Vector<f32, 3> up{0.0F, 1.0F, 0.0F};

    // For this simple case (looking down -Z), the view matrix should be
    // mostly identity with a translation
    auto viewTranslation{translate<f32>(-eye[0], -eye[1], -eye[2])};

    // Test that a point at the origin moves to (0, 0, -5) in view space
    Vector<f32, 4> origin{0.0F, 0.0F, 0.0F, 1.0F};
    auto viewSpace{viewTranslation * origin};

    REQUIRE(viewSpace[0] == Catch::Approx(0.0F));
    REQUIRE(viewSpace[1] == Catch::Approx(0.0F));
    REQUIRE(viewSpace[2] == Catch::Approx(-5.0F));
    REQUIRE(viewSpace[3] == Catch::Approx(1.0F));
  }
}

TEMPLATE_TEST_CASE("Matrix transform edge cases", "[math][matrix][transform][template]", f32, f64)
{
  constexpr TestType epsilon{EPSILON<TestType>};

  SECTION("Zero scale matrix")
  {
    auto zeroScale{scale<TestType>(TestType{0}, TestType{1}, TestType{1})};

    // Should create a singular matrix
    REQUIRE(zeroScale.determinant() == Catch::Approx(static_cast<TestType>(0)));
    REQUIRE_THROWS_AS(zeroScale.inverse(), std::domain_error);
  }

  SECTION("Identity transformations")
  {
    auto identityTrans{translate<TestType>(TestType{0}, TestType{0}, TestType{0})};
    auto identityScale{scale<TestType>(TestType{1}, TestType{1}, TestType{1})};
    auto identityRot{rotate_x<TestType>(TestType{0})};

    auto identity{Matrix<TestType, 4, 4>::identity()};

    REQUIRE(identityTrans.approx_equal(identity, epsilon));
    REQUIRE(identityScale.approx_equal(identity, epsilon));
    REQUIRE(identityRot.approx_equal(identity, epsilon));
  }

  SECTION("Negative scale transformations")
  {
    auto negScale{scale<TestType>(TestType{-1}, TestType{1}, TestType{1})};

    // Should flip the determinant
    REQUIRE(negScale.determinant() == Catch::Approx(static_cast<TestType>(-1)));

    Vector<TestType, 4> point{TestType{1}, TestType{2}, TestType{3}, TestType{1}};
    auto transformed{negScale * point};

    REQUIRE(transformed[0] == Catch::Approx(static_cast<TestType>(-1)));
    REQUIRE(transformed[1] == Catch::Approx(static_cast<TestType>(2)));
    REQUIRE(transformed[2] == Catch::Approx(static_cast<TestType>(3)));
  }
}

// ...existing code...

TEST_CASE("Transformation order dependency", "[math][matrix][transform]")
{
  SECTION("Non-commutative operations")
  {
    auto trans{translate<f32>(5.0F, 0.0F, 0.0F)};
    auto rot{rotate_z<f32>(PI<f32> / 2.0F)}; // 90 degrees

    // T * R vs R * T should be different
    auto transFirst{trans * rot};
    auto rotFirst{rot * trans};

    Vector<f32, 4> point{1.0F, 0.0F, 0.0F, 1.0F};
    auto result1{transFirst * point};
    auto result2{rotFirst * point};

    // Use appropriate epsilon for floating-point comparisons
    constexpr f32 epsilon{1e-6F};

    // Let's check what the actual results should be:
    // T * R * point: First rotate (1,0,0) -> (0,1,0), then translate -> (5,1,0)
    // R * T * point: First translate (1,0,0) -> (6,0,0), then rotate -> (0,6,0)

    // Verify the actual expected values instead of just checking they're different
    REQUIRE(result1[0] == Catch::Approx(5.0F).margin(epsilon));
    REQUIRE(result1[1] == Catch::Approx(1.0F).margin(epsilon));
    REQUIRE(result1[2] == Catch::Approx(0.0F).margin(epsilon));

    REQUIRE(result2[0] == Catch::Approx(0.0F).margin(epsilon));
    REQUIRE(result2[1] == Catch::Approx(6.0F).margin(epsilon));
    REQUIRE(result2[2] == Catch::Approx(0.0F).margin(epsilon));

    // Also verify they are indeed different
    REQUIRE_FALSE(result1[0] == Catch::Approx(result2[0]));
    REQUIRE_FALSE(result1[1] == Catch::Approx(result2[1]));
  }

  SECTION("Scale before vs after rotation")
  {
    auto scaleNonUniform{scale<f32>(2.0F, 1.0F, 1.0F)};
    auto rot{rotate_z<f32>(PI<f32> / 4.0F)}; // 45 degrees

    auto scaleFirst{rot * scaleNonUniform};
    auto rotFirst{scaleNonUniform * rot};

    Vector<f32, 4> point{1.0F, 0.0F, 0.0F, 1.0F};
    auto result1{scaleFirst * point};
    auto result2{rotFirst * point};

    constexpr auto sqrt2{SQRT_2<f32>};
    constexpr auto sqrt2Half{SQRT_HALF_2<f32>};

    REQUIRE(result1[0] == Catch::Approx(sqrt2).margin(1e-6F));
    REQUIRE(result1[1] == Catch::Approx(sqrt2).margin(1e-6F));
    REQUIRE(result1[2] == Catch::Approx(0.0F));

    REQUIRE(result2[0] == Catch::Approx(sqrt2).margin(1e-6F));
    REQUIRE(result2[1] == Catch::Approx(sqrt2Half).margin(1e-6F));
    REQUIRE(result2[2] == Catch::Approx(0.0F));

    // Should produce different results
    REQUIRE_FALSE(result1[1] == Catch::Approx(result2[1]));
  }

  SECTION("Translation and rotation order with detailed verification")
  {
    // Use a more obvious case to verify transformation order
    auto trans{translate<f32>(0.0F, 5.0F, 0.0F)}; // Translate in Y
    auto rot{rotate_z<f32>(PI<f32> / 2.0F)};      // 90 degrees around Z

    Vector<f32, 4> point{1.0F, 0.0F, 0.0F, 1.0F}; // Point on X-axis

    // Method 1: Translate first, then rotate
    auto step1{trans * point}; // (1, 5, 0)
    auto final1{rot * step1};  // Should be (-5, 1, 0)

    // Method 2: Using combined matrix (T then R)
    auto combined1{rot * trans};
    auto result1{combined1 * point};

    // Method 3: Rotate first, then translate
    auto step2{rot * point};    // (0, 1, 0)
    auto final2{trans * step2}; // Should be (0, 6, 0)

    // Method 4: Using combined matrix (R then T)
    auto combined2{trans * rot};
    auto result2{combined2 * point};

    // Use appropriate epsilon for floating-point comparisons
    constexpr f32 epsilon{1e-6F};

    // Verify the results
    REQUIRE(result1[0] == Catch::Approx(-5.0F).margin(epsilon));
    REQUIRE(result1[1] == Catch::Approx(1.0F).margin(epsilon));
    REQUIRE(result1[2] == Catch::Approx(0.0F).margin(epsilon));

    REQUIRE(result2[0] == Catch::Approx(0.0F).margin(epsilon));
    REQUIRE(result2[1] == Catch::Approx(6.0F).margin(epsilon));
    REQUIRE(result2[2] == Catch::Approx(0.0F).margin(epsilon));

    // Verify consistency between step-by-step and matrix approaches
    REQUIRE(final1[0] == Catch::Approx(result1[0]).margin(epsilon));
    REQUIRE(final1[1] == Catch::Approx(result1[1]).margin(epsilon));
    REQUIRE(final2[0] == Catch::Approx(result2[0]).margin(epsilon));
    REQUIRE(final2[1] == Catch::Approx(result2[1]).margin(epsilon));
  }
}

TEST_CASE("Matrix transform numerical stability", "[math][matrix][transform]")
{
  SECTION("Very small transformations")
  {
    constexpr f32 verySmall{1e-6F};
    auto smallTrans{translate<f32>(verySmall, verySmall, verySmall)};
    auto smallScale{scale<f32>(1.0F + verySmall, 1.0F + verySmall, 1.0F + verySmall)};

    // Should not degenerate to identity due to precision loss
    REQUIRE_FALSE(smallTrans.approx_equal(Matrix<f32, 4, 4>::identity(), 1e-7F));
    REQUIRE_FALSE(smallScale.approx_equal(Matrix<f32, 4, 4>::identity(), 1e-7F));
  }

  SECTION("Large transformations")
  {
    constexpr f32 large{1e6F};
    auto largeTrans{translate<f32>(large, large, large)};

    Vector<f32, 4> point{1.0F, 1.0F, 1.0F, 1.0F};
    auto result{largeTrans * point};

    // Should handle large values without overflow
    REQUIRE(std::isfinite(result[0]));
    REQUIRE(std::isfinite(result[1]));
    REQUIRE(std::isfinite(result[2]));
  }
}
