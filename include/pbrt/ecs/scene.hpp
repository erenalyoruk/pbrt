#pragma once

#include "pbrt/ecs/types.hpp"
#include "pbrt/export.hpp"
#include "pbrt/logging/logger.hpp"

#include <functional>
#include <optional>

namespace pbrt::ecs
{
/**
 * @brief The Scene class is a container for entities and their components.
 */
class PBRT_API Scene
{
public:
  /**
   * @brief Constructs a new Scene.
   */
  Scene();

  /**
   * @brief Destroys the Scene.
   */
  ~Scene();

  /**
   * @brief Creates a new entity in the scene.
   *
   * @return std::optional<Entity> The created entity, or std::nullopt if the entity could not be
   * created.
   */
  [[nodiscard]] std::optional<Entity> create_entity();

  /**
   * @brief Destroys an entity in the scene.
   *
   * @param entity The entity to destroy.
   */
  void destroy_entity(Entity entity);

  /**
   * @brief Adds a component to an entity.
   *
   * @tparam Component The type of the component to add.
   * @tparam Args The types of the arguments to pass to the component constructor.
   * @param entity The entity to add the component to.
   * @param args The arguments to pass to the component constructor.
   * @return std::optional<std::reference_wrapper<Component>> A reference to the component, or
   * std::nullopt if the component could not be added.
   */
  template <typename Component, typename... Args>
  std::optional<std::reference_wrapper<Component>> add_component(Entity entity, Args &&...args);

  /**
   * @brief Gets a component from an entity.
   *
   * @tparam Component The type of the component to get.
   * @param entity The entity to get the component from.
   * @return std::optional<std::reference_wrapper<Component>> A reference to the component, or
   * std::nullopt if the component could not be found.
   */
  template <typename Component>
  [[nodiscard]] std::optional<std::reference_wrapper<Component>> get_component(Entity entity);

  /**
   * @brief Gets a component from an entity.
   *
   * @tparam Component The type of the component to get.
   * @param entity The entity to get the component from.
   * @return std::optional<std::reference_wrapper<const Component>> A const reference to the
   * component, or std::nullopt if the component could not be found.
   */
  template <typename Component>
  [[nodiscard]] std::optional<std::reference_wrapper<const Component>> get_component(
      Entity entity) const;

  /**
   * @brief Checks if an entity has a component.
   *
   * @tparam Component The type of the component to check for.
   * @param entity The entity to check.
   * @return true if the entity has the component.
   * @return false if the entity does not have the component.
   */
  template <typename Component>
  [[nodiscard]] bool has_component(Entity entity) const;

  /**
   * @brief Removes a component from an entity.
   *
   * @tparam Component The type of the component to remove.
   * @param entity The entity to remove the component from.
   */
  template <typename Component>
  void remove_component(Entity entity);

  /**
   * @brief Gets a view of the entities with the specified components.
   *
   * @tparam Components The types of the components to view.
   * @return auto A view of the entities with the specified components.
   */
  template <typename... Components>
  auto view();

  /**
   * @brief Gets a view of the entities with the specified components.
   *
   * @tparam Components The types of the components to view.
   * @return auto A const view of the entities with the specified components.
   */
  template <typename... Components>
  [[nodiscard]] auto view() const;

  /**
   * @brief Gets the registry.
   *
   * @return Registry& A reference to the registry.
   */
  [[nodiscard]] Registry &get_registry();

  /**
   * @brief Gets the registry.
   *
   * @return const Registry& A const reference to the registry.
   */
  [[nodiscard]] const Registry &get_registry() const;

private:
  Registry m_registry;
};

template <typename Component, typename... Args>
inline std::optional<std::reference_wrapper<Component>> Scene::add_component(Entity entity,
                                                                             Args &&...args)
{
  if (!m_registry.valid(entity))
  {
    PBRT_LOG_ERROR("Entity is not valid");
    return std::nullopt;
  }

  if (has_component<Component>(entity))
  {
    PBRT_LOG_ERROR("Entity already has component of type {}", entt::type_name<Component>().value());
    return std::nullopt;
  }

  return m_registry.emplace<Component>(entity, std::forward<Args>(args)...);
}

template <typename Component>
inline std::optional<std::reference_wrapper<Component>> Scene::get_component(Entity entity)
{
  if (!m_registry.valid(entity))
  {
    PBRT_LOG_ERROR("Entity is not valid");
    return std::nullopt;
  }

  if (!has_component<Component>(entity))
  {
    PBRT_LOG_ERROR("Entity does not have component of type {}",
                   entt::type_name<Component>().value());
    return std::nullopt;
  }

  return m_registry.get<Component>(entity);
}

template <typename Component>
inline std::optional<std::reference_wrapper<const Component>> Scene::get_component(
    Entity entity) const
{
  if (!m_registry.valid(entity))
  {
    PBRT_LOG_ERROR("Entity is not valid");
    return std::nullopt;
  }

  if (!has_component<Component>(entity))
  {
    PBRT_LOG_ERROR("Entity does not have component of type {}", entt::type_name<Component>());
    return std::nullopt;
  }

  return m_registry.get<Component>(entity);
}

template <typename Component>
inline bool Scene::has_component(Entity entity) const
{
  if (!m_registry.valid(entity))
  {
    PBRT_LOG_ERROR("Entity is not valid");
    return false;
  }

  return m_registry.all_of<Component>(entity);
}

template <typename Component>
inline void Scene::remove_component(Entity entity)
{
  if (!m_registry.valid(entity))
  {
    PBRT_LOG_ERROR("Entity is not valid");
    return;
  }

  if (!has_component<Component>(entity))
  {
    PBRT_LOG_ERROR("Entity does not have component of type {}",
                   entt::type_name<Component>().value());
    return;
  }

  m_registry.remove<Component>(entity);
}

template <typename... Components>
inline auto Scene::view()
{
  return m_registry.view<Components...>();
}

template <typename... Components>
inline auto Scene::view() const
{
  return m_registry.view<Components...>();
}
} // namespace pbrt::ecs
