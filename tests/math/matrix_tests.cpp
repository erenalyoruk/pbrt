#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <pbrt/assertions.hpp>
#include <pbrt/math/constants.hpp>
#include <pbrt/math/matrix.hpp>
#include <pbrt/types.hpp>

using namespace pbrt;
using namespace pbrt::math;

TEMPLATE_TEST_CASE("Matrix construction and element access", "[math][matrix][template]", u32, u64,
                   i32, i64, f32, f64)
{
  SECTION("Default constructor")
  {
    Matrix<TestType, 2, 2> m2x2{};
    REQUIRE(m2x2(0, 0) == TestType{0});
    REQUIRE(m2x2(0, 1) == TestType{0});
    REQUIRE(m2x2(1, 0) == TestType{0});
    REQUIRE(m2x2(1, 1) == TestType{0});

    Matrix<TestType, 3, 3> m3x3{};
    for (usize i = 0; i < 3; ++i)
    {
      for (usize j = 0; j < 3; ++j)
      {
        REQUIRE(m3x3(i, j) == TestType{0});
      }
    }

    Matrix<TestType, 4, 4> m4x4{};
    for (usize i = 0; i < 4; ++i)
    {
      for (usize j = 0; j < 4; ++j)
      {
        REQUIRE(m4x4(i, j) == TestType{0});
      }
    }
  }

  SECTION("Variadic constructor")
  {
    Matrix<TestType, 2, 2> m2x2{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    REQUIRE(m2x2(0, 0) == TestType{1});
    REQUIRE(m2x2(0, 1) == TestType{2});
    REQUIRE(m2x2(1, 0) == TestType{3});
    REQUIRE(m2x2(1, 1) == TestType{4});

    Matrix<TestType, 2, 3> m2x3{TestType{1}, TestType{2}, TestType{3},
                                TestType{4}, TestType{5}, TestType{6}};
    REQUIRE(m2x3(0, 0) == TestType{1});
    REQUIRE(m2x3(0, 1) == TestType{2});
    REQUIRE(m2x3(0, 2) == TestType{3});
    REQUIRE(m2x3(1, 0) == TestType{4});
    REQUIRE(m2x3(1, 1) == TestType{5});
    REQUIRE(m2x3(1, 2) == TestType{6});

    Matrix<TestType, 4, 4> m4x4{TestType{1},  TestType{2},  TestType{3},  TestType{4},
                                TestType{5},  TestType{6},  TestType{7},  TestType{8},
                                TestType{9},  TestType{10}, TestType{11}, TestType{12},
                                TestType{13}, TestType{14}, TestType{15}, TestType{16}};
    REQUIRE(m4x4(0, 0) == TestType{1});
    REQUIRE(m4x4(3, 3) == TestType{16});
  }

  SECTION("Fill constructor")
  {
    Matrix<TestType, 3, 3> m3x3{TestType{5}};
    for (usize i = 0; i < 3; ++i)
    {
      for (usize j = 0; j < 3; ++j)
      {
        REQUIRE(m3x3(i, j) == TestType{5});
      }
    }

    Matrix<TestType, 4, 4> m4x4{TestType{7}};
    for (usize i = 0; i < 4; ++i)
    {
      for (usize j = 0; j < 4; ++j)
      {
        REQUIRE(m4x4(i, j) == TestType{7});
      }
    }
  }

  SECTION("Element access with operator()")
  {
    Matrix<TestType, 2, 2> m{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    REQUIRE(m(0, 0) == TestType{1});
    REQUIRE(m(0, 1) == TestType{2});
    REQUIRE(m(1, 0) == TestType{3});
    REQUIRE(m(1, 1) == TestType{4});

    // Test modification
    m(0, 0) = TestType{10};
    REQUIRE(m(0, 0) == TestType{10});

    Matrix<TestType, 4, 4> m4x4 = Matrix<TestType, 4, 4>::identity();
    REQUIRE(m4x4(0, 0) == TestType{1});
    REQUIRE(m4x4(1, 1) == TestType{1});
    REQUIRE(m4x4(0, 1) == TestType{0});
  }

  SECTION("Access with at()")
  {
    Matrix<TestType, 2, 2> m{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    REQUIRE(m.at(0, 0) == TestType{1});
    REQUIRE(m.at(1, 1) == TestType{4});

    Matrix<TestType, 4, 4> m4x4{TestType{1},  TestType{2},  TestType{3},  TestType{4},
                                TestType{5},  TestType{6},  TestType{7},  TestType{8},
                                TestType{9},  TestType{10}, TestType{11}, TestType{12},
                                TestType{13}, TestType{14}, TestType{15}, TestType{16}};
    REQUIRE(m4x4.at(2, 1) == TestType{10});
    REQUIRE(m4x4.at(3, 3) == TestType{16});
  }

  SECTION("Row and column access")
  {
    Matrix<TestType, 2, 3> m{TestType{1}, TestType{2}, TestType{3},
                             TestType{4}, TestType{5}, TestType{6}};

    auto row0 = m.row(0);
    REQUIRE(row0[0] == TestType{1});
    REQUIRE(row0[1] == TestType{2});
    REQUIRE(row0[2] == TestType{3});

    auto col0 = m.column(0);
    REQUIRE(col0[0] == TestType{1});
    REQUIRE(col0[1] == TestType{4});

    Matrix<TestType, 4, 4> m4x4{TestType{1},  TestType{2},  TestType{3},  TestType{4},
                                TestType{5},  TestType{6},  TestType{7},  TestType{8},
                                TestType{9},  TestType{10}, TestType{11}, TestType{12},
                                TestType{13}, TestType{14}, TestType{15}, TestType{16}};

    auto row2 = m4x4.row(2);
    REQUIRE(row2[0] == TestType{9});
    REQUIRE(row2[3] == TestType{12});

    auto col3 = m4x4.column(3);
    REQUIRE(col3[0] == TestType{4});
    REQUIRE(col3[3] == TestType{16});
  }

  SECTION("Size and capacity")
  {
    Matrix<TestType, 2, 3> m{};
    REQUIRE(m.size() == 6);
    REQUIRE(m.max_size() == 6);
    REQUIRE(m.rows() == 2);
    REQUIRE(m.columns() == 3);
    REQUIRE_FALSE(m.empty());

    Matrix<TestType, 4, 4> m4x4{};
    REQUIRE(m4x4.size() == 16);
    REQUIRE(m4x4.max_size() == 16);
    REQUIRE(m4x4.rows() == 4);
    REQUIRE(m4x4.columns() == 4);
    REQUIRE_FALSE(m4x4.empty());
  }

  SECTION("Iterator access")
  {
    Matrix<TestType, 2, 2> m{TestType{1}, TestType{2}, TestType{3}, TestType{4}};

    TestType sum{0};
    for (auto it = m.begin(); it != m.end(); ++it)
    {
      sum += *it;
    }
    REQUIRE(sum == TestType{10});

    // Range-based for loop
    sum = TestType{0};
    for (const auto &val : m)
    {
      sum += val;
    }
    REQUIRE(sum == TestType{10});

    // Test with 4x4 matrix
    Matrix<TestType, 4, 4> m4x4 = Matrix<TestType, 4, 4>::diagonal(TestType{2});
    TestType sum4x4{0};
    for (const auto &val : m4x4)
    {
      sum4x4 += val;
    }
    REQUIRE(sum4x4 == TestType{8}); // Four 2s on diagonal
  }

  SECTION("Data pointer access")
  {
    Matrix<TestType, 2, 2> m{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    const auto *data = m.data();
    REQUIRE(data[0] == TestType{1}); // NOLINT
    REQUIRE(data[1] == TestType{2}); // NOLINT
    REQUIRE(data[2] == TestType{3}); // NOLINT
    REQUIRE(data[3] == TestType{4}); // NOLINT

    Matrix<TestType, 4, 4> m4x4 = Matrix<TestType, 4, 4>::identity();
    const auto *data4x4 = m4x4.data();
    REQUIRE(data4x4[0] == TestType{1}); // NOLINT
    REQUIRE(data4x4[5] == TestType{1}); // NOLINT (1,1)
    REQUIRE(data4x4[1] == TestType{0}); // NOLINT (0,1)
  }
}

TEMPLATE_TEST_CASE("Matrix arithmetic operations", "[math][matrix][template]", u32, u64, i32, i64,
                   f32, f64)
{
  Matrix<TestType, 2, 2> m1{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
  Matrix<TestType, 2, 2> m2{TestType{5}, TestType{6}, TestType{7}, TestType{8}};

  SECTION("Matrix addition")
  {
    auto result = m1 + m2;
    REQUIRE(result(0, 0) == TestType{6});
    REQUIRE(result(0, 1) == TestType{8});
    REQUIRE(result(1, 0) == TestType{10});
    REQUIRE(result(1, 1) == TestType{12});

    // Compound assignment
    Matrix<TestType, 2, 2> m3 = m1;
    m3 += m2;
    REQUIRE(m3(0, 0) == TestType{6});
    REQUIRE(m3(1, 1) == TestType{12});

    // 4x4 matrix addition
    Matrix<TestType, 4, 4> m41 = Matrix<TestType, 4, 4>::identity();
    Matrix<TestType, 4, 4> m42 = Matrix<TestType, 4, 4>::diagonal(TestType{3});
    auto result4x4 = m41 + m42;

    REQUIRE(result4x4(0, 0) == TestType{4}); // 1 + 3
    REQUIRE(result4x4(1, 1) == TestType{4}); // 1 + 3
    REQUIRE(result4x4(0, 1) == TestType{0}); // 0 + 0
  }

  SECTION("Scalar addition")
  {
    auto result = m1 + TestType{10};
    REQUIRE(result(0, 0) == TestType{11});
    REQUIRE(result(0, 1) == TestType{12});
    REQUIRE(result(1, 0) == TestType{13});
    REQUIRE(result(1, 1) == TestType{14});

    Matrix<TestType, 2, 2> m3 = m1;
    m3 += TestType{10};
    REQUIRE(m3(0, 0) == TestType{11});
  }

  SECTION("Matrix subtraction")
  {
    auto result = m2 - m1;
    REQUIRE(result(0, 0) == TestType{4});
    REQUIRE(result(0, 1) == TestType{4});
    REQUIRE(result(1, 0) == TestType{4});
    REQUIRE(result(1, 1) == TestType{4});

    Matrix<TestType, 2, 2> m3 = m2;
    m3 -= m1;
    REQUIRE(m3(0, 0) == TestType{4});

    // 4x4 matrix subtraction
    Matrix<TestType, 4, 4> m41 = Matrix<TestType, 4, 4>::diagonal(TestType{5});
    Matrix<TestType, 4, 4> m42 = Matrix<TestType, 4, 4>::diagonal(TestType{2});
    auto result4x4 = m41 - m42;

    REQUIRE(result4x4(0, 0) == TestType{3}); // 5 - 2
    REQUIRE(result4x4(1, 1) == TestType{3}); // 5 - 2
    REQUIRE(result4x4(0, 1) == TestType{0}); // 0 - 0
  }

  SECTION("Scalar subtraction")
  {
    auto result = m1 - TestType{1};
    REQUIRE(result(0, 0) == TestType{0});
    REQUIRE(result(0, 1) == TestType{1});
    REQUIRE(result(1, 0) == TestType{2});
    REQUIRE(result(1, 1) == TestType{3});
  }

  SECTION("Unary negation")
  {
    auto result = -m1;
    if constexpr (std::is_signed_v<TestType>)
    {
      REQUIRE(result(0, 0) == TestType{-1});
      REQUIRE(result(0, 1) == TestType{-2});
      REQUIRE(result(1, 0) == TestType{-3});
      REQUIRE(result(1, 1) == TestType{-4});
    }
  }

  SECTION("Scalar multiplication")
  {
    auto result = m1 * TestType{2};
    REQUIRE(result(0, 0) == TestType{2});
    REQUIRE(result(0, 1) == TestType{4});
    REQUIRE(result(1, 0) == TestType{6});
    REQUIRE(result(1, 1) == TestType{8});

    // Left multiplication
    result = TestType{3} * m1;
    REQUIRE(result(0, 0) == TestType{3});
    REQUIRE(result(1, 1) == TestType{12});

    Matrix<TestType, 2, 2> m3 = m1;
    m3 *= TestType{2};
    REQUIRE(m3(0, 0) == TestType{2});

    // 4x4 scalar multiplication
    Matrix<TestType, 4, 4> m4x4 = Matrix<TestType, 4, 4>::diagonal(TestType{2});
    m4x4 *= TestType{3};
    REQUIRE(m4x4(0, 0) == TestType{6}); // 2 * 3
    REQUIRE(m4x4(1, 1) == TestType{6}); // 2 * 3
    REQUIRE(m4x4(0, 1) == TestType{0}); // 0 * 3
  }

  if constexpr (std::is_floating_point_v<TestType>)
  {
    SECTION("Scalar division")
    {
      auto result = m1 / TestType{2};
      REQUIRE(result(0, 0) == Catch::Approx(0.5));
      REQUIRE(result(0, 1) == Catch::Approx(1.0));
      REQUIRE(result(1, 0) == Catch::Approx(1.5));
      REQUIRE(result(1, 1) == Catch::Approx(2.0));

      Matrix<TestType, 2, 2> m3 = m1;
      m3 /= TestType{2};
      REQUIRE(m3(0, 0) == Catch::Approx(0.5));

      // Division by zero should not throw but return zero matrix
      REQUIRE_NOTHROW(m1 / TestType{0});
    }
  }

  SECTION("Comparison operators")
  {
    Matrix<TestType, 2, 2> m3 = m1;
    REQUIRE(m1 == m3);
    REQUIRE_FALSE(m1 == m2);
    REQUIRE(m1 != m2);
    REQUIRE_FALSE(m1 != m3);

    // 4x4 matrix comparison
    Matrix<TestType, 4, 4> m41 = Matrix<TestType, 4, 4>::identity();
    Matrix<TestType, 4, 4> m42 = Matrix<TestType, 4, 4>::identity();
    Matrix<TestType, 4, 4> m43 = Matrix<TestType, 4, 4>::diagonal(TestType{2});

    REQUIRE(m41 == m42);
    REQUIRE_FALSE(m41 == m43);
    REQUIRE(m41 != m43);
  }
}

// SIMD-specific tests for f32 4x4 matrices
TEST_CASE("SIMD f32 4x4 matrix operations", "[math][matrix][simd]")
{
  using Mat4f = Matrix<f32, 4, 4>;

  SECTION("SIMD Matrix addition")
  {
    Mat4f m1{1.0F, 2.0F,  3.0F,  4.0F,  5.0F,  6.0F,  7.0F,  8.0F,
             9.0F, 10.0F, 11.0F, 12.0F, 13.0F, 14.0F, 15.0F, 16.0F};

    Mat4f m2{16.0F, 15.0F, 14.0F, 13.0F, 12.0F, 11.0F, 10.0F, 9.0F,
             8.0F,  7.0F,  6.0F,  5.0F,  4.0F,  3.0F,  2.0F,  1.0F};

    Mat4f result = m1;
    result += m2;

    // All elements should be 17
    for (usize i = 0; i < 4; ++i)
    {
      for (usize j = 0; j < 4; ++j)
      {
        REQUIRE(result(i, j) == Catch::Approx(17.0F));
      }
    }
  }

  SECTION("SIMD Matrix subtraction")
  {
    Mat4f m1{10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F,
             10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F};

    Mat4f m2{1.0F, 2.0F, 3.0F, 4.0F, 1.0F, 2.0F, 3.0F, 4.0F,
             1.0F, 2.0F, 3.0F, 4.0F, 1.0F, 2.0F, 3.0F, 4.0F};

    Mat4f result = m1;
    result -= m2;

    REQUIRE(result(0, 0) == Catch::Approx(9.0F));
    REQUIRE(result(0, 1) == Catch::Approx(8.0F));
    REQUIRE(result(0, 2) == Catch::Approx(7.0F));
    REQUIRE(result(0, 3) == Catch::Approx(6.0F));
    REQUIRE(result(3, 0) == Catch::Approx(9.0F));
    REQUIRE(result(3, 3) == Catch::Approx(6.0F));
  }

  SECTION("SIMD Scalar multiplication")
  {
    Mat4f m1 = Mat4f::diagonal(2.0F);
    m1 *= 3.5F;

    REQUIRE(m1(0, 0) == Catch::Approx(7.0F)); // 2.0 * 3.5
    REQUIRE(m1(1, 1) == Catch::Approx(7.0F));
    REQUIRE(m1(2, 2) == Catch::Approx(7.0F));
    REQUIRE(m1(3, 3) == Catch::Approx(7.0F));
    REQUIRE(m1(0, 1) == Catch::Approx(0.0F)); // 0.0 * 3.5
  }

  SECTION("SIMD Matrix-vector multiplication")
  {
    Mat4f m = Mat4f::identity();
    Vector<f32, 4> v{1.0F, 2.0F, 3.0F, 4.0F};

    Vector<f32, 4> result = m * v;

    REQUIRE(result[0] == Catch::Approx(1.0F));
    REQUIRE(result[1] == Catch::Approx(2.0F));
    REQUIRE(result[2] == Catch::Approx(3.0F));
    REQUIRE(result[3] == Catch::Approx(4.0F));

    // Test with non-identity matrix
    Mat4f m2{2.0F, 0.0F, 0.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F,
             0.0F, 0.0F, 4.0F, 0.0F, 0.0F, 0.0F, 0.0F, 5.0F};

    Vector<f32, 4> result2 = m2 * v;

    REQUIRE(result2[0] == Catch::Approx(2.0F));  // 2 * 1
    REQUIRE(result2[1] == Catch::Approx(6.0F));  // 3 * 2
    REQUIRE(result2[2] == Catch::Approx(12.0F)); // 4 * 3
    REQUIRE(result2[3] == Catch::Approx(20.0F)); // 5 * 4
  }

  SECTION("SIMD Matrix equality comparison")
  {
    Mat4f m1 = Mat4f::identity();
    Mat4f m2 = Mat4f::identity();
    Mat4f m3 = Mat4f::diagonal(2.0F);

    REQUIRE(m1 == m2);
    REQUIRE_FALSE(m1 == m3);

    // Test with small differences
    Mat4f m4 = m1;
    m4(2, 2) = 1.0001F;
    REQUIRE_FALSE(m1 == m4);
  }

  SECTION("SIMD Matrix transpose")
  {
    Mat4f m{1.0F, 2.0F,  3.0F,  4.0F,  5.0F,  6.0F,  7.0F,  8.0F,
            9.0F, 10.0F, 11.0F, 12.0F, 13.0F, 14.0F, 15.0F, 16.0F};

    Mat4f transposed = m.transpose();

    REQUIRE(transposed(0, 0) == Catch::Approx(1.0F));
    REQUIRE(transposed(0, 1) == Catch::Approx(5.0F));
    REQUIRE(transposed(0, 2) == Catch::Approx(9.0F));
    REQUIRE(transposed(0, 3) == Catch::Approx(13.0F));
    REQUIRE(transposed(1, 0) == Catch::Approx(2.0F));
    REQUIRE(transposed(3, 3) == Catch::Approx(16.0F));

    // Test transpose property: (A^T)^T = A
    Mat4f doubleTransposed = transposed.transpose();
    REQUIRE(doubleTransposed == m);
  }

  SECTION("SIMD Complex matrix operations")
  {
    Mat4f rotation{0.0F, -1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F,
                   0.0F, 0.0F,  1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F};

    Vector<f32, 4> point{1.0F, 0.0F, 0.0F, 1.0F};
    Vector<f32, 4> rotated = rotation * point;

    REQUIRE(rotated[0] == Catch::Approx(0.0F).margin(EPSILON<f32>));
    REQUIRE(rotated[1] == Catch::Approx(1.0F));
    REQUIRE(rotated[2] == Catch::Approx(0.0F));
    REQUIRE(rotated[3] == Catch::Approx(1.0F));
  }
}

// SIMD-specific tests for f64 4x4 matrices (AVX)
TEST_CASE("SIMD f64 4x4 matrix operations", "[math][matrix][simd][avx]")
{
  using Mat4d = Matrix<f64, 4, 4>;

  SECTION("AVX Matrix addition")
  {
    Mat4d m1{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};

    Mat4d m2 = Mat4d::diagonal(0.5);

    Mat4d result = m1;
    result += m2;

    REQUIRE(result(0, 0) == Catch::Approx(1.5));  // 1.0 + 0.5
    REQUIRE(result(1, 1) == Catch::Approx(6.5));  // 6.0 + 0.5
    REQUIRE(result(2, 2) == Catch::Approx(11.5)); // 11.0 + 0.5
    REQUIRE(result(3, 3) == Catch::Approx(16.5)); // 16.0 + 0.5
    REQUIRE(result(0, 1) == Catch::Approx(2.0));  // 2.0 + 0.0
  }

  SECTION("AVX Matrix-vector multiplication")
  {
    Mat4d m{2.0, 0.0, 0.0, 1.0, 0.0, 2.0, 0.0, 2.0, 0.0, 0.0, 2.0, 3.0, 0.0, 0.0, 0.0, 1.0};

    Vector<f64, 4> v{1.0, 1.0, 1.0, 1.0};
    Vector<f64, 4> result = m * v;

    REQUIRE(result[0] == Catch::Approx(3.0)); // 2*1 + 0*1 + 0*1 + 1*1
    REQUIRE(result[1] == Catch::Approx(4.0)); // 0*1 + 2*1 + 0*1 + 2*1
    REQUIRE(result[2] == Catch::Approx(5.0)); // 0*1 + 0*1 + 2*1 + 3*1
    REQUIRE(result[3] == Catch::Approx(1.0)); // 0*1 + 0*1 + 0*1 + 1*1
  }

  SECTION("AVX Matrix equality comparison")
  {
    Mat4d m1 = Mat4d::identity();
    Mat4d m2 = Mat4d::identity();
    Mat4d m3 = Mat4d::diagonal(1.00001);

    REQUIRE(m1 == m2);
    REQUIRE_FALSE(m1 == m3);

    // Test with precision differences
    Mat4d m4 = m1;
    m4(1, 1) = 1.0 + 1e-15; // Very small difference
    REQUIRE_FALSE(m1 == m4);
  }
}

TEMPLATE_TEST_CASE("Matrix-vector multiplication", "[math][matrix][template]", f32, f64)
{
  SECTION("2x2 matrix-vector multiplication")
  {
    Matrix<TestType, 2, 2> m{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    Vector<TestType, 2> v{TestType{5}, TestType{6}};

    auto result = m * v;
    REQUIRE(result[0] == Catch::Approx(static_cast<TestType>(17))); // 1*5 + 2*6
    REQUIRE(result[1] == Catch::Approx(static_cast<TestType>(39))); // 3*5 + 4*6
  }

  SECTION("3x3 matrix-vector multiplication")
  {
    Matrix<TestType, 3, 3> m{TestType{1}, TestType{2}, TestType{3}, TestType{4}, TestType{5},
                             TestType{6}, TestType{7}, TestType{8}, TestType{9}};
    Vector<TestType, 3> v{TestType{1}, TestType{2}, TestType{3}};

    auto result = m * v;
    REQUIRE(result[0] == Catch::Approx(static_cast<TestType>(14))); // 1*1 + 2*2 + 3*3
    REQUIRE(result[1] == Catch::Approx(static_cast<TestType>(32))); // 4*1 + 5*2 + 6*3
    REQUIRE(result[2] == Catch::Approx(static_cast<TestType>(50))); // 7*1 + 8*2 + 9*3
  }

  SECTION("4x4 matrix-vector multiplication")
  {
    Matrix<TestType, 4, 4> m = Matrix<TestType, 4, 4>::identity();
    Vector<TestType, 4> v{TestType{1}, TestType{2}, TestType{3}, TestType{4}};

    auto result = m * v;
    REQUIRE(result[0] == Catch::Approx(static_cast<TestType>(1)));
    REQUIRE(result[1] == Catch::Approx(static_cast<TestType>(2)));
    REQUIRE(result[2] == Catch::Approx(static_cast<TestType>(3)));
    REQUIRE(result[3] == Catch::Approx(static_cast<TestType>(4)));

    // Test with transformation matrix
    Matrix<TestType, 4, 4> transform{TestType{2}, TestType{0}, TestType{0}, TestType{5},
                                     TestType{0}, TestType{3}, TestType{0}, TestType{10},
                                     TestType{0}, TestType{0}, TestType{4}, TestType{15},
                                     TestType{0}, TestType{0}, TestType{0}, TestType{1}};

    Vector<TestType, 4> point{TestType{1}, TestType{1}, TestType{1}, TestType{1}};
    auto transformed = transform * point;

    REQUIRE(transformed[0] == Catch::Approx(static_cast<TestType>(7)));  // 2*1 + 5*1
    REQUIRE(transformed[1] == Catch::Approx(static_cast<TestType>(13))); // 3*1 + 10*1
    REQUIRE(transformed[2] == Catch::Approx(static_cast<TestType>(19))); // 4*1 + 15*1
    REQUIRE(transformed[3] == Catch::Approx(static_cast<TestType>(1)));  // 1*1
  }
}

TEMPLATE_TEST_CASE("Matrix-matrix multiplication", "[math][matrix][template]", f32, f64)
{
  SECTION("2x2 matrix multiplication")
  {
    Matrix<TestType, 2, 2> m1{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    Matrix<TestType, 2, 2> m2{TestType{5}, TestType{6}, TestType{7}, TestType{8}};

    auto result = m1 * m2;
    REQUIRE(result(0, 0) == Catch::Approx(static_cast<TestType>(19))); // 1*5 + 2*7
    REQUIRE(result(0, 1) == Catch::Approx(static_cast<TestType>(22))); // 1*6 + 2*8
    REQUIRE(result(1, 0) == Catch::Approx(static_cast<TestType>(43))); // 3*5 + 4*7
    REQUIRE(result(1, 1) == Catch::Approx(static_cast<TestType>(50))); // 3*6 + 4*8
  }

  SECTION("3x3 matrix multiplication")
  {
    Matrix<TestType, 3, 3> m1 = Matrix<TestType, 3, 3>::identity();
    Matrix<TestType, 3, 3> m2{TestType{1}, TestType{2}, TestType{3}, TestType{4}, TestType{5},
                              TestType{6}, TestType{7}, TestType{8}, TestType{9}};

    auto result = m1 * m2;
    REQUIRE(result == m2); // Identity multiplication
  }

  SECTION("4x4 matrix multiplication")
  {
    Matrix<TestType, 4, 4> m1 = Matrix<TestType, 4, 4>::identity();
    Matrix<TestType, 4, 4> m2 = Matrix<TestType, 4, 4>::diagonal(TestType{3});

    auto result = m1 * m2;
    REQUIRE(result == m2); // Identity multiplication

    // Test with more complex matrices
    Matrix<TestType, 4, 4> rotation{TestType{0}, TestType{-1}, TestType{0}, TestType{0},
                                    TestType{1}, TestType{0},  TestType{0}, TestType{0},
                                    TestType{0}, TestType{0},  TestType{1}, TestType{0},
                                    TestType{0}, TestType{0},  TestType{0}, TestType{1}};

    Matrix<TestType, 4, 4> translation{TestType{1}, TestType{0}, TestType{0}, TestType{5},
                                       TestType{0}, TestType{1}, TestType{0}, TestType{10},
                                       TestType{0}, TestType{0}, TestType{1}, TestType{15},
                                       TestType{0}, TestType{0}, TestType{0}, TestType{1}};

    auto combined = translation * rotation;

    // Check that rotation part is preserved
    REQUIRE(combined(0, 0) == Catch::Approx(static_cast<TestType>(0)));
    REQUIRE(combined(0, 1) == Catch::Approx(static_cast<TestType>(-1)));
    REQUIRE(combined(1, 0) == Catch::Approx(static_cast<TestType>(1)));
    REQUIRE(combined(1, 1) == Catch::Approx(static_cast<TestType>(0)));

    // Check that translation part is preserved
    REQUIRE(combined(0, 3) == Catch::Approx(static_cast<TestType>(5)));
    REQUIRE(combined(1, 3) == Catch::Approx(static_cast<TestType>(10)));
    REQUIRE(combined(2, 3) == Catch::Approx(static_cast<TestType>(15)));
  }

  SECTION("Non-square matrix multiplication")
  {
    Matrix<TestType, 2, 3> m1{TestType{1}, TestType{2}, TestType{3},
                              TestType{4}, TestType{5}, TestType{6}};
    Matrix<TestType, 3, 2> m2{TestType{7},  TestType{8},  TestType{9},
                              TestType{10}, TestType{11}, TestType{12}};

    auto result = m1 * m2;                                              // Should be 2x2
    REQUIRE(result(0, 0) == Catch::Approx(static_cast<TestType>(58)));  // 1*7 + 2*9 + 3*11
    REQUIRE(result(0, 1) == Catch::Approx(static_cast<TestType>(64)));  // 1*8 + 2*10 + 3*12
    REQUIRE(result(1, 0) == Catch::Approx(static_cast<TestType>(139))); // 4*7 + 5*9 + 6*11
    REQUIRE(result(1, 1) == Catch::Approx(static_cast<TestType>(154))); // 4*8 + 5*10 + 6*12
  }

  SECTION("Large matrix multiplication uses blocked algorithm")
  {
    // Test that large matrices use the blocked multiplication
    Matrix<TestType, 10, 10> m1 = Matrix<TestType, 10, 10>::identity();
    Matrix<TestType, 10, 10> m2 = Matrix<TestType, 10, 10>::diagonal(TestType{2});

    auto result = m1 * m2;

    // Should be diagonal matrix with 2s
    for (usize i = 0; i < 10; ++i)
    {
      for (usize j = 0; j < 10; ++j)
      {
        if (i == j)
        {
          REQUIRE(result(i, j) == Catch::Approx(static_cast<TestType>(2)));
        }
        else
        {
          REQUIRE(result(i, j) == Catch::Approx(static_cast<TestType>(0)));
        }
      }
    }
  }
}

TEMPLATE_TEST_CASE("Matrix determinant and inverse", "[math][matrix][template]", f32, f64)
{
  SECTION("1x1 determinant and inverse")
  {
    Matrix<TestType, 1, 1> m{TestType{5}};
    REQUIRE(m.determinant() == Catch::Approx(static_cast<TestType>(5)));

    auto inv = m.inverse();
    REQUIRE(inv(0, 0) == Catch::Approx(static_cast<TestType>(0.2)));
  }

  SECTION("2x2 determinant and inverse")
  {
    Matrix<TestType, 2, 2> m{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    REQUIRE(m.determinant() == Catch::Approx(static_cast<TestType>(-2))); // 1*4 - 2*3

    auto inv = m.inverse();
    auto identity = m * inv;

    // Check if m * inv ≈ I
    constexpr TestType epsilon{pbrt::math::EPSILON<TestType>};
    REQUIRE(identity(0, 0) == Catch::Approx(static_cast<TestType>(1)).margin(epsilon));
    REQUIRE(identity(0, 1) == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
    REQUIRE(identity(1, 0) == Catch::Approx(static_cast<TestType>(0)).margin(epsilon));
    REQUIRE(identity(1, 1) == Catch::Approx(static_cast<TestType>(1)).margin(epsilon));
  }

  SECTION("3x3 determinant and inverse")
  {
    // Use a well-conditioned matrix that's guaranteed to be invertible
    Matrix<TestType, 3, 3> m{TestType{2}, TestType{0}, TestType{0}, TestType{0}, TestType{3},
                             TestType{0}, TestType{0}, TestType{0}, TestType{4}};

    TestType det = m.determinant();
    REQUIRE(det == Catch::Approx(static_cast<TestType>(24))); // 2*3*4 = 24

    auto inv = m.inverse();
    auto identity = m * inv;

    // Check if m * inv ≈ I
    for (usize i = 0; i < 3; ++i)
    {
      for (usize j = 0; j < 3; ++j)
      {
        TestType expected = (i == j) ? TestType{1} : TestType{0};
        REQUIRE(identity(i, j) == Catch::Approx(expected).margin(pbrt::math::EPSILON<TestType>));
      }
    }
  }

  SECTION("4x4 determinant and inverse")
  {
    Matrix<TestType, 4, 4> m{TestType{1}, TestType{0}, TestType{0}, TestType{0},
                             TestType{0}, TestType{2}, TestType{0}, TestType{0},
                             TestType{0}, TestType{0}, TestType{3}, TestType{0},
                             TestType{0}, TestType{0}, TestType{0}, TestType{4}};

    REQUIRE(m.determinant() == Catch::Approx(static_cast<TestType>(24))); // 1*2*3*4

    auto inv = m.inverse();
    REQUIRE(inv(0, 0) == Catch::Approx(static_cast<TestType>(1)));
    REQUIRE(inv(1, 1) == Catch::Approx(static_cast<TestType>(0.5)));
    REQUIRE(inv(2, 2) == Catch::Approx(static_cast<TestType>(1.0 / 3.0)));
    REQUIRE(inv(3, 3) == Catch::Approx(static_cast<TestType>(0.25)));

    // Test inverse with transformation matrix
    Matrix<TestType, 4, 4> transform{TestType{2}, TestType{0}, TestType{0}, TestType{5},
                                     TestType{0}, TestType{3}, TestType{0}, TestType{10},
                                     TestType{0}, TestType{0}, TestType{4}, TestType{15},
                                     TestType{0}, TestType{0}, TestType{0}, TestType{1}};

    auto invTransform = transform.inverse();
    auto shouldBeIdentity = transform * invTransform;

    constexpr TestType epsilon{pbrt::math::EPSILON<TestType> * 10};
    for (usize i = 0; i < 4; ++i)
    {
      for (usize j = 0; j < 4; ++j)
      {
        TestType expected = (i == j) ? TestType{1} : TestType{0};
        REQUIRE(shouldBeIdentity(i, j) == Catch::Approx(expected).margin(epsilon));
      }
    }
  }

  SECTION("Singular matrix returns zero matrix on inverse")
  {
    Matrix<TestType, 2, 2> singular{TestType{1}, TestType{2}, TestType{2}, TestType{4}};
    REQUIRE(singular.determinant() == Catch::Approx(static_cast<TestType>(0)));

    auto inv = singular.inverse();
    // Should return zero matrix for singular matrices
    for (usize i = 0; i < 2; ++i)
    {
      for (usize j = 0; j < 2; ++j)
      {
        REQUIRE(inv(i, j) == Catch::Approx(static_cast<TestType>(0)));
      }
    }
  }

  SECTION("Large matrix determinant uses LU decomposition")
  {
    Matrix<TestType, 5, 5> m = Matrix<TestType, 5, 5>::identity();
    REQUIRE(m.determinant() == Catch::Approx(static_cast<TestType>(1)));

    Matrix<TestType, 6, 6> m6 = Matrix<TestType, 6, 6>::diagonal(TestType{2});
    REQUIRE(m6.determinant() == Catch::Approx(static_cast<TestType>(64))); // 2^6
  }
}

TEMPLATE_TEST_CASE("Matrix static factory methods", "[math][matrix][template]", f32, f64)
{
  SECTION("Zero matrix")
  {
    auto zero = Matrix<TestType, 3, 3>::zero();
    for (usize i = 0; i < 3; ++i)
    {
      for (usize j = 0; j < 3; ++j)
      {
        REQUIRE(zero(i, j) == TestType{0});
      }
    }

    auto zero4x4 = Matrix<TestType, 4, 4>::zero();
    for (usize i = 0; i < 4; ++i)
    {
      for (usize j = 0; j < 4; ++j)
      {
        REQUIRE(zero4x4(i, j) == TestType{0});
      }
    }
  }

  SECTION("Identity matrix")
  {
    auto identity = Matrix<TestType, 3, 3>::identity();
    for (usize i = 0; i < 3; ++i)
    {
      for (usize j = 0; j < 3; ++j)
      {
        TestType expected = (i == j) ? TestType{1} : TestType{0};
        REQUIRE(identity(i, j) == expected);
      }
    }

    auto identity4x4 = Matrix<TestType, 4, 4>::identity();
    for (usize i = 0; i < 4; ++i)
    {
      for (usize j = 0; j < 4; ++j)
      {
        TestType expected = (i == j) ? TestType{1} : TestType{0};
        REQUIRE(identity4x4(i, j) == expected);
      }
    }
  }

  SECTION("Diagonal matrix")
  {
    auto diag = Matrix<TestType, 4, 4>::diagonal(TestType{5});
    for (usize i = 0; i < 4; ++i)
    {
      for (usize j = 0; j < 4; ++j)
      {
        TestType expected = (i == j) ? TestType{5} : TestType{0};
        REQUIRE(diag(i, j) == expected);
      }
    }

    auto diagNegative = Matrix<TestType, 3, 3>::diagonal(TestType{-2});
    for (usize i = 0; i < 3; ++i)
    {
      for (usize j = 0; j < 3; ++j)
      {
        TestType expected = (i == j) ? TestType{-2} : TestType{0};
        REQUIRE(diagNegative(i, j) == expected);
      }
    }
  }
}

TEMPLATE_TEST_CASE("Matrix utility operations", "[math][matrix][template]", f32, f64)
{
  SECTION("Transpose")
  {
    Matrix<TestType, 2, 3> m{TestType{1}, TestType{2}, TestType{3},
                             TestType{4}, TestType{5}, TestType{6}};

    auto transposed = m.transpose();
    REQUIRE(transposed(0, 0) == TestType{1});
    REQUIRE(transposed(0, 1) == TestType{4});
    REQUIRE(transposed(1, 0) == TestType{2});
    REQUIRE(transposed(1, 1) == TestType{5});
    REQUIRE(transposed(2, 0) == TestType{3});
    REQUIRE(transposed(2, 1) == TestType{6});

    // Free function
    auto transposed2 = transpose(m);
    REQUIRE(transposed == transposed2);

    // 4x4 transpose
    Matrix<TestType, 4, 4> m4x4{TestType{1},  TestType{2},  TestType{3},  TestType{4},
                                TestType{5},  TestType{6},  TestType{7},  TestType{8},
                                TestType{9},  TestType{10}, TestType{11}, TestType{12},
                                TestType{13}, TestType{14}, TestType{15}, TestType{16}};

    auto transposed4x4 = m4x4.transpose();
    REQUIRE(transposed4x4(0, 0) == TestType{1});
    REQUIRE(transposed4x4(0, 1) == TestType{5});
    REQUIRE(transposed4x4(1, 0) == TestType{2});
    REQUIRE(transposed4x4(3, 3) == TestType{16});

    // Test transpose property: (A^T)^T = A
    auto doubleTransposed = transposed4x4.transpose();
    REQUIRE(doubleTransposed == m4x4);
  }

  SECTION("Absolute value")
  {
    if constexpr (std::is_signed_v<TestType>)
    {
      Matrix<TestType, 2, 2> m{TestType{-1}, TestType{2}, TestType{-3}, TestType{4}};
      auto absM = m.abs();
      REQUIRE(absM(0, 0) == TestType{1});
      REQUIRE(absM(0, 1) == TestType{2});
      REQUIRE(absM(1, 0) == TestType{3});
      REQUIRE(absM(1, 1) == TestType{4});

      Matrix<TestType, 4, 4> m4x4{TestType{-1}, TestType{-2},  TestType{3},   TestType{-4},
                                  TestType{5},  TestType{-6},  TestType{-7},  TestType{8},
                                  TestType{-9}, TestType{10},  TestType{-11}, TestType{-12},
                                  TestType{13}, TestType{-14}, TestType{15},  TestType{-16}};
      auto absM4x4 = m4x4.abs();

      for (usize i = 0; i < 4; ++i)
      {
        for (usize j = 0; j < 4; ++j)
        {
          REQUIRE(absM4x4(i, j) == std::abs(m4x4(i, j)));
        }
      }
    }
  }

  SECTION("Approximate equality")
  {
    Matrix<TestType, 2, 2> m1{TestType{1.0}, TestType{2.0}, TestType{3.0}, TestType{4.0}};
    Matrix<TestType, 2, 2> m2{TestType{1.001}, TestType{2.001}, TestType{3.001}, TestType{4.001}};

    REQUIRE_FALSE(m1 == m2);
    REQUIRE(m1.approx_equal(m2, TestType{0.01}));
    REQUIRE_FALSE(m1.approx_equal(m2, TestType{0.0001}));

    Matrix<TestType, 4, 4> m14x4 = Matrix<TestType, 4, 4>::identity();
    Matrix<TestType, 4, 4> m24x4 = m14x4;
    m24x4(2, 2) = TestType{1.0001};

    REQUIRE_FALSE(m14x4 == m24x4);
    REQUIRE(m14x4.approx_equal(m24x4, TestType{0.001}));
    REQUIRE_FALSE(m14x4.approx_equal(m24x4, TestType{0.00001}));
  }
}

TEST_CASE("Matrix type aliases", "[math][matrix]")
{
  SECTION("Float matrices")
  {
    Mat2f m2f{};
    Mat3f m3f{};
    Mat4f m4f{};

    REQUIRE(m2f.rows() == 2);
    REQUIRE(m2f.columns() == 2);
    REQUIRE(m3f.rows() == 3);
    REQUIRE(m3f.columns() == 3);
    REQUIRE(m4f.rows() == 4);
    REQUIRE(m4f.columns() == 4);
  }

  SECTION("Double matrices")
  {
    Mat2d m2d{};
    Mat3d m3d{};
    Mat4d m4d{};

    REQUIRE(m2d.rows() == 2);
    REQUIRE(m3d.rows() == 3);
    REQUIRE(m4d.rows() == 4);
  }

  SECTION("Integer matrices")
  {
    Mat2i m2i{};
    Mat3u m3u{};

    REQUIRE(m2i.rows() == 2);
    REQUIRE(m3u.rows() == 3);
  }

  SECTION("Default aliases")
  {
    Mat2 m2{};
    Mat3 m3{};
    Mat4 m4{};

    // Should be float by default
    static_assert(std::is_same_v<Mat2, Mat2f>);
    static_assert(std::is_same_v<Mat3, Mat3f>);
    static_assert(std::is_same_v<Mat4, Mat4f>);
  }
}

TEST_CASE("Matrix free functions", "[math][matrix]")
{
  SECTION("Free function determinant")
  {
    Mat2f m{1.0F, 2.0F, 3.0F, 4.0F};
    REQUIRE(determinant(m) == Catch::Approx(-2.0F));

    Mat4f m4x4 = Mat4f::diagonal(2.0F);
    REQUIRE(determinant(m4x4) == Catch::Approx(16.0F)); // 2^4
  }

  SECTION("Free function inverse")
  {
    Mat2f m{1.0F, 2.0F, 3.0F, 4.0F};
    auto inv = inverse(m);
    auto identity = m * inv;

    constexpr f32 epsilon{pbrt::math::EPSILON<f32>};
    REQUIRE(identity(0, 0) == Catch::Approx(1.0F).margin(epsilon));
    REQUIRE(identity(0, 1) == Catch::Approx(0.0F).margin(epsilon));
    REQUIRE(identity(1, 0) == Catch::Approx(0.0F).margin(epsilon));
    REQUIRE(identity(1, 1) == Catch::Approx(1.0F).margin(epsilon));

    Mat4f m4x4{2.0F, 0.0F, 0.0F, 1.0F, 0.0F, 3.0F, 0.0F, 2.0F,
               0.0F, 0.0F, 4.0F, 3.0F, 0.0F, 0.0F, 0.0F, 1.0F};
    auto inv4x4 = inverse(m4x4);
    auto identity4x4 = m4x4 * inv4x4;

    for (usize i = 0; i < 4; ++i)
    {
      for (usize j = 0; j < 4; ++j)
      {
        f32 expected = (i == j) ? 1.0F : 0.0F;
        REQUIRE(identity4x4(i, j) == Catch::Approx(expected).margin(epsilon * 10.0F));
      }
    }
  }

  SECTION("Free function transpose")
  {
    Matrix<f32, 2, 3> m{1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F};
    auto transposed = transpose(m);
    REQUIRE(transposed(0, 0) == Catch::Approx(1.0F));
    REQUIRE(transposed(0, 1) == Catch::Approx(4.0F));
    REQUIRE(transposed(1, 0) == Catch::Approx(2.0F));
    REQUIRE(transposed(2, 0) == Catch::Approx(3.0F));
    REQUIRE(transposed(1, 1) == Catch::Approx(5.0F));
    REQUIRE(transposed(2, 1) == Catch::Approx(6.0F));

    // test with 4x4 matrix
    Matrix<f32, 4, 4> m4x4{1.0F, 2.0F,  3.0F,  4.0F,  5.0F,  6.0F,  7.0F,  8.0F,
                           9.0F, 10.0F, 11.0F, 12.0F, 13.0F, 14.0F, 15.0F, 16.0F};
    auto transposed4x4 = transpose(m4x4);
    REQUIRE(transposed4x4(0, 0) == Catch::Approx(1.0F));
    REQUIRE(transposed4x4(0, 1) == Catch::Approx(5.0F));
    REQUIRE(transposed4x4(1, 0) == Catch::Approx(2.0F));
    REQUIRE(transposed4x4(3, 3) == Catch::Approx(16.0F));

    // Test transpose property: (A^T)^T = A
    auto doubleTransposed = transposed4x4.transpose();
    REQUIRE(doubleTransposed == m4x4);
  }
}
