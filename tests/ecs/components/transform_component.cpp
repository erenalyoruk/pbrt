#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <pbrt/ecs/components/transform_component.hpp>
#include <pbrt/math/constants.hpp>
#include <pbrt/math/matrix_transform.hpp>
#include <pbrt/types.hpp>

using namespace pbrt;
using namespace pbrt::ecs::components;
using namespace pbrt::math;

TEST_CASE("TransformComponent construction", "[ecs][transform]")
{
  SECTION("Default constructor")
  {
    TransformComponent transform{};

    REQUIRE(transform.get_position() == Vec3f::zero());
    REQUIRE(transform.get_rotation() == Quatf::identity());
    REQUIRE(transform.get_scale() == Vec3f::one());

    // Verify world matrix is identity
    auto worldMatrix{transform.get_world_matrix()};
    auto identity{Mat4f::identity()};
    REQUIRE(worldMatrix.approx_equal(identity, EPSILON<f32>));
  }

  SECTION("Parameterized constructor")
  {
    Vec3f position{1.0F, 2.0F, 3.0F};
    auto rotation{Quatf::from_axis_angle(Vec3f{0.0F, 1.0F, 0.0F}, PI<f32> / 4.0F)};
    Vec3f scale{2.0F, 3.0F, 4.0F};

    TransformComponent transform{position, rotation, scale};

    REQUIRE(transform.get_position() == position);
    REQUIRE(transform.get_rotation().approx_equal(rotation, EPSILON<f32>));
    REQUIRE(transform.get_scale() == scale);
  }
}

TEST_CASE("TransformComponent setters", "[ecs][transform]")
{
  TransformComponent transform{};

  SECTION("set_position")
  {
    Vec3f newPosition{5.0F, 10.0F, 15.0F};
    transform.set_position(newPosition);

    REQUIRE(transform.get_position() == newPosition);

    // Matrix should be updated
    auto worldMatrix{transform.get_world_matrix()};
    REQUIRE(worldMatrix(0, 3) == Catch::Approx(5.0F));
    REQUIRE(worldMatrix(1, 3) == Catch::Approx(10.0F));
    REQUIRE(worldMatrix(2, 3) == Catch::Approx(15.0F));
  }

  SECTION("set_rotation")
  {
    auto axis{Vec3f{0.0F, 0.0F, 1.0F}};
    auto angle{PI<f32> / 2.0F};
    auto newRotation{Quatf::from_axis_angle(axis, angle)};

    transform.set_rotation(newRotation);

    auto storedRotation{transform.get_rotation()};
    REQUIRE(storedRotation.approx_equal(newRotation, EPSILON<f32> * 10.0F));

    // Rotation should be normalized
    REQUIRE(storedRotation.length() == Catch::Approx(1.0F));
  }

  SECTION("set_scale")
  {
    Vec3f newScale{0.5F, 2.0F, 3.0F};
    transform.set_scale(newScale);

    REQUIRE(transform.get_scale() == newScale);
  }
}

TEST_CASE("TransformComponent direction vectors", "[ecs][transform]")
{
  SECTION("Identity rotation direction vectors")
  {
    TransformComponent transform{};

    auto forward{transform.get_forward()};
    auto right{transform.get_right()};
    auto up{transform.get_up()};

    REQUIRE(forward.x() == Catch::Approx(0.0F).margin(EPSILON<f32>));
    REQUIRE(forward.y() == Catch::Approx(0.0F).margin(EPSILON<f32>));
    REQUIRE(forward.z() == Catch::Approx(-1.0F).margin(EPSILON<f32>));

    REQUIRE(right.x() == Catch::Approx(1.0F).margin(EPSILON<f32>));
    REQUIRE(right.y() == Catch::Approx(0.0F).margin(EPSILON<f32>));
    REQUIRE(right.z() == Catch::Approx(0.0F).margin(EPSILON<f32>));

    REQUIRE(up.x() == Catch::Approx(0.0F).margin(EPSILON<f32>));
    REQUIRE(up.y() == Catch::Approx(1.0F).margin(EPSILON<f32>));
    REQUIRE(up.z() == Catch::Approx(0.0F).margin(EPSILON<f32>));
  }

  SECTION("90 degree Y rotation direction vectors")
  {
    TransformComponent transform{};
    auto rotation{Quatf::from_axis_angle(Vec3f{0.0F, 1.0F, 0.0F}, PI<f32> / 2.0F)};
    transform.set_rotation(rotation);

    auto forward{transform.get_forward()};
    auto right{transform.get_right()};
    auto up{transform.get_up()};

    REQUIRE(forward.x() == Catch::Approx(1.0F).margin(EPSILON<f32> * 10.0F));
    REQUIRE(forward.y() == Catch::Approx(0.0F).margin(EPSILON<f32> * 10.0F));
    REQUIRE(forward.z() == Catch::Approx(0.0F).margin(EPSILON<f32> * 10.0F));

    REQUIRE(right.x() == Catch::Approx(0.0F).margin(EPSILON<f32> * 10.0F));
    REQUIRE(right.y() == Catch::Approx(0.0F).margin(EPSILON<f32> * 10.0F));
    REQUIRE(right.z() == Catch::Approx(1.0F).margin(EPSILON<f32> * 10.0F));

    REQUIRE(up.x() == Catch::Approx(0.0F).margin(EPSILON<f32> * 10.0F));
    REQUIRE(up.y() == Catch::Approx(1.0F).margin(EPSILON<f32> * 10.0F));
    REQUIRE(up.z() == Catch::Approx(0.0F).margin(EPSILON<f32> * 10.0F));
  }
}

