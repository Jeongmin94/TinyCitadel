#include <CitadelPhysicsEngine2D/AABB.h>

namespace CitadelPhysicsEngine2D
{

/**
 * @brief 두 AABB의 충돌 여부를 확인합니다.
 *
 * @param a AABB 1
 * @param b AABB 2
 * @return true 충돌하지 않는 경우
 * @return false 충돌하는 경우
 */
bool AABB::AABBvsAABB(const AABB& a, const AABB& b)
{
    if (a.max.x < b.min.x || a.min.x > b.max.x)
        return true;
    if (a.max.y < b.min.y || a.min.y > b.max.y)
        return true;

    return false;
}

} // namespace CitadelPhysicsEngine2D
