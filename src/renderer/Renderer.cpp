#include "core/Transform.h"
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

    // !TODO - 렌더링 오브젝트 초기화 작업 분리
    m_Mesh = ShapeFactory::CreateTriangle();
    // m_Mesh = ShapeFactory::CreateCircle();
    m_Transform = Transform::Default();
    m_Color = glm::vec4(1.0, 0.0f, 1.0f, 1.0f);

    m_Rect = ShapeFactory::CreateRectangle(0.5f, 0.5f);
    m_RectTransform =
        Transform::Of(glm::vec2(1.0f, 1.0f), 45.0f, glm::vec2(0.5f, 0.0f));
    m_RectColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

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

    // clear color, depth, stencil buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::Render()
{
    // using transform ver
    m_ShapeRenderer.Draw(m_Mesh, m_Transform, m_Color);
    m_ShapeRenderer.Draw(m_Rect, m_RectTransform, m_RectColor);
}

} // namespace Citadel
