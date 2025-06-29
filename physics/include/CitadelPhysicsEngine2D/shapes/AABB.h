#pragma once

#include <CitadelPhysicsEngine2D/math/EngineMath.h>

namespace CitadelPhysicsEngine2D
{

struct AABB
{
    glm::vec2 min;
    glm::vec2 max;

    AABB() = default;
    AABB(const glm::vec2& min, const glm::vec2& max) : min(min), max(max) {}

    static bool AABBvsAABB(const AABB& a, const AABB& b);
};

} // namespace CitadelPhysicsEngine2D
