#include <catch2/catch_test_macros.hpp>
#include <pbrt/ecs/scene.hpp>
#include <pbrt/logging/logger.hpp>

struct TestComponent
{
  int value{0};

  TestComponent() = default;
  TestComponent(int v) : value(v)
  {
  }
};

TEST_CASE("Scene basic entity/component operations", "[ecs]")
{
  using namespace pbrt::ecs;

  Scene scene;

  SECTION("Entity creation and destruction")
  {
    auto entityOpt{scene.create_entity()};
    REQUIRE(entityOpt.has_value());

    Entity entity{entityOpt.value()};
    REQUIRE(scene.get_registry().valid(entity));

    scene.destroy_entity(entity);
    REQUIRE_FALSE(scene.get_registry().valid(entity));
  }

  SECTION("Component addition, removal and retrieval")
  {
    auto entityOpt{scene.create_entity()};
    REQUIRE(entityOpt.has_value());

    Entity entity{entityOpt.value()};
    REQUIRE(scene.get_registry().valid(entity));
    REQUIRE_FALSE(scene.has_component<TestComponent>(entity));

    scene.add_component<TestComponent>(entity, 42);
    REQUIRE(scene.has_component<TestComponent>(entity));
    REQUIRE(scene.get_component<TestComponent>(entity)->get().value == 42);

    REQUIRE_FALSE(scene.add_component<TestComponent>(entity, 51).has_value());

    scene.remove_component<TestComponent>(entity);
    REQUIRE_FALSE(scene.has_component<TestComponent>(entity));
    REQUIRE_FALSE(scene.get_component<TestComponent>(entity).has_value());
  }

  SECTION("Adding existing component")
  {
    auto entityOpt{scene.create_entity()};
    REQUIRE(entityOpt.has_value());

    Entity entity{entityOpt.value()};
    REQUIRE(scene.get_registry().valid(entity));

    scene.add_component<TestComponent>(entity, 42);
    REQUIRE(scene.has_component<TestComponent>(entity));
    REQUIRE(scene.get_component<TestComponent>(entity)->get().value == 42);

    REQUIRE_FALSE(scene.add_component<TestComponent>(entity, 100).has_value());
    REQUIRE(scene.get_component<TestComponent>(entity)->get().value == 42);

    scene.remove_component<TestComponent>(entity);
    REQUIRE_FALSE(scene.has_component<TestComponent>(entity));
  }

  SECTION("Invalid entity operations")
  {
    Entity invalidEntity{NULL_ENTITY};

    REQUIRE_FALSE(scene.get_component<TestComponent>(invalidEntity).has_value());
    REQUIRE_FALSE(scene.add_component<TestComponent>(invalidEntity, 42));
    REQUIRE_FALSE(scene.has_component<TestComponent>(invalidEntity));
    REQUIRE_FALSE(scene.get_registry().valid(invalidEntity));
  }

  SECTION("Entity iteration")
  {
    auto entity1Opt{scene.create_entity()};
    auto entity2Opt{scene.create_entity()};
    REQUIRE(entity1Opt.has_value());
    REQUIRE(entity2Opt.has_value());

    Entity entity1{entity1Opt.value()};
    Entity entity2{entity2Opt.value()};

    scene.add_component<TestComponent>(entity1, 42);
    scene.add_component<TestComponent>(entity2, 100);

    std::vector<Entity> entities;
    scene.view<TestComponent>().each(
        [&](auto entity, TestComponent &) { entities.push_back(entity); });

    REQUIRE(entities.size() == 2);

    scene.destroy_entity(entity1);
    scene.destroy_entity(entity2);
  }
}
