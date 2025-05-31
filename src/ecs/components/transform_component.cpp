#include "pbrt/ecs/components/transform_component.hpp"

#include "pbrt/math/matrix_transform.hpp"

namespace pbrt::ecs::components
{
TransformComponent::TransformComponent()
{
  m_rotation.normalize();

  update_world_matrix();
  update_rotation_matrix();
}

TransformComponent::TransformComponent(math::Vec3f const &position, math::Quatf const &rotation,
                                       math::Vec3f const &scale)
    : m_position{position}, m_rotation{rotation}, m_scale{scale},
      m_worldMatrix{math::Mat4f::identity()}, m_inverseWorldMatrix{math::Mat4f::identity()}
{
  m_rotation.normalize();

  update_rotation_matrix();
  update_world_matrix();
}

void TransformComponent::set_position(math::Vec3f const &position) noexcept
{
  m_position = position;
  mark_world_matrix_dirty();
}

void TransformComponent::set_rotation(math::Quatf const &rotation)
{
  m_rotation = rotation;
  m_rotation.normalize();

  mark_world_matrix_dirty();
  mark_rotation_matrix_dirty();
}

void TransformComponent::set_scale(math::Vec3f const &scale) noexcept
{
  m_scale = scale;
  mark_world_matrix_dirty();
}

math::Vec3f TransformComponent::get_forward() noexcept
{
  if (m_isRotationMatrixDirty)
  {
    update_rotation_matrix();
  }

  return -math::xyz(m_rotationMatrix.row(2));
}

math::Vec3f TransformComponent::get_right() noexcept
{
  if (m_isRotationMatrixDirty)
  {
    update_rotation_matrix();
  }

  return math::xyz(m_rotationMatrix.row(0));
}

math::Vec3f TransformComponent::get_up() noexcept
{
  if (m_isRotationMatrixDirty)
  {
    update_rotation_matrix();
  }

  return math::xyz(m_rotationMatrix.row(1));
}

math::Vec3f const &TransformComponent::get_position() const noexcept
{
  return m_position;
}

math::Quatf const &TransformComponent::get_rotation() const noexcept
{
  return m_rotation;
}

math::Vec3f const &TransformComponent::get_scale() const noexcept
{
  return m_scale;
}

math::Mat4f const &TransformComponent::get_world_matrix() noexcept
{
  if (m_isWorldMatrixDirty)
  {
    update_world_matrix();
  }

  return m_worldMatrix;
}

math::Mat4f const &TransformComponent::get_inverse_world_matrix() noexcept
{
  if (m_isWorldMatrixDirty)
  {
    update_world_matrix();
  }

  return m_inverseWorldMatrix;
}

void TransformComponent::update_world_matrix() noexcept
{
  if (!m_isWorldMatrixDirty)
  {
    return;
  }

  const auto translationMatrix{math::translate(m_position)};
  const auto rotationMatrix{math::rotate(m_rotation)};
  const auto scaleMatrix{math::scale(m_scale)};

  m_worldMatrix = translationMatrix * rotationMatrix * scaleMatrix;
  m_inverseWorldMatrix = m_worldMatrix.inverse();

  m_isWorldMatrixDirty = false;
}

void TransformComponent::update_rotation_matrix() noexcept
{
  if (!m_isRotationMatrixDirty)
  {
    return;
  }

  m_rotationMatrix = math::rotate(m_rotation);
  m_isRotationMatrixDirty = false;
}

void TransformComponent::mark_world_matrix_dirty() noexcept
{
  m_isWorldMatrixDirty = true;
}

void TransformComponent::mark_rotation_matrix_dirty() noexcept
{
  m_isRotationMatrixDirty = true;
}
} // namespace pbrt::ecs::components