TEST_CASE("TransformComponent world matrix computation", "[ecs][transform]")
{
  SECTION("Translation only")
  {
    Vec3f position{10.0F, 20.0F, 30.0F};
    TransformComponent transform{position, Quatf::identity(), Vec3f::one()};

    auto worldMatrix{transform.get_world_matrix()};

    REQUIRE(worldMatrix(0, 3) == Catch::Approx(10.0F));
    REQUIRE(worldMatrix(1, 3) == Catch::Approx(20.0F));
    REQUIRE(worldMatrix(2, 3) == Catch::Approx(30.0F));
    REQUIRE(worldMatrix(3, 3) == Catch::Approx(1.0F));
  }

  SECTION("Scale only")
  {
    Vec3f scale{2.0F, 3.0F, 4.0F};
    TransformComponent transform{Vec3f::zero(), Quatf::identity(), scale};

    auto worldMatrix{transform.get_world_matrix()};

    REQUIRE(worldMatrix(0, 0) == Catch::Approx(2.0F));
    REQUIRE(worldMatrix(1, 1) == Catch::Approx(3.0F));
    REQUIRE(worldMatrix(2, 2) == Catch::Approx(4.0F));
    REQUIRE(worldMatrix(3, 3) == Catch::Approx(1.0F));
  }

  SECTION("Combined transformation")
  {
    Vec3f position{5.0F, 0.0F, 0.0F};
    auto rotation{Quatf::from_axis_angle(Vec3f{0.0F, 0.0F, 1.0F}, PI<f32> / 2.0F)};
    Vec3f scale{2.0F, 2.0F, 2.0F};

    TransformComponent transform{position, rotation, scale};
    auto worldMatrix{transform.get_world_matrix()};

    // Test point transformation
    Vec4f point{1.0F, 0.0F, 0.0F, 1.0F};
    auto transformedPoint{worldMatrix * point};

    // Scale first: (2, 0, 0), then rotate: (0, 2, 0), then translate: (5, 2, 0)
    REQUIRE(transformedPoint[0] == Catch::Approx(5.0F).margin(EPSILON<f32> * 10.0F));
    REQUIRE(transformedPoint[1] == Catch::Approx(2.0F).margin(EPSILON<f32> * 10.0F));
    REQUIRE(transformedPoint[2] == Catch::Approx(0.0F).margin(EPSILON<f32> * 10.0F));
    REQUIRE(transformedPoint[3] == Catch::Approx(1.0F));
  }
}

TEST_CASE("TransformComponent inverse world matrix", "[ecs][transform]")
{
  SECTION("Matrix inversion correctness")
  {
    Vec3f position{1.0F, 2.0F, 3.0F};
    auto rotation{Quatf::from_axis_angle(Vec3f{0.0F, 1.0F, 0.0F}, PI<f32> / 6.0F)};
    Vec3f scale{0.5F, 2.0F, 1.5F};

    TransformComponent transform{position, rotation, scale};

    auto worldMatrix{transform.get_world_matrix()};
    auto inverseMatrix{transform.get_inverse_world_matrix()};

    auto identity{worldMatrix * inverseMatrix};
    auto expectedIdentity{Mat4f::identity()};

    REQUIRE(identity.approx_equal(expectedIdentity, EPSILON<f32> * 100.0F));
  }

  SECTION("Point transformation round trip")
  {
    Vec3f position{5.0F, 10.0F, 15.0F};
    auto rotation{Quatf::from_axis_angle(Vec3f{1.0F, 1.0F, 1.0F}.normalized(), PI<f32> / 4.0F)};
    Vec3f scale{2.0F, 3.0F, 4.0F};

    TransformComponent transform{position, rotation, scale};

    auto worldMatrix{transform.get_world_matrix()};
    auto inverseMatrix{transform.get_inverse_world_matrix()};

    Vec4f originalPoint{1.0F, 2.0F, 3.0F, 1.0F};
    auto transformedPoint{worldMatrix * originalPoint};
    auto backToOriginal{inverseMatrix * transformedPoint};

    REQUIRE(backToOriginal[0] == Catch::Approx(originalPoint[0]).margin(EPSILON<f32> * 100.0F));
    REQUIRE(backToOriginal[1] == Catch::Approx(originalPoint[1]).margin(EPSILON<f32> * 100.0F));
    REQUIRE(backToOriginal[2] == Catch::Approx(originalPoint[2]).margin(EPSILON<f32> * 100.0F));
    REQUIRE(backToOriginal[3] == Catch::Approx(originalPoint[3]).margin(EPSILON<f32> * 100.0F));
  }
}

