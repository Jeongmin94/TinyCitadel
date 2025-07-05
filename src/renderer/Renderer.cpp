#include "pch.h"

#include "Renderer.h"
#include "ShapeFactory.h"

#include "core/Window.h"

namespace Citadel
{

Renderer::Renderer() {}

Renderer::~Renderer() {}

bool Renderer::Init(const Window* window)
{
    // 1. Window 초기화
    m_Window = window;

    // m_Mesh = ShapeFactory::CreateTriangle();
    m_Mesh = ShapeFactory::CreateCircle();
    m_ShapeRenderer.Init();

    return true;
}

void Renderer::Shutdown() { m_ShapeRenderer.Shutdown(); }

void Renderer::SetViewport()
{
    int display_w, display_h;
    glfwGetFramebufferSize(m_Window->GetNativeWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
}

void Renderer::BeginFrame()
{
    glClearColor(m_ClearColor.x * m_ClearColor.w,
                 m_ClearColor.y * m_ClearColor.w,
                 m_ClearColor.z * m_ClearColor.w, m_ClearColor.w);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::Render()
{
    m_ShapeRenderer.Draw(m_Mesh, {0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f,
                         {1.0f, 0.0f, 0.0f, 1.0f});
}

} // namespace Citadel
