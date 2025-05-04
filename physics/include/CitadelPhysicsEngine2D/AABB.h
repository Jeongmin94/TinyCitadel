#pragma once

#include "MathTypes.h"

namespace CitadelPhysicsEngine2D
{

struct AABB
{
    Vec2 min;
    Vec2 max;

    AABB() = default;
    AABB(const Vec2& min, const Vec2& max) : min(min), max(max) {}

    static bool AABBvsAABB(const AABB& a, const AABB& b);
};

} // namespace CitadelPhysicsEngine2D
