#include "pbrt/ecs/scene.hpp"

namespace pbrt::ecs
{
Scene::Scene() = default;
Scene::~Scene() = default;

std::optional<Entity> Scene::create_entity()
{
  return m_registry.create();
}

void Scene::destroy_entity(Entity entity)
{
  m_registry.destroy(entity);
}

Registry &Scene::get_registry()
{
  return m_registry;
}

const Registry &Scene::get_registry() const
{
  return m_registry;
}
} // namespace pbrt::ecs
