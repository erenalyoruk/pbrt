#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <pbrt/math/constants.hpp>
#include <pbrt/math/quaternion.hpp>
#include <pbrt/math/vector.hpp>
#include <pbrt/types.hpp>

#include <cmath>
#include <utility>

using namespace pbrt;
using namespace pbrt::math;

TEMPLATE_TEST_CASE("Quaternion construction and element access", "[math][quaternion][template]",
                   f32, f64)
{
  SECTION("Default constructor")
  {
    Quaternion<TestType> q{};
    REQUIRE(q.w() == TestType{1});
    REQUIRE(q.x() == TestType{0});
    REQUIRE(q.y() == TestType{0});
    REQUIRE(q.z() == TestType{0});
  }

  SECTION("Constructor with components")
  {
    Quaternion<TestType> q{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    REQUIRE(q.w() == TestType{1});
    REQUIRE(q.x() == TestType{2});
    REQUIRE(q.y() == TestType{3});
    REQUIRE(q.z() == TestType{4});
  }

  SECTION("Constructor with vector and scalar")
  {
    Vector<TestType, 3> v{TestType{1}, TestType{2}, TestType{3}};
    Quaternion<TestType> q{TestType{4}, v};
    REQUIRE(q.w() == TestType{4});
    REQUIRE(q.x() == TestType{1});
    REQUIRE(q.y() == TestType{2});
    REQUIRE(q.z() == TestType{3});
  }

  SECTION("Copy constructor and assignment")
  {
    Quaternion<TestType> q1{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    Quaternion<TestType> q2{q1};
    REQUIRE(q1 == q2);

    Quaternion<TestType> q3;
    q3 = q1;
    REQUIRE(q3 == q2);
    REQUIRE(q3 == q1);
  }

  SECTION("Move constructor and assignment")
  {
    Quaternion<TestType> q1{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    Quaternion<TestType> q2{std::move(q1)};
    REQUIRE(q2.w() == TestType{1});
    REQUIRE(q2.x() == TestType{2});
    REQUIRE(q2.y() == TestType{3});
    REQUIRE(q2.z() == TestType{4});

    Quaternion<TestType> q3;
    q3 = std::move(q2);
    REQUIRE(q3 == q2);
  }

  SECTION("Element access and modification")
  {
    Quaternion<TestType> q{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    REQUIRE(q.w() == TestType{1});
    REQUIRE(q.x() == TestType{2});
    REQUIRE(q.y() == TestType{3});
    REQUIRE(q.z() == TestType{4});

    REQUIRE(q[0] == TestType{1});
    REQUIRE(q[1] == TestType{2});
    REQUIRE(q[2] == TestType{3});
    REQUIRE(q[3] == TestType{4});

    q[0] = TestType{5};
    q.x() = TestType{6};
    q.y() = TestType{7};
    q[3] = TestType{8};

    REQUIRE(q.w() == TestType{5});
    REQUIRE(q.x() == TestType{6});
    REQUIRE(q.y() == TestType{7});
    REQUIRE(q.z() == TestType{8});
  }

  SECTION("Vector part access")
  {
    Quaternion<TestType> q{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    Vector<TestType, 3> v{q.vector()};
    REQUIRE(v.x() == TestType{2});
    REQUIRE(v.y() == TestType{3});
    REQUIRE(v.z() == TestType{4});
  }
}

TEMPLATE_TEST_CASE("Quaternion arithmetic operations", "[math][quaternion][template]", f32, f64)
{
  Quaternion<TestType> q1{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
  Quaternion<TestType> q2{TestType{5}, TestType{6}, TestType{7}, TestType{8}};

  SECTION("Addition")
  {
    auto result{q1 + q2};
    REQUIRE(result.w() == TestType{6});
    REQUIRE(result.x() == TestType{8});
    REQUIRE(result.y() == TestType{10});
    REQUIRE(result.z() == TestType{12});

    Quaternion<TestType> q3{q1};
    q3 += q2;
    REQUIRE(q3 == result);
  }

  SECTION("Subtraction")
  {
    auto result = q2 - q1;
    REQUIRE(result.w() == TestType{4});
    REQUIRE(result.x() == TestType{4});
    REQUIRE(result.y() == TestType{4});
    REQUIRE(result.z() == TestType{4});

    Quaternion<TestType> q3{q2};
    q3 -= q1;
    REQUIRE(q3 == result);
  }

  SECTION("Scalar multiplication")
  {
    auto result{q1 * TestType{2}};
    REQUIRE(result.w() == TestType{2});
    REQUIRE(result.x() == TestType{4});
    REQUIRE(result.y() == TestType{6});
    REQUIRE(result.z() == TestType{8});

    auto result2{TestType{3} * q1};
    REQUIRE(result2.w() == TestType{3});
    REQUIRE(result2.x() == TestType{6});
    REQUIRE(result2.y() == TestType{9});
    REQUIRE(result2.z() == TestType{12});

    Quaternion<TestType> q3{q1};
    q3 *= TestType{2};
    REQUIRE(q3 == result);
  }

  SECTION("Unary negation")
  {
    auto result{-q1};
    REQUIRE(result.w() == TestType{-1});
    REQUIRE(result.x() == TestType{-2});
    REQUIRE(result.y() == TestType{-3});
    REQUIRE(result.z() == TestType{-4});
  }

  SECTION("Comparison operators")
  {
    Quaternion<TestType> q3{q1};
    REQUIRE(q1 != q2);
    REQUIRE(q1 == q3);

    REQUIRE_FALSE(q1 == q2);
    REQUIRE_FALSE(q1 != q3);
  }
}

TEMPLATE_TEST_CASE("Quaternion multiplication", "[math][quaternion][template]", f32, f64)
{
  SECTION("Basic quaternion multiplication")
  {
    Quaternion<TestType> i{TestType{0}, TestType{1}, TestType{0}, TestType{0}};
    Quaternion<TestType> j{TestType{0}, TestType{0}, TestType{1}, TestType{0}};
    Quaternion<TestType> k{TestType{0}, TestType{0}, TestType{0}, TestType{1}};

    auto ij{i * j};
    REQUIRE(ij.w() == Catch::Approx(static_cast<TestType>(0)));
    REQUIRE(ij.x() == Catch::Approx(static_cast<TestType>(0)));
    REQUIRE(ij.y() == Catch::Approx(static_cast<TestType>(0)));
    REQUIRE(ij.z() == Catch::Approx(static_cast<TestType>(1)));

    auto jk{j * k};
    REQUIRE(jk.w() == Catch::Approx(static_cast<TestType>(0)));
    REQUIRE(jk.x() == Catch::Approx(static_cast<TestType>(1)));
    REQUIRE(jk.y() == Catch::Approx(static_cast<TestType>(0)));
    REQUIRE(jk.z() == Catch::Approx(static_cast<TestType>(0)));

    auto ki{k * i};
    REQUIRE(ki.w() == Catch::Approx(static_cast<TestType>(0)));
    REQUIRE(ki.x() == Catch::Approx(static_cast<TestType>(0)));
    REQUIRE(ki.y() == Catch::Approx(static_cast<TestType>(1)));
    REQUIRE(ki.z() == Catch::Approx(static_cast<TestType>(0)));
  }

  SECTION("Identity quaternion multiplication")
  {
    Quaternion<TestType> identity{TestType{1}, TestType{0}, TestType{0}, TestType{0}};
    Quaternion<TestType> q{TestType{2}, TestType{3}, TestType{4}, TestType{5}};

    auto result1{identity * q};
    auto result2{q * identity};

    REQUIRE(result1 == q);
    REQUIRE(result2 == q);
  }

  SECTION("Non-commutative property")
  {
    Quaternion<TestType> q1{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    Quaternion<TestType> q2{TestType{5}, TestType{6}, TestType{7}, TestType{8}};

    auto result1{q1 * q2};
    auto result2{q2 * q1};

    REQUIRE_FALSE(result1 == result2);
  }

  SECTION("Associative property")
  {
    Quaternion<TestType> q1{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    Quaternion<TestType> q2{TestType{5}, TestType{6}, TestType{7}, TestType{8}};
    Quaternion<TestType> q3{TestType{9}, TestType{10}, TestType{11}, TestType{12}};

    auto result1{(q1 * q2) * q3};
    auto result2{q1 * (q2 * q3)};

    constexpr TestType epsilon{EPSILON<TestType> * TestType{10}};
    REQUIRE(result1.w() == Catch::Approx(result2.w()).margin(epsilon));
    REQUIRE(result1.x() == Catch::Approx(result2.x()).margin(epsilon));
    REQUIRE(result1.y() == Catch::Approx(result2.y()).margin(epsilon));
    REQUIRE(result1.z() == Catch::Approx(result2.z()).margin(epsilon));
  }
}

TEMPLATE_TEST_CASE("Quaternion magnitude and normalization", "[math][quaternion][template]", f32,
                   f64)
{
  SECTION("Magnitude calculation")
  {
    Quaternion<TestType> q{TestType{1}, TestType{2}, TestType{3}, TestType{4}};

    auto lengthSquared{q.length_squared()};
    auto length{q.length()};

    REQUIRE(lengthSquared == Catch::Approx(static_cast<TestType>(30)));
    REQUIRE(length == Catch::Approx(std::sqrt(static_cast<TestType>(30))));
  }

  SECTION("Normalization")
  {
    Quaternion<TestType> q{TestType{1}, TestType{2}, TestType{3}, TestType{4}};

    auto normalized{q.normalized()};
    REQUIRE(normalized.length() == Catch::Approx(static_cast<TestType>(1)));

    REQUIRE(q.length() == Catch::Approx(std::sqrt(static_cast<TestType>(30))));

    q.normalize();
    REQUIRE(q.length() == Catch::Approx(static_cast<TestType>(1)));
  }

  SECTION("Zero quaternion edge case")
  {
    Quaternion<TestType> zero{TestType{0}, TestType{0}, TestType{0}, TestType{0}};

    REQUIRE(zero.length() == TestType{0});
    REQUIRE(zero.length_squared() == TestType{0});

    REQUIRE_THROWS_AS(zero.normalize(), std::domain_error);
    REQUIRE_THROWS_AS(zero.normalized(), std::domain_error);
  }
}

TEMPLATE_TEST_CASE("Quaternion conjugate and inverse", "[math][quaternion][template]", f32, f64)
{
  SECTION("Conjugate")
  {
    Quaternion<TestType> q{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    auto conj{q.conjugate()};

    REQUIRE(conj.w() == TestType{1});
    REQUIRE(conj.x() == TestType{-2});
    REQUIRE(conj.y() == TestType{-3});
    REQUIRE(conj.z() == TestType{-4});

    auto doubleConj{conj.conjugate()};
    REQUIRE(doubleConj == q);
  }

  SECTION("Inverse")
  {
    Quaternion<TestType> q{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    auto inv{q.inverse()};

    auto product{q * inv};
    constexpr TestType epsilon{EPSILON<TestType> * TestType{10}};
    REQUIRE(product.w() == Catch::Approx(TestType{1}).margin(epsilon));
    REQUIRE(product.x() == Catch::Approx(TestType{0}).margin(epsilon));
    REQUIRE(product.y() == Catch::Approx(TestType{0}).margin(epsilon));
    REQUIRE(product.z() == Catch::Approx(TestType{0}).margin(epsilon));

    auto product2{inv * q};
    REQUIRE(product2.w() == Catch::Approx(TestType{1}).margin(epsilon));
    REQUIRE(product2.x() == Catch::Approx(TestType{0}).margin(epsilon));
    REQUIRE(product2.y() == Catch::Approx(TestType{0}).margin(epsilon));
    REQUIRE(product2.z() == Catch::Approx(TestType{0}).margin(epsilon));
  }

  SECTION("Unit quaternion inverse")
  {
    Quaternion<TestType> q{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    q.normalize();

    auto inv{q.inverse()};
    auto conj{q.conjugate()};

    constexpr TestType epsilon{EPSILON<TestType> * TestType{10}};

    REQUIRE(inv.w() == Catch::Approx(conj.w()).margin(epsilon));
    REQUIRE(inv.x() == Catch::Approx(conj.x()).margin(epsilon));
    REQUIRE(inv.y() == Catch::Approx(conj.y()).margin(epsilon));
    REQUIRE(inv.z() == Catch::Approx(conj.z()).margin(epsilon));
  }

  SECTION("Zero quaternion inverse edge case")
  {
    Quaternion<TestType> zero{TestType{0}, TestType{0}, TestType{0}, TestType{0}};

    REQUIRE_THROWS_AS(zero.inverse(), std::domain_error);
  }
}

TEMPLATE_TEST_CASE("Quaternion rotation functionality", "[math][quaternion][template]", f32, f64)
{
  constexpr TestType epsilon{EPSILON<TestType> * TestType{10}};

  SECTION("Axis-angle construction")
  {
    Vector<TestType, 3> axis{TestType{0}, TestType{0}, TestType{1}};
    TestType angle{PI<TestType> / TestType{2}};

    auto q{Quaternion<TestType>::from_axis_angle(axis, angle)};

    TestType halfAngle{angle / TestType{2}};
    TestType cosHalf{std::cos(halfAngle)};
    TestType sinHalf{std::sin(halfAngle)};

    REQUIRE(q.w() == Catch::Approx(cosHalf).margin(epsilon));
    REQUIRE(q.x() == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
    REQUIRE(q.y() == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
    REQUIRE(q.z() == Catch::Approx(sinHalf).margin(epsilon));
  }

  SECTION("Vector rotation")
  {
    Vector<TestType, 3> axis{TestType{0}, TestType{0}, TestType{1}};
    TestType angle{PI<TestType> / TestType{2}};

    auto q{Quaternion<TestType>::from_axis_angle(axis, angle)};

    Vector<TestType, 3> v{TestType{1}, TestType{0}, TestType{0}};
    auto rotated{q.rotate(v)};

    REQUIRE(rotated.x() == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
    REQUIRE(rotated.y() == Catch::Approx(static_cast<TestType>(1)).margin(epsilon));
    REQUIRE(rotated.z() == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
  }

  SECTION("180 degree rotation")
  {
    Vector<TestType, 3> axis{TestType{0}, TestType{1}, TestType{0}};
    TestType angle{PI<TestType>};

    auto q{Quaternion<TestType>::from_axis_angle(axis, angle)};

    Vector<TestType, 3> v{TestType{1}, TestType{0}, TestType{0}};
    auto rotated{q.rotate(v)};

    REQUIRE(rotated.x() == Catch::Approx(static_cast<TestType>(-1)).margin(epsilon));
    REQUIRE(rotated.y() == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
    REQUIRE(rotated.z() == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
  }

  SECTION("Identity rotation")
  {
    auto identity{Quaternion<TestType>::identity()};
    Vector<TestType, 3> v{TestType{1}, TestType{2}, TestType{3}};
    auto rotated{identity.rotate(v)};

    REQUIRE(rotated.x() == Catch::Approx(v.x()).margin(epsilon));
    REQUIRE(rotated.y() == Catch::Approx(v.y()).margin(epsilon));
    REQUIRE(rotated.z() == Catch::Approx(v.z()).margin(epsilon));
  }

  SECTION("Small angle rotation")
  {
    Vector<TestType, 3> axis{TestType{0}, TestType{1}, TestType{0}};
    TestType angle{EPSILON<TestType> * TestType{10}};

    auto q{Quaternion<TestType>::from_axis_angle(axis, angle)};

    Vector<TestType, 3> v{TestType{1}, TestType{0}, TestType{0}};
    auto rotated{q.rotate(v)};

    REQUIRE(rotated.x() == Catch::Approx(v.x()).margin(epsilon));
    REQUIRE(rotated.y() == Catch::Approx(v.y()).margin(epsilon));
    REQUIRE(rotated.z() == Catch::Approx(v.z()).margin(epsilon));
  }
}

TEMPLATE_TEST_CASE("Quaternion interpolation", "[math][quaternion][template]", f32, f64)
{
  constexpr TestType epsilon{EPSILON<TestType> * TestType{10}};

  SECTION("Spherical linear interpolation (SLERP)")
  {
    auto q1{Quaternion<TestType>::identity()};
    Vector<TestType, 3> axis{TestType{0}, TestType{0}, TestType{1}};
    auto q2{Quaternion<TestType>::from_axis_angle(axis, PI<TestType> / TestType{2})};

    auto slerpHalf{slerp(q1, q2, TestType{0.5})};

    TestType expectedAngle{PI<TestType> / TestType{4}};
    TestType halfAngle{expectedAngle / TestType{2}};

    REQUIRE(slerpHalf.w() == Catch::Approx(std::cos(halfAngle)).margin(epsilon));
    REQUIRE(slerpHalf.z() == Catch::Approx(std::sin(halfAngle)).margin(epsilon));

    auto slerpStart{slerp(q1, q2, TestType{0})};
    auto slerpEnd{slerp(q1, q2, TestType{1})};

    REQUIRE(slerpStart.w() == Catch::Approx(q1.w()).margin(epsilon));
    REQUIRE(slerpEnd.w() == Catch::Approx(q2.w()).margin(epsilon));
  }

  SECTION("SLERP with opposite quaternions")
  {
    auto q1{Quaternion<TestType>::identity()};
    auto q2{-q1};

    auto slerpResult{slerp(q1, q2, TestType{0.5})};

    REQUIRE(std::isfinite(slerpResult.w()));
    REQUIRE(std::isfinite(slerpResult.x()));
    REQUIRE(std::isfinite(slerpResult.y()));
    REQUIRE(std::isfinite(slerpResult.z()));
  }
}

TEMPLATE_TEST_CASE("Quaternion conversion and extraction", "[math][quaternion][template]", f32, f64)
{
  constexpr TestType epsilon{EPSILON<TestType> * TestType{100}};

  SECTION("To axis-angle conversion")
  {
    Vector<TestType, 3> originalAxis{TestType{0}, TestType{1}, TestType{0}};
    TestType originalAngle = PI<TestType> / TestType{3}; // 60 degrees

    auto q = Quaternion<TestType>::from_axis_angle(originalAxis, originalAngle);
    auto [extracted_axis, extracted_angle] = q.to_axis_angle();

    REQUIRE(extracted_angle == Catch::Approx(originalAngle).margin(epsilon));
    REQUIRE(extracted_axis[0] == Catch::Approx(originalAxis[0]).margin(epsilon));
    REQUIRE(extracted_axis[1] == Catch::Approx(originalAxis[1]).margin(epsilon));
    REQUIRE(extracted_axis[2] == Catch::Approx(originalAxis[2]).margin(epsilon));
  }

  SECTION("Identity quaternion to axis-angle")
  {
    auto identity{Quaternion<TestType>::identity()};
    auto [axis, angle]{identity.to_axis_angle()};

    REQUIRE(angle == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
    REQUIRE(axis.length() == Catch::Approx(static_cast<TestType>(1)).margin(epsilon));
  }

  SECTION("Small angle axis-angle conversion")
  {
    Vector<TestType, 3> axis{TestType{1}, TestType{0}, TestType{0}};
    auto smallAngle{TestType{0.001}};

    auto q{Quaternion<TestType>::from_axis_angle(axis, smallAngle)};
    auto [extracted_axis, extracted_angle]{q.to_axis_angle()};

    REQUIRE(extracted_angle == Catch::Approx(smallAngle).margin(epsilon));
    REQUIRE(extracted_axis[0] == Catch::Approx(static_cast<TestType>(1)).margin(epsilon));
    REQUIRE(extracted_axis[1] == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
    REQUIRE(extracted_axis[2] == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
  }

  SECTION("Large angle axis-angle conversion")
  {
    Vector<TestType, 3> axis{TestType{0}, TestType{0}, TestType{1}};
    TestType largeAngle{PI<TestType> * TestType{0.9}}; // 162 degrees

    auto q{Quaternion<TestType>::from_axis_angle(axis, largeAngle)};
    auto [extracted_axis, extracted_angle]{q.to_axis_angle()};

    REQUIRE(extracted_angle == Catch::Approx(largeAngle).margin(epsilon));
    REQUIRE(extracted_axis[2] == Catch::Approx(static_cast<TestType>(1)).margin(epsilon));
  }
}

TEMPLATE_TEST_CASE("Quaternion static factory methods", "[math][quaternion][template]", f32, f64)
{
  SECTION("Identity quaternion")
  {
    auto identityQuat{Quaternion<TestType>::identity()};
    REQUIRE(identityQuat.w() == TestType{1});
    REQUIRE(identityQuat.x() == TestType{0});
    REQUIRE(identityQuat.y() == TestType{0});
    REQUIRE(identityQuat.z() == TestType{0});
  }
}

TEMPLATE_TEST_CASE("Quaternion approximate equality", "[math][quaternion][template]", f32, f64)
{
  SECTION("Approximate equality with small differences")
  {
    Quaternion<TestType> firstQuat{TestType{1}, TestType{2}, TestType{3}, TestType{4}};

    if constexpr (std::is_same_v<TestType, f32>)
    {
      Quaternion<TestType> secondQuat{1.00001F, 2.00001F, 3.00001F, 4.00001F};

      REQUIRE_FALSE(firstQuat == secondQuat);
      REQUIRE(firstQuat.approx_equal(secondQuat, 0.0001F));
      REQUIRE_FALSE(firstQuat.approx_equal(secondQuat, 0.000001F));
    }
    else
    {
      Quaternion<TestType> secondQuat{1.0000001, 2.0000001, 3.0000001, 4.0000001};

      REQUIRE_FALSE(firstQuat == secondQuat);
      REQUIRE(firstQuat.approx_equal(secondQuat, 0.000001));
      REQUIRE_FALSE(firstQuat.approx_equal(secondQuat, 0.00000001));
    }
  }

  SECTION("Double-cover property")
  {
    Quaternion<TestType> testQuat{TestType{0.5}, TestType{0.5}, TestType{0.5}, TestType{0.5}};
    testQuat.normalize();
    Quaternion<TestType> negativeQuat{-testQuat};

    // testQuat and -testQuat represent the same rotation
    Vector<TestType, 3> testVector{TestType{1}, TestType{0}, TestType{0}};
    auto firstRotated{testQuat.rotate(testVector)};
    auto secondRotated{negativeQuat.rotate(testVector)};

    constexpr TestType allowedError{EPSILON<TestType> * TestType{10}};
    REQUIRE(firstRotated[0] == Catch::Approx(secondRotated[0]).margin(allowedError));
    REQUIRE(firstRotated[1] == Catch::Approx(secondRotated[1]).margin(allowedError));
    REQUIRE(firstRotated[2] == Catch::Approx(secondRotated[2]).margin(allowedError));
  }
}

TEST_CASE("Quaternion type aliases", "[math][quaternion]")
{
  SECTION("Float quaternions")
  {
    Quatf floatQuat{};
    REQUIRE(floatQuat.w() == 1.0F);

    static_assert(std::is_same_v<Quatf, Quaternion<f32>>);
  }

  SECTION("Double quaternions")
  {
    Quatd doubleQuat{};
    REQUIRE(doubleQuat.w() == 1.0);

    static_assert(std::is_same_v<Quatd, Quaternion<f64>>);
  }

  SECTION("Default quaternion alias")
  {
    Quat defaultQuat{};
    REQUIRE(defaultQuat.w() == 1.0F);

    // Should be float by default
    static_assert(std::is_same_v<Quat, Quatf>);
    static_assert(std::is_same_v<Quat, Quaternion<f32>>);
  }
}

TEST_CASE("Quaternion free functions", "[math][quaternion]")
{
  SECTION("Free function dot product")
  {
    Quatf firstQuat{1.0F, 2.0F, 3.0F, 4.0F};
    Quatf secondQuat{5.0F, 6.0F, 7.0F, 8.0F};

    auto dotResult{dot(firstQuat, secondQuat)};
    auto expectedValue{(1.0F * 5.0F) + (2.0F * 6.0F) + (3.0F * 7.0F) + (4.0F * 8.0F)}; // 70

    REQUIRE(dotResult == Catch::Approx(expectedValue));
  }

  SECTION("Free function normalization")
  {
    Quatf testQuat{1.0F, 2.0F, 3.0F, 4.0F};
    auto normalizedQuat{testQuat.normalized()};

    REQUIRE(normalizedQuat.length() == Catch::Approx(1.0F));
  }

  SECTION("Free function lerp and slerp")
  {
    auto firstQuat{Quatf::identity()};
    Vector<f32, 3> rotationAxis{0.0F, 0.0F, 1.0F};
    auto secondQuat{Quatf::from_axis_angle(rotationAxis, PI<f32> / 2.0F)};

    auto slerpResult{slerp(firstQuat, secondQuat, 0.5F)};

    REQUIRE(std::isfinite(slerpResult.w()));
  }
}

TEST_CASE("Quaternion edge cases and numerical stability", "[math][quaternion]")
{
  SECTION("Very small quaternion components")
  {
    constexpr f32 verySmallValue{EPSILON<f32>};
    Quatf testQuat{verySmallValue, verySmallValue, verySmallValue, verySmallValue};

    // Should normalize without issues
    auto normalizedQuat{testQuat.normalized()};
    REQUIRE(normalizedQuat.length() == Catch::Approx(1.0F));
  }

  SECTION("Very large quaternion components")
  {
    constexpr f32 veryLargeValue{EPSILON<f32>};
    Quatf testQuat{veryLargeValue, veryLargeValue, veryLargeValue, veryLargeValue};

    // Should normalize without overflow
    auto normalizedQuat{testQuat.normalized()};
    REQUIRE(normalizedQuat.length() == Catch::Approx(1.0F).margin(EPSILON<f32>));
    REQUIRE(std::isfinite(normalizedQuat.w()));
    REQUIRE(std::isfinite(normalizedQuat.x()));
    REQUIRE(std::isfinite(normalizedQuat.y()));
    REQUIRE(std::isfinite(normalizedQuat.z()));
  }

  SECTION("Nearly zero angle rotation")
  {
    Vector<f32, 3> rotationAxis{1.0F, 0.0F, 0.0F};
    f32 tinyAngle{1e-8F};

    auto rotationQuat{Quatf::from_axis_angle(rotationAxis, tinyAngle)};

    // Should be close to identity
    REQUIRE(rotationQuat.w() == Catch::Approx(1.0F).margin(EPSILON<f32>));
    REQUIRE(std::abs(rotationQuat.x()) < EPSILON<f32>);
    REQUIRE(std::abs(rotationQuat.y()) < EPSILON<f32>);
    REQUIRE(std::abs(rotationQuat.z()) < EPSILON<f32>);
  }

  SECTION("Precision preservation in multiple operations")
  {
    auto accumulatedQuat{Quatf::identity()};
    Vector<f32, 3> rotationAxis{0.0F, 1.0F, 0.0F};
    f32 smallAngle{PI<f32> / 360.0F}; // 0.5 degrees
    auto smallRotation{Quatf::from_axis_angle(rotationAxis, smallAngle)};

    for (int iterationCount{0}; iterationCount < 720; ++iterationCount)
    {
      accumulatedQuat = accumulatedQuat * smallRotation;
    }

    REQUIRE(std::abs(accumulatedQuat.w()) == Catch::Approx(1.0F).margin(0.01F));
    REQUIRE(std::abs(accumulatedQuat.x()) < 0.01F);
    REQUIRE(std::abs(accumulatedQuat.y()) < 0.01F);
    REQUIRE(std::abs(accumulatedQuat.z()) < 0.01F);
  }
}
