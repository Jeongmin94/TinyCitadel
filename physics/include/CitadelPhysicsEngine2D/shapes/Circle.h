#pragma once

#include <CitadelPhysicsEngine2D/math/EngineMath.h>

namespace CitadelPhysicsEngine2D
{

struct Circle
{
    float radius;
    glm::vec2 position;

    Circle(float radius = 1.0f, const glm::vec2& position = {})
        : radius(radius), position(position)
    {
    }

    static bool CirclevsCircle(const Circle& a, const Circle& b);
};

} // namespace CitadelPhysicsEngine2D
