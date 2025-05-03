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
};

inline bool Intersect(const AABB& a, const AABB& b)
{
    bool overlapX = a.max.x > b.min.x && a.min.x < b.max.x;
    bool overlapY = a.max.y > b.min.y && a.min.y < b.max.y;
    return overlapX && overlapY;
}

} // namespace CitadelPhysicsEngine2D
