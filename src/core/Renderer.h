#pragma once

#include "imgui.h" // For ImVec4

namespace Citadel
{

class Window; // Forward declaration

class Renderer
{
public:
    Renderer();
    ~Renderer();

public:
    // 초기화/종료 (필요하다면 구현)
    // void Init();
    // void Shutdown();

    // 프레임 시작 시 호출 (뷰포트 설정, 화면 클리어)
    void BeginFrame(const Window& window);

    // 클리어 색상 설정/조회
    void SetClearColor(const ImVec4& color) { m_ClearColor = color; }
    const ImVec4& GetClearColor() const { return m_ClearColor; }

private:
    ImVec4 m_ClearColor = {0.45f, 0.55f, 0.60f, 1.00f};
};

} // namespace Citadel