TEST_CASE("TransformComponent matrix caching", "[ecs][transform]")
{
  SECTION("Matrix update only when dirty")
  {
    TransformComponent transform{};

    // Get initial matrix
    auto const &matrix1{transform.get_world_matrix()};
    auto const &matrix2{transform.get_world_matrix()};

    // Should be the same object (no recomputation)
    REQUIRE(&matrix1 == &matrix2);
  }

  SECTION("Matrix update after position change")
  {
    TransformComponent transform{};
    auto initialMatrix{transform.get_world_matrix()};

    transform.set_position(Vec3f{1.0F, 2.0F, 3.0F});
    auto updatedMatrix{transform.get_world_matrix()};

    REQUIRE_FALSE(initialMatrix.approx_equal(updatedMatrix, EPSILON<f32>));
  }

  SECTION("Matrix update after rotation change")
  {
    TransformComponent transform{};
    auto initialMatrix{transform.get_world_matrix()};

    auto rotation{Quatf::from_axis_angle(Vec3f{0.0F, 1.0F, 0.0F}, PI<f32> / 4.0F)};
    transform.set_rotation(rotation);
    auto updatedMatrix{transform.get_world_matrix()};

    REQUIRE_FALSE(initialMatrix.approx_equal(updatedMatrix, EPSILON<f32>));
  }

  SECTION("Matrix update after scale change")
  {
    TransformComponent transform{};
    auto initialMatrix{transform.get_world_matrix()};

    transform.set_scale(Vec3f{2.0F, 3.0F, 4.0F});
    auto updatedMatrix{transform.get_world_matrix()};

    REQUIRE_FALSE(initialMatrix.approx_equal(updatedMatrix, EPSILON<f32>));
  }
}

TEST_CASE("TransformComponent edge cases", "[ecs][transform]")
{
  SECTION("Large transformation values")
  {
    const auto largeValue{math::EPSILON_E<f32, 6>};
    const auto smallValue{math::EPSILON_E<f32, 3>};

    Vec3f largePosition{largeValue};
    Vec3f largeScale{smallValue};

    TransformComponent transform{largePosition, Quatf::identity(), largeScale};
    auto worldMatrix{transform.get_world_matrix()};

    REQUIRE(std::isfinite(worldMatrix(0, 0)));
    REQUIRE(std::isfinite(worldMatrix(0, 3)));
  }

  SECTION("Quaternion normalization")
  {
    TransformComponent transform{};

    // Set unnormalized quaternion
    Quatf unnormalizedQuat{1.0F, 2.0F, 3.0F, 4.0F}; // Length > 1
    transform.set_rotation(unnormalizedQuat);

    auto storedRotation{transform.get_rotation()};
    REQUIRE(storedRotation.length() == Catch::Approx(1.0F));
  }
}

TEST_CASE("TransformComponent transformation order", "[ecs][transform]")
{
  SECTION("TRS order verification")
  {
    // Create transform with all components
    Vec3f position{10.0F, 0.0F, 0.0F};
    auto rotation{Quatf::from_axis_angle(Vec3f{0.0F, 0.0F, 1.0F}, PI<f32> / 2.0F)};
    Vec3f scale{2.0F, 1.0F, 1.0F};

    TransformComponent transform{position, rotation, scale};
    auto worldMatrix{transform.get_world_matrix()};

    // Test with a point at origin
    Vec4f point{1.0F, 0.0F, 0.0F, 1.0F};
    auto result{worldMatrix * point};

    // Manual TRS: Scale(2,0,0) -> Rotate(0,2,0) -> Translate(10,2,0)
    REQUIRE(result[0] == Catch::Approx(10.0F).margin(EPSILON<f32> * 10.0F));
    REQUIRE(result[1] == Catch::Approx(2.0F).margin(EPSILON<f32> * 10.0F));
    REQUIRE(result[2] == Catch::Approx(0.0F).margin(EPSILON<f32> * 10.0F));
  }
}
