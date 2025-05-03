#pragma once

namespace CitadelPhysicsEngine2D
{

struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}

    // 여기에 필요한 다른 Vec2 관련 연산자나 함수들을 추가할 수 있습니다.
    // 예: Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y
    // + other.y); } 예: float Length() const { return /* 길이 계산 */; }
};

// 여기에 다른 수학 관련 타입 (예: Mat2x2 등)을 추가할 수도 있습니다.

} // namespace CitadelPhysicsEngine2D