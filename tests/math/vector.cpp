#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <pbrt/assertions.hpp>
#include <pbrt/math/constants.hpp>
#include <pbrt/math/vector.hpp>
#include <pbrt/types.hpp>

using namespace pbrt;
using namespace pbrt::math;

TEMPLATE_TEST_CASE("Vector construction and indexing", "[math][vector][template]", u32, u64, i32,
                   i64, f32, f64)
{
  SECTION("Default constructor")
  {
    Vector<TestType, 2> v{};
    REQUIRE(v[0] == TestType{0});
    REQUIRE(v[1] == TestType{0});

    Vector<TestType, 3> v3{};
    REQUIRE(v3[0] == TestType{0});
    REQUIRE(v3[1] == TestType{0});
    REQUIRE(v3[2] == TestType{0});

    Vector<TestType, 4> v4{};
    REQUIRE(v4[0] == TestType{0});
    REQUIRE(v4[1] == TestType{0});
    REQUIRE(v4[2] == TestType{0});
    REQUIRE(v4[3] == TestType{0});
  }

  SECTION("Element access")
  {
    Vector<TestType, 2> v2{TestType{1}, TestType{2}};
    REQUIRE(v2[0] == TestType{1});
    REQUIRE(v2[1] == TestType{2});
    REQUIRE(v2[0] == v2.x());
    REQUIRE(v2[1] == v2.y());
    REQUIRE(v2[0] == v2.at(0));
    REQUIRE(v2[1] == v2.at(1));
    REQUIRE(v2[0] == v2.front());
    REQUIRE(v2[1] == v2.back());
    REQUIRE_THROWS_AS(v2.at(3), std::out_of_range);

    Vector<TestType, 3> v3{TestType{1}, TestType{2}, TestType{3}};
    REQUIRE(v3[0] == TestType{1});
    REQUIRE(v3[1] == TestType{2});
    REQUIRE(v3[2] == TestType{3});
    REQUIRE(v3[0] == v3.x());
    REQUIRE(v3[1] == v3.y());
    REQUIRE(v3[2] == v3.z());
    REQUIRE(v3[0] == v3.at(0));
    REQUIRE(v3[1] == v3.at(1));
    REQUIRE(v3[2] == v3.at(2));
    REQUIRE(v3[0] == v3.front());
    REQUIRE(v3[2] == v3.back());
    REQUIRE_THROWS_AS(v3.at(3), std::out_of_range);

    Vector<TestType, 4> v4{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    REQUIRE(v4[0] == TestType{1});
    REQUIRE(v4[1] == TestType{2});
    REQUIRE(v4[2] == TestType{3});
    REQUIRE(v4[3] == TestType{4});
    REQUIRE(v4[0] == v4.x());
    REQUIRE(v4[1] == v4.y());
    REQUIRE(v4[2] == v4.z());
    REQUIRE(v4[3] == v4.w());
    REQUIRE(v4[0] == v4.at(0));
    REQUIRE(v4[1] == v4.at(1));
    REQUIRE(v4[2] == v4.at(2));
    REQUIRE(v4[3] == v4.at(3));
    REQUIRE(v4[0] == v4.front());
    REQUIRE(v4[3] == v4.back());
    REQUIRE_THROWS_AS(v4.at(4), std::out_of_range);
  }

  SECTION("Capacity, size, maximum size and empty")
  {
    Vector<TestType, 2> v2{TestType{1}, TestType{2}};
    REQUIRE(v2.size() == 2);
    REQUIRE(v2.max_size() == 2);
    REQUIRE_FALSE(v2.empty());

    Vector<TestType, 3> v3{TestType{1}, TestType{2}, TestType{3}};
    REQUIRE(v3.size() == 3);
    REQUIRE(v3.max_size() == 3);
    REQUIRE_FALSE(v3.empty());

    Vector<TestType, 4> v4{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    REQUIRE(v4.size() == 4);
    REQUIRE(v4.max_size() == 4);
    REQUIRE_FALSE(v4.empty());
  }

  SECTION("Value constructor")
  {
    Vector<TestType, 2> v{TestType{5}};
    REQUIRE(v[0] == TestType{5});
    REQUIRE(v[1] == TestType{5});

    Vector<TestType, 3> v3{TestType{5}};
    REQUIRE(v3[0] == TestType{5});
    REQUIRE(v3[1] == TestType{5});
    REQUIRE(v3[2] == TestType{5});

    Vector<TestType, 4> v4{TestType{5}};
    REQUIRE(v4[0] == TestType{5});
    REQUIRE(v4[1] == TestType{5});
    REQUIRE(v4[2] == TestType{5});
    REQUIRE(v4[3] == TestType{5});
  }

  SECTION("Variadic constructor")
  {
    Vector<TestType, 2> v{TestType{1}, TestType{2}};
    REQUIRE(v[0] == TestType{1});
    REQUIRE(v[1] == TestType{2});

    Vector<TestType, 3> v3{TestType{1}, TestType{2}, TestType{3}};
    REQUIRE(v3[0] == TestType{1});
    REQUIRE(v3[1] == TestType{2});
    REQUIRE(v3[2] == TestType{3});

    Vector<TestType, 4> v4{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    REQUIRE(v4[0] == TestType{1});
    REQUIRE(v4[1] == TestType{2});
    REQUIRE(v4[2] == TestType{3});
    REQUIRE(v4[3] == TestType{4});
  }
}

TEMPLATE_TEST_CASE("Vector arithmetic operations", "[math][vector][template]", u32, u64, i32, i64,
                   f32, f64)
{
  Vector<TestType, 2> v1{TestType{1}, TestType{2}};
  Vector<TestType, 2> v2{TestType{3}, TestType{4}};

  SECTION("Addition")
  {
    auto result = v1 + v2;
    REQUIRE(result[0] == TestType{4});
    REQUIRE(result[1] == TestType{6});

    result = v1 + TestType{2};
    REQUIRE(result[0] == TestType{3});

    result = v1;
    result += v2;
    REQUIRE(result[0] == TestType{4});
    REQUIRE(result[1] == TestType{6});
  }

  SECTION("Subtraction")
  {
    auto result = v1 - v2;

    if constexpr (std::is_signed_v<TestType>)
    {
      REQUIRE(result[0] == TestType{-2});
      REQUIRE(result[1] == TestType{-2});

      result = v1 - TestType{2};
      REQUIRE(result[0] == TestType{-1});

      result = v1;
      result -= v2;
      REQUIRE(result[0] == TestType{-2});
      REQUIRE(result[1] == TestType{-2});
    }
    else
    {
      REQUIRE(result[0] == static_cast<TestType>(-2));
      REQUIRE(result[1] == static_cast<TestType>(-2));

      result = v1 - TestType{2};
      REQUIRE(result[0] == static_cast<TestType>(-1));

      result = v1;
      result -= v2;
      REQUIRE(result[0] == static_cast<TestType>(-2));
      REQUIRE(result[1] == static_cast<TestType>(-2));
    }
  }

  SECTION("Scalar multiplication")
  {
    auto result = v1 * TestType{2};
    REQUIRE(result[0] == TestType{2});
    REQUIRE(result[1] == TestType{4});

    result = TestType{2} * v1;
    REQUIRE(result[0] == TestType{2});
    REQUIRE(result[1] == TestType{4});

    result = v1 * TestType{2};
    REQUIRE(result[0] == TestType{2});
    REQUIRE(result[1] == TestType{4});

    result = v1;
    result *= TestType{2};
    REQUIRE(result[0] == TestType{2});
    REQUIRE(result[1] == TestType{4});
  }

  if constexpr (std::is_floating_point_v<TestType>)
  {
    SECTION("Scalar division")
    {
      auto result = v1 / TestType{2};
      REQUIRE(result[0] == Catch::Approx(0.5));
      REQUIRE(result[1] == Catch::Approx(1));

      result = v1;
      result /= TestType{2};
      REQUIRE(result[0] == Catch::Approx(0.5));
      REQUIRE(result[1] == Catch::Approx(1));
    }
  }

  SECTION("Dot product, length and normalize")
  {
    using namespace Catch;

    Vector<double, 2> v{3.0, 4.0};
    REQUIRE(v.dot(v) == Approx(25.0));
    REQUIRE(v.length_squared() == Approx(25.0));
    REQUIRE(v.length() == Approx(5.0));

    auto vNormalizedCopy = v.normalized();
    REQUIRE(vNormalizedCopy[0] == Approx(3.0 / 5.0));
    REQUIRE(vNormalizedCopy[1] == Approx(4.0 / 5.0));
    REQUIRE(vNormalizedCopy.length() == Approx(1.0));
    REQUIRE(v[0] == Approx(3.0));

    v.normalize();
    REQUIRE(v[0] == Approx(3.0 / 5.0));
    REQUIRE(v[1] == Approx(4.0 / 5.0));
    REQUIRE(v.length() == Approx(1.0));
  }

  SECTION("Comparison")
  {
    Vector<TestType, 3> x{1, 2, 3};
    Vector<TestType, 3> y{1, 2, 3};
    Vector<TestType, 3> z{3, 2, 1};

    REQUIRE(x == y);
    REQUIRE(x != z);
  }

  SECTION("Cross product")
  {
    Vector<float, 3> i{1, 0, 0};
    Vector<float, 3> j{0, 1, 0};
    Vector<float, 3> k{0, 0, 1};

    REQUIRE(cross(i, j) == k);
    REQUIRE(cross(j, k) == i);
    REQUIRE(cross(k, i) == j);
    REQUIRE(cross(i, i) == Vector<float, 3>::zero());
  }

  SECTION("Iterators and data")
  {
    Vector<int, 5> v{1, 2, 3, 4, 5};
    int sum{0};
    for (auto &e : v)
    {
      sum += e;
    }
    REQUIRE(sum == 15);

    sum = 0;
    for (int it : v)
    {
      sum += it;
    }
    REQUIRE(sum == 15);

    const auto *pData{v.data()};
    for (usize i = 0; i < v.size(); ++i)
    {
      REQUIRE(pData[i] == v[i]); // NOLINT
    }
  }

  SECTION("Static factories: zero, one, unit_x/y/z/w")
  {
    REQUIRE(Vector<TestType, 2>::zero() == Vector<TestType, 2>{0, 0});
    REQUIRE(Vector<TestType, 2>::one() == Vector<TestType, 2>{1, 1});
    REQUIRE(Vector<TestType, 2>::unit_x() == Vector<TestType, 2>{1, 0});
    REQUIRE(Vector<TestType, 2>::unit_y() == Vector<TestType, 2>{0, 1});
    REQUIRE(Vector<TestType, 3>::unit_z() == Vector<TestType, 3>{0, 0, 1});
    REQUIRE(Vector<TestType, 4>::unit_w() == Vector<TestType, 4>{0, 0, 0, 1});
  }
}

TEMPLATE_TEST_CASE("Vector floating point features", "[math][vector][template]", f32, f64)
{
  using namespace Catch;

  using Vec2T = Vector<TestType, 2>;
  using Vec3T = Vector<TestType, 3>;

  SECTION("Approximate equality")
  {
    Vec2T v2{static_cast<TestType>(1.0), static_cast<TestType>(2.0)};

    if constexpr (std::is_same_v<TestType, f32>)
    {
      Vec2T v1{static_cast<TestType>(1.0), static_cast<TestType>(2.0001)};

      REQUIRE_FALSE(v1 == v2);
      REQUIRE(v1.approx_equal(v2, pbrt::math::LOOSE_EPSILON<TestType>));
      REQUIRE_FALSE(v1.approx_equal(v2, pbrt::math::EPSILON<TestType>));
    }
    else
    {
      Vec2T v1{static_cast<TestType>(1.0), static_cast<TestType>(2.0000001)};

      REQUIRE_FALSE(v1 == v2);
      REQUIRE(v1.approx_equal(v2, pbrt::math::EPSILON<TestType>));
      REQUIRE_FALSE(v1.approx_equal(v2, pbrt::math::ZERO_EPSILON<TestType>));
    }
  }

  SECTION("Distance and DistanceSquared (member and free functions)")
  {
    Vec2T p1{1, 1};
    Vec2T p2{4, 5};

    REQUIRE(p1.distance_squared(p2) == Approx(static_cast<TestType>(25.0)));
    REQUIRE(p2.distance_squared(p1) == Approx(static_cast<TestType>(25.0)));
    REQUIRE(p1.distance(p2) == Approx(static_cast<TestType>(5.0)));
    REQUIRE(p2.distance(p1) == Approx(static_cast<TestType>(5.0)));

    REQUIRE(distance_squared(p1, p2) == Approx(static_cast<TestType>(25.0)));
    REQUIRE(distance(p1, p2) == Approx(static_cast<TestType>(5.0)));
  }

  SECTION("Length and LengthSquared (free functions)")
  {
    Vec2T v{3, 4};
    REQUIRE(length_squared(v) == Approx(static_cast<TestType>(25.0)));
    REQUIRE(length(v) == Approx(static_cast<TestType>(5.0)));
  }

  SECTION("Safe Normalization (member and free functions)")
  {
    Vec3T v{2, 0, 0};
    Vec3T fallback{1, 2, 3};

    auto snMem = v.safe_normalized(fallback);
    REQUIRE(snMem.x() == Approx(static_cast<TestType>(1.0)));
    REQUIRE(snMem.y() == Approx(static_cast<TestType>(0.0)));
    REQUIRE(snMem.z() == Approx(static_cast<TestType>(0.0)));

    auto snFree = safe_normalized(v, fallback);
    REQUIRE(snFree.x() == Approx(static_cast<TestType>(1.0)));
    REQUIRE(snFree.y() == Approx(static_cast<TestType>(0.0)));
    REQUIRE(snFree.z() == Approx(static_cast<TestType>(0.0)));

    Vec3T zeroVec = Vec3T::zero();
    auto snZeroMem = zeroVec.safe_normalized(fallback);
    REQUIRE(snZeroMem == fallback);

    auto snZeroFree = safe_normalized(zeroVec, fallback);
    REQUIRE(snZeroFree == fallback);

    auto snZeroDefaultFb = zeroVec.safe_normalized();
    REQUIRE(snZeroDefaultFb == Vec3T::unit_x());
  }

  SECTION("Projection (member)")
  {
    Vec2T a{2, 2};
    Vec2T b{3, 0};
    Vec2T projAOnB = a.project_onto(b);
    REQUIRE(projAOnB.x() == Approx(static_cast<TestType>(2.0)));
    REQUIRE(projAOnB.y() == Approx(static_cast<TestType>(0.0)));
  }

  SECTION("Reflection (member and free functions)")
  {
    Vec2T incident{-1, -1};
    Vec2T normal{0, 1};
    Vec2T reflectedMem = incident.reflect(normal);
    REQUIRE(reflectedMem.x() == Approx(static_cast<TestType>(-1.0)));
    REQUIRE(reflectedMem.y() == Approx(static_cast<TestType>(1.0)));

    Vec2T reflectedFree = reflect(incident, normal);
    REQUIRE(reflectedFree.x() == Approx(static_cast<TestType>(-1.0)));
    REQUIRE(reflectedFree.y() == Approx(static_cast<TestType>(1.0)));
  }

  SECTION("Refraction (member and free functions)")
  {
    Vec3T incident = Vec3T(1, -1, 0).normalized();
    Vec3T normal(0, 1, 0);
    auto eta{static_cast<TestType>(1.0 / 1.5)};

    Vec3T refractedMem{incident.refract(normal, eta)};
    Vec3T refractedFree{refract(incident, normal, eta)};

    REQUIRE(refractedMem.x() == Approx(static_cast<TestType>(0.47140452)));
    REQUIRE(refractedMem.y() == Approx(static_cast<TestType>(-0.8819171)));
    REQUIRE(refractedMem.z() == Approx(static_cast<TestType>(0.0)));
    REQUIRE(refractedMem == refractedFree);

    Vec3T incidentTir{Vec3T(0.8, -0.2, 0).normalized()};
    auto etaTir{static_cast<TestType>(1.5)};
    Vec3T refractedTirMem{incidentTir.refract(normal, etaTir)};
    REQUIRE(refractedTirMem == Vec3T::zero());
  }

  SECTION("Lerp (member and free functions)")
  {
    Vec2T vStart{0, 0};
    Vec2T vEnd{10, 20};
    REQUIRE(vStart.lerp(vEnd, static_cast<TestType>(0.0)) == vStart);
    REQUIRE(vStart.lerp(vEnd, static_cast<TestType>(1.0)) == vEnd);
    REQUIRE(vStart.lerp(vEnd, static_cast<TestType>(0.5)) == Vec2T(5, 10));

    REQUIRE(lerp(vStart, vEnd, static_cast<TestType>(0.25)) == Vec2T(2.5, 5.0));
  }

  SECTION("Component-wise: min_component, max_component, abs, clamp")
  {
    Vec3T v{-1, 2, -3};
    REQUIRE(v.min_component() == Approx(static_cast<TestType>(-3.0)));
    REQUIRE(v.max_component() == Approx(static_cast<TestType>(2.0)));

    Vec3T vAbs = v.abs();
    REQUIRE(vAbs.x() == Approx(static_cast<TestType>(1.0)));
    REQUIRE(vAbs.y() == Approx(static_cast<TestType>(2.0)));
    REQUIRE(vAbs.z() == Approx(static_cast<TestType>(3.0)));

    Vec3T clampedScalar = v.clamp(static_cast<TestType>(-2.0), static_cast<TestType>(1.5));
    REQUIRE(clampedScalar.x() == Approx(static_cast<TestType>(-1.0)));
    REQUIRE(clampedScalar.y() == Approx(static_cast<TestType>(1.5)));
    REQUIRE(clampedScalar.z() == Approx(static_cast<TestType>(-2.0)));

    Vec3T minV{-1.5, 0, 0.5};
    Vec3T maxV{1.5, 1, 2.5};
    Vec3T clampedVec = v.clamp(minV, maxV);
    REQUIRE(clampedVec.x() == Approx(static_cast<TestType>(-1.0)));
    REQUIRE(clampedVec.y() == Approx(static_cast<TestType>(1.0)));
    REQUIRE(clampedVec.z() == Approx(static_cast<TestType>(0.5)));
  }

  SECTION("Min/Max Dimension")
  {
    Vec3T v1{10, 0, 20};
    REQUIRE(static_cast<usize>(v1.min_dimension()) == 1);
    REQUIRE(static_cast<usize>(v1.max_dimension()) == 2);

    Vec3T v2{-5, -10, -2};
    REQUIRE(static_cast<usize>(v2.min_dimension()) == 1);
    REQUIRE(static_cast<usize>(v2.max_dimension()) == 2);
  }

  SECTION("Free functions: min, max (vector-wise)")
  {
    Vec2T a{1, 5};
    Vec2T b{3, 2};

    Vec2T minAb = min(a, b);
    Vec2T maxAb = max(a, b);

    REQUIRE(minAb.x() == Approx(static_cast<TestType>(1.0)));
    REQUIRE(minAb.y() == Approx(static_cast<TestType>(2.0)));
    REQUIRE(maxAb.x() == Approx(static_cast<TestType>(3.0)));
    REQUIRE(maxAb.y() == Approx(static_cast<TestType>(5.0)));
  }

  SECTION("Swizzling free functions: xy, xz, yz, xyz")
  {
    Vec3T v3{1, 2, 3};
    REQUIRE(xy(v3) == Vec2T(1, 2));
    REQUIRE(xz(v3) == Vec2T(1, 3));
    REQUIRE(yz(v3) == Vec2T(2, 3));

    Vector<TestType, 4> v4{1, 2, 3, 4};
    REQUIRE(xyz(v4) == Vec3T(1, 2, 3));
  }

  SECTION("Construction from Vector of different (convertible) type")
  {
    if constexpr (std::is_same_v<TestType, float>)
    {
      Vector<f32, 3> vDouble{1.1, 2.2, 3.3};
      Vector<f64, 3> vFloat(vDouble);
      REQUIRE(vFloat[0] == Approx(1.1F));
      REQUIRE(vFloat[1] == Approx(2.2F));
      REQUIRE(vFloat[2] == Approx(3.3F));
    }
    else if constexpr (std::is_same_v<TestType, double>)
    {
      Vector<float, 3> vFloat{1.1F, 2.2F, 3.3F};
      Vector<double, 3> vDouble(vFloat);
      REQUIRE(vDouble[0] == Approx(static_cast<double>(1.1F)));
      REQUIRE(vDouble[1] == Approx(static_cast<double>(2.2F)));
      REQUIRE(vDouble[2] == Approx(static_cast<double>(3.3F)));
    }

    Vector<int, 2> vInt{5, 10};
    Vector<TestType, 2> vTestType(vInt);
    REQUIRE(vTestType[0] == Approx(static_cast<TestType>(5)));
    REQUIRE(vTestType[1] == Approx(static_cast<TestType>(10)));
  }
}
