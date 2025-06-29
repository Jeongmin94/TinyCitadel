#include "pch.h"

#include "Application.h"
#include "LayerStack.h"
#include "layers/ImGuiLayer.h"

namespace Citadel
{

Application::Application()
{
    m_Renderer = std::make_unique<Renderer>();
    m_Window = std::make_unique<Window>();
}

Application::~Application()
{
    Shutdown(); // 소멸 시 자동으로 정리
}

bool Application::Init()
{
    // --- 현재 작업 디렉토리 출력 ---
    std::cout << "Current Working Directory: "
              << std::filesystem::current_path() << std::endl;
    // -------------------------------

    if (m_Initialized == true)
    {
        std::cerr << "Application already initialized!" << std::endl;
        return true;
    }

    // 1. Window 초기화 (GLFW, GLAD 포함)
    if (m_Window == nullptr || m_Window->Init() == false)
    {
        std::cerr << "Failed to initialize Window" << std::endl;
        return false;
    }
    std::cout << "Window Initialized" << std::endl;

    // 2. Renderer 초기화
    // - window 초기화
    // - shader 초기화
    // - VAO, VBO 초기화
    if (m_Renderer == nullptr || m_Renderer->Init(m_Window.get()) == false)
    {
        std::cerr << "Failed to initialize Renderer" << std::endl;
        return false;
    }
    std::cout << "Renderer Initialized" << std::endl;

    // 3. LayerStack 초기화
    // m_LayerStack.PushOverlay(
    //     std::make_unique<ImGuiLayer>("ImGuiLayer", m_Window.get()));

    // // 4. LayerStack 검증
    // if (m_LayerStack.ValidateLayers() == false)
    // {
    //     std::cerr << "Failed to validate LayerStack" << std::endl;
    //     return false;
    // }

    m_Initialized = true;
    m_Running = true;
    return true;
}

void Application::Run()
{
    if (m_Initialized == false)
    {
        std::cerr << "Application not initialized. Call Init() first."
                  << std::endl;
        return;
    }

    while (m_Running == true && m_Window->ShouldClose() == false)
    {
        m_Renderer->SetViewport();

        m_Renderer->BeginFrame();

        for (auto& layer : m_LayerStack)
        {
            layer->OnUpdate(0.0f);
        }

        for (auto& layer : m_LayerStack)
        {
            layer->OnRender();
        }
        m_Renderer->RenderTriangle();

        // --- Window 업데이트 (이벤트 폴링 + 버퍼 스왑) ---
        m_Window->OnUpdate();
    }
}

void Application::Shutdown()
{
    if (!m_Initialized)
        return;

    std::cout << "Starting Application Shutdown..." << std::endl;

    // 1. 레이어 스택 정리 (모든 레이어의 OnDetach 호출)
    m_LayerStack.Clear();

    // 2. Window 종료 (GLFW 종료 포함)
    if (m_Window != nullptr)
    {
        m_Window->Shutdown();
        m_Window.reset(); // unique_ptr 명시적 리셋
        std::cout << "Window Shutdown" << std::endl;
    }

    // 3. Renderer 종료 (필요하다면)
    if (m_Renderer != nullptr)
    {
        // m_Renderer->Shutdown(); // Renderer에 종료 로직이 있다면 호출
        m_Renderer.reset(); // unique_ptr 명시적 리셋
        std::cout << "Renderer Shutdown" << std::endl;
    }

    m_Running = false;
    m_Initialized = false;
    std::cout << "Application Shutdown Complete" << std::endl;
}

} // namespace Citadel