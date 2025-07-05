#pragma once

#include "imgui.h" // For ImVec4
#include <glad/glad.h>

#include "vector"

#include "Mesh.h"
#include "ShapeRenderer.h"

#include <CitadelPhysicsEngine2D/core.h>

namespace Citadel
{

using namespace CitadelPhysicsEngine2D;

class Window; // Forward declaration

class Renderer
{
public:
    Renderer();
    ~Renderer();

public:
    // 초기화/종료 (필요하다면 구현)
    bool Init(const Window* window);
    void Shutdown();

    // 프레임 시작 시 호출 (뷰포트 설정, 화면 클리어)
    void SetViewport();
    void BeginFrame();

    // 렌더링 함수
    void Render();

    // 클리어 색상 설정/조회
    void SetClearColor(const ImVec4& color) { m_ClearColor = color; }
    const ImVec4& GetClearColor() const { return m_ClearColor; }

private:
    ImVec4 m_ClearColor = {0.45f, 0.55f, 0.60f, 1.00f};

    const Window* m_Window;

private:
    Mesh m_Mesh;
    ShapeRenderer m_ShapeRenderer;
};

} // namespace Citadel