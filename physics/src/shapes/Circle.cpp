#include <CitadelPhysicsEngine2D/shapes/Circle.h>

namespace CitadelPhysicsEngine2D
{

/**
 * @brief 두 원의 충돌 여부를 확인합니다.
 *
 * @param a 원 1
 * @param b 원 2
 * @return true 충돌하지 않는 경우
 * @return false 충돌하는 경우
 */
bool Circle::CirclevsCircle(const Circle& a, const Circle& b)
{
    float r = a.radius + b.radius;
    float x = a.position.x - b.position.x;
    float y = a.position.y - b.position.y;

    return (r * r) < ((x * x) + (y * y));
}

} // namespace CitadelPhysicsEngine2D