#pragma once

#include "LayerStack.h"
#include "Renderer.h"
#include "Window.h"

#include <memory>

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
    bool m_Running = true;
    bool m_Initialized = false;

    LayerStack m_LayerStack;

    std::unique_ptr<Window> m_Window;
    std::unique_ptr<Renderer> m_Renderer;

    Layer* m_ImGuiLayer;
};

} // namespace Citadel
