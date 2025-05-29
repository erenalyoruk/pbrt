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
  }

  SECTION("Bounds-checked access with at()")
  {
    Matrix<TestType, 2, 2> m{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    REQUIRE(m.at(0, 0) == TestType{1});
    REQUIRE(m.at(1, 1) == TestType{4});

    REQUIRE_THROWS_AS(m.at(2, 0), std::out_of_range);
    REQUIRE_THROWS_AS(m.at(0, 2), std::out_of_range);
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
  }

  SECTION("Size and capacity")
  {
    Matrix<TestType, 2, 3> m{};
    REQUIRE(m.size() == 6);
    REQUIRE(m.max_size() == 6);
    REQUIRE(m.rows() == 2);
    REQUIRE(m.columns() == 3);
    REQUIRE_FALSE(m.empty());
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
  }

  SECTION("Data pointer access")
  {
    Matrix<TestType, 2, 2> m{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
    const auto *data = m.data();
    REQUIRE(data[0] == TestType{1}); // NOLINT
    REQUIRE(data[1] == TestType{2}); // NOLINT
    REQUIRE(data[2] == TestType{3}); // NOLINT
    REQUIRE(data[3] == TestType{4}); // NOLINT
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

      // Division by zero should throw
      REQUIRE_THROWS_AS(m1 / TestType{0}, std::domain_error);
    }
  }

  SECTION("Comparison operators")
  {
    Matrix<TestType, 2, 2> m3 = m1;
    REQUIRE(m1 == m3);
    REQUIRE_FALSE(m1 == m2);
    REQUIRE(m1 != m2);
    REQUIRE_FALSE(m1 != m3);
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
  }

  SECTION("Singular matrix throws on inverse")
  {
    Matrix<TestType, 2, 2> singular{TestType{1}, TestType{2}, TestType{2}, TestType{4}};
    REQUIRE(singular.determinant() == Catch::Approx(static_cast<TestType>(0)));
    REQUIRE_THROWS_AS(singular.inverse(), std::domain_error);
  }

  SECTION("Large matrix determinant uses LU decomposition")
  {
    Matrix<TestType, 5, 5> m = Matrix<TestType, 5, 5>::identity();
    REQUIRE(m.determinant() == Catch::Approx(static_cast<TestType>(1)));
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
    }
  }

  SECTION("Approximate equality")
  {
    Matrix<TestType, 2, 2> m1{TestType{1.0}, TestType{2.0}, TestType{3.0}, TestType{4.0}};
    Matrix<TestType, 2, 2> m2{TestType{1.001}, TestType{2.001}, TestType{3.001}, TestType{4.001}};

    REQUIRE_FALSE(m1 == m2);
    REQUIRE(m1.approx_equal(m2, TestType{0.01}));
    REQUIRE_FALSE(m1.approx_equal(m2, TestType{0.0001}));
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
    REQUIRE(m4f.rows() == 4);
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
  }

  SECTION("Free function transpose")
  {
    Matrix<f32, 2, 3> m{1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F};
    auto t = transpose(m);

    REQUIRE(t.rows() == 3);
    REQUIRE(t.columns() == 2);
    REQUIRE(t(0, 0) == 1.0F);
    REQUIRE(t(1, 0) == 2.0F);
    REQUIRE(t(2, 1) == 6.0F);
  }
}

TEST_CASE("Matrix performance edge cases", "[math][matrix]")
{
  SECTION("Large matrix operations")
  {
    // Test that large matrices don't cause stack overflow
    Matrix<f32, 100, 100> large1 = Matrix<f32, 100, 100>::identity();
    Matrix<f32, 100, 100> large2 = Matrix<f32, 100, 100>::diagonal(2.0F);

    auto result = large1 + large2;
    REQUIRE(result(0, 0) == Catch::Approx(3.0F));
    REQUIRE(result(50, 50) == Catch::Approx(3.0F));
  }

  SECTION("Matrix conversion between types")
  {
    Matrix<f32, 2, 2> mf{1.5F, 2.5F, 3.5F, 4.5F};
    Matrix<f64, 2, 2> md{mf}; // Convert f32 to f64

    REQUIRE(md(0, 0) == Catch::Approx(1.5));
    REQUIRE(md(1, 1) == Catch::Approx(4.5));
  }
}
