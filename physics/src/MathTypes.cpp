#include <CitadelPhysicsEngine2D/MathTypes.h>

#include <cmath>

namespace CitadelPhysicsEngine2D
{

float CitadelMath::Distance(const Vec2& a, const Vec2& b)
{
    float x = a.x - b.x;
    float y = a.y - b.y;
    x *= x;
    y *= y;

    return std::sqrt(x + y);
}

float CitadelMath::Square(float x) { return x * x; }

} // namespace CitadelPhysicsEngine2D
