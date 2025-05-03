#include "pch.h"

#include "Renderer.h"

#include "Window.h"

namespace Citadel
{

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::BeginFrame(const Window& window)
{
    int display_w, display_h;
    glfwGetFramebufferSize(window.GetNativeWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(m_ClearColor.x * m_ClearColor.w,
                 m_ClearColor.y * m_ClearColor.w,
                 m_ClearColor.z * m_ClearColor.w, m_ClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace Citadel
