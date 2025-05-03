#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Window.h"

#include <memory>

class Window;

namespace Citadel
{

class Application
{
public:
    Application();
    virtual ~Application();

private:
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

public:
    bool Init();
    void Run();
    void Shutdown(); // 명시적 호출 가능, 소멸자에서 자동 호출됨

private:
    // ImGui 관련 초기화/종료는 Application 에서 처리
    bool InitImGui();
    void ShutdownImGui();

    void RenderUI();

private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    bool m_Initialized = false;
    const char* m_GlslVersion = nullptr;                // ImGui 초기화 시 필요
    ImVec4 m_ClearColor = {0.45f, 0.55f, 0.60f, 1.00f}; // 배경색
};

} // namespace Citadel
