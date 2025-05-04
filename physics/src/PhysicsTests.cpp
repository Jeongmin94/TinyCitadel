#include <CitadelPhysicsEngine2D/core.h>

#include <cassert>  // assert 매크로 사용
#include <iostream> // 테스트 메시지 출력용

namespace CitadelPhysicsEngine2D
{

// 테스트 함수 정의
void RunPhysicsTests()
{
    std::cout << "Running Physics Engine tests...\n";

    // --- Circle vs Circle Tests ---
    std::cout << "  Testing Circle vs Circle...\n";

    // Case 1: 충돌하는 경우
    Circle c1(1.0f, {0.0f, 0.0f});
    Circle c2(1.0f, {1.0f, 0.0f}); // 반지름 합 = 2, 거리 = 1
    assert(Circle::CirclevsCircle(c1, c2) == false);
    std::cout << "    Test 1 (Intersecting): Passed\n";

    // Case 2: 정확히 접하는 경우
    Circle c3(1.0f, {0.0f, 0.0f});
    Circle c4(1.0f, {2.0f, 0.0f});                   // 반지름 합 = 2, 거리 = 2
    assert(Circle::CirclevsCircle(c3, c4) == false); // 경계 포함 시 false
    std::cout << "    Test 2 (Touching): Passed\n";

    // Case 3: 충돌하지 않는 경우
    Circle c5(1.0f, {0.0f, 0.0f});
    Circle c6(1.0f, {3.0f, 0.0f}); // 반지름 합 = 2, 거리 = 3
    assert(Circle::CirclevsCircle(c5, c6) == true);
    std::cout << "    Test 3 (Not Intersecting): Passed\n";

    // --- AABB vs AABB Tests ---
    std::cout << "  Testing AABB vs AABB...\n";

    // Case 1: 충돌하는 경우 (겹침)
    AABB box1({0.0f, 0.0f}, {2.0f, 2.0f});
    AABB box2({1.0f, 1.0f}, {3.0f, 3.0f}); // X, Y 모두 겹침
    assert(AABB::AABBvsAABB(box1, box2) == false);
    std::cout << "    Test 1 (Intersecting): Passed\n";

    // Case 2: 정확히 접하는 경우 (X축)
    AABB box3({0.0f, 0.0f}, {1.0f, 1.0f});
    AABB box4({1.0f, 0.0f}, {2.0f, 1.0f}); // X축에서 접함
    assert(AABB::AABBvsAABB(box3, box4) == false);
    std::cout << "    Test 2 (Touching X): Passed\n";

    // Case 3: 정확히 접하는 경우 (Y축)
    AABB box5({0.0f, 0.0f}, {1.0f, 1.0f});
    AABB box6({0.0f, 1.0f}, {1.0f, 2.0f}); // Y축에서 접함
    assert(AABB::AABBvsAABB(box5, box6) == false);
    std::cout << "    Test 3 (Touching Y): Passed\n";

    // Case 4: 충돌하지 않는 경우 (X축 분리)
    AABB box7({0.0f, 0.0f}, {1.0f, 1.0f});
    AABB box8({2.0f, 0.0f}, {3.0f, 1.0f}); // X축에서 떨어짐
    assert(AABB::AABBvsAABB(box7, box8) == true);
    std::cout << "    Test 4 (Not Intersecting X): Passed\n";

    // Case 5: 충돌하지 않는 경우 (Y축 분리)
    AABB box9({0.0f, 0.0f}, {1.0f, 1.0f});
    AABB box10({0.0f, 2.0f}, {1.0f, 3.0f}); // Y축에서 떨어짐
    assert(AABB::AABBvsAABB(box9, box10) == true);
    std::cout << "    Test 5 (Not Intersecting Y): Passed\n";

    // --- 다른 테스트들 추가 가능 ---

    std::cout << "Physics Engine tests finished successfully.\n";
}

} // namespace CitadelPhysicsEngine2D