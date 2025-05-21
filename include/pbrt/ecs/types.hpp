#pragma once

#include <entt/entt.hpp>

namespace pbrt::ecs
{
using Entity = entt::entity;
using Registry = entt::registry;

constexpr Entity NULL_ENTITY{entt::null};
} // namespace pbrt::ecs
