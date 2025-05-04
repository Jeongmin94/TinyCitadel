#pragma once

#include <CitadelPhysicsEngine2D/MathTypes.h>

namespace CitadelPhysicsEngine2D
{

struct Circle
{
    float radius;
    Vec2 position;

    Circle(float radius = 1.0f, const Vec2& position = {})
        : radius(radius), position(position)
    {
    }

    static bool CirclevsCircle(const Circle& a, const Circle& b);
};

} // namespace CitadelPhysicsEngine2D
