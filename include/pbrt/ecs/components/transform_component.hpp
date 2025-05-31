#pragma once

#include "pbrt/export.hpp"
#include "pbrt/math/matrix.hpp"
#include "pbrt/math/quaternion.hpp"
#include "pbrt/math/vector.hpp"

namespace pbrt::ecs::components
{
/**
 * @brief Component that represents the transformation of an entity in 3D space.
 * It includes position, rotation, and scale, and provides methods to manipulate
 * these properties. The component also computes the world matrix and its inverse
 * for rendering and physics calculations.
 */
class PBRT_API TransformComponent
{
public:
  /**
   * @brief Default constructor for TransformComponent.
   */
  TransformComponent();

  /**
   * @brief Constructs a TransformComponent with specified position, rotation, and scale.
   *
   * @param position The position of the entity in 3D space.
   * @param rotation The rotation of the entity as a quaternion.
   * @param scale The scale of the entity in 3D space.
   */
  TransformComponent(math::Vec3f const &position, math::Quatf const &rotation,
                     math::Vec3f const &scale);

  /**
   * @brief Sets the position of the entity.
   *
   * @param position The new position of the entity in 3D space.
   */
  void set_position(math::Vec3f const &position) noexcept;

  /**
   * @brief Sets the rotation of the entity.
   *
   * @param rotation The new rotation of the entity as a quaternion.
   */
  void set_rotation(math::Quatf const &rotation);

  /**
   * @brief Sets the scale of the entity.
   *
   * @param scale The new scale of the entity in 3D space.
   */
  void set_scale(math::Vec3f const &scale) noexcept;

  /**
   * @brief Gets the forward direction of the entity.
   *
   * @return math::Vec3f The forward direction vector of the entity.
   */
  math::Vec3f get_forward() noexcept;

  /**
   * @brief Gets the right direction of the entity.
   *
   * @return math::Vec3f The right direction vector of the entity.
   */
  math::Vec3f get_right() noexcept;

  /**
   * @brief Gets the up direction of the entity.
   *
   * @return math::Vec3f The up direction vector of the entity.
   */
  math::Vec3f get_up() noexcept;

  /**
   * @brief Gets the position of the entity.
   *
   * @return math::Vec3f const& The position of the entity in 3D space.
   */
  math::Vec3f const &get_position() const noexcept;

  /**
   * @brief Gets the rotation of the entity.
   *
   * @return math::Quatf const& The rotation of the entity as a quaternion.
   */
  math::Quatf const &get_rotation() const noexcept;

  /**
   * @brief Gets the scale of the entity.
   *
   * @return math::Vec3f const& The scale of the entity in 3D space.
   */
  math::Vec3f const &get_scale() const noexcept;

  /**
   * @brief Gets the world matrix of the entity.
   *
   * The world matrix is a transformation matrix that combines position, rotation,
   * and scale, allowing for transformations in the 3D space.
   *
   * @return math::Mat4f const& The world matrix of the entity.
   */
  math::Mat4f const &get_world_matrix() noexcept;

  /**
   * @brief Gets the inverse world matrix of the entity.
   *
   * The inverse world matrix is used for transforming coordinates from world space
   * back to local space, which is useful for various calculations in rendering and physics.
   *
   * @return math::Mat4f const& The inverse world matrix of the entity.
   */
  math::Mat4f const &get_inverse_world_matrix() noexcept;

private:
  math::Vec3f m_position{0.0F};
  math::Quatf m_rotation{math::Quatf::identity()};
  math::Vec3f m_scale{1.0F};

  mutable bool m_isWorldMatrixDirty{true};
  math::Mat4f m_worldMatrix{math::Mat4f::identity()};
  math::Mat4f m_inverseWorldMatrix{math::Mat4f::identity()};

  mutable bool m_isRotationMatrixDirty{true};
  math::Mat4f m_rotationMatrix{math::Mat4f::identity()};

  void update_world_matrix() noexcept;
  void update_rotation_matrix() noexcept;

  void mark_world_matrix_dirty() noexcept;
  void mark_rotation_matrix_dirty() noexcept;
};
} // namespace pbrt::ecs::components
