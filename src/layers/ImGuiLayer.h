#pragma once

#include "Layer.h"

#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Window;

namespace Citadel
{

class Window;

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ImGuiLayer(const std::string& name, const Window* window);
    ~ImGuiLayer() override;

public:
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;

private:
    const Window* m_WindowHandle = nullptr;
    const char* m_GlslVersion = "#version 410";
    ImVec4 m_ClearColor = {0.45f, 0.55f, 0.60f, 1.00f}; // 배경색
};

} // namespace Citadel
