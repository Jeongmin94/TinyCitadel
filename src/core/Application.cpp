#include <glad/glad.h> // GLAD를 가장 먼저 include

#include "Application.h"

#include <GLFW/glfw3.h> // For GetNativeWindow return type

#include <iostream>

namespace Citadel
{

Application::Application()
{
    // 생성자에서 Window 생성
    m_Window = std::make_unique<Window>();
}

Application::~Application()
{
    Shutdown(); // 소멸 시 자동으로 정리
}

bool Application::Init()
{
    if (m_Initialized)
    {
        std::cerr << "Application already initialized!" << std::endl;
        return true;
    }

    // 1. Window 초기화 (GLFW, GLAD 포함)
    if (!m_Window || !m_Window->Init())
    {
        std::cerr << "Failed to initialize Window" << std::endl;
        return false;
    }
    std::cout << "Window Initialized" << std::endl;

    // 2. ImGui 초기화
    if (!InitImGui())
    {
        std::cerr << "Failed to initialize ImGui" << std::endl;
        m_Window->Shutdown(); // Window 정리
        return false;
    }
    std::cout << "ImGui Initialized" << std::endl;

    m_Initialized = true;
    m_Running = true;
    return true;
}

bool Application::InitImGui()
{
    // GLSL 버전 설정 (Window 초기화 후 OpenGL 버전 확인 가능하지만, 여기선
    // 고정값 사용)
    m_GlslVersion = "#version 410"; // macOS 호환성 고려

    // ImGui 컨텍스트 생성
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    // Platform/Renderer 백엔드 초기화
    if (!ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true))
    {
        std::cerr << "Failed to initialize ImGui GLFW backend" << std::endl;
        return false;
    }
    if (!ImGui_ImplOpenGL3_Init(m_GlslVersion))
    {
        std::cerr << "Failed to initialize ImGui OpenGL3 backend" << std::endl;
        ImGui_ImplGlfw_Shutdown(); // GLFW 백엔드 정리
        return false;
    }

    return true;
}

void Application::Run()
{
    if (!m_Initialized)
    {
        std::cerr << "Application not initialized. Call Init() first."
                  << std::endl;
        return;
    }

    while (m_Running && !m_Window->ShouldClose())
    {
        // --- 렌더링 준비 ---
        int display_w, display_h;
        glfwGetFramebufferSize(m_Window->GetNativeWindow(), &display_w,
                               &display_h);     // 프레임버퍼 크기
        glViewport(0, 0, display_w, display_h); // 뷰포트 설정
        glClearColor(m_ClearColor.x * m_ClearColor.w,
                     m_ClearColor.y * m_ClearColor.w,
                     m_ClearColor.z * m_ClearColor.w, m_ClearColor.w); // 배경색
        glClear(GL_COLOR_BUFFER_BIT);

        // --- ImGui 새 프레임 ---
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- UI 렌더링 ---
        RenderUI();
        // ImGui::ShowDemoWindow(); // 데모 창 필요 시 활성화

        // --- ImGui 그리기 ---
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // --- ImGui 뷰포트 처리 (Docking/Viewport 활성화 시) ---
        // ImGuiIO& io = ImGui::GetIO();
        // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        //     GLFWwindow* backup_current_context = glfwGetCurrentContext();
        //     ImGui::UpdatePlatformWindows();
        //     ImGui::RenderPlatformWindowsDefault();
        //     glfwMakeContextCurrent(backup_current_context);
        // }

        // --- Window 업데이트 (이벤트 폴링 + 버퍼 스왑) ---
        m_Window->OnUpdate();
    }
}

void Application::RenderUI()
{
    // 여기에 ImGui UI 코드 작성 (기존 main의 ImGui 블록)
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, Citadel!");
    ImGui::Text("This UI is rendered by the Application class.");
    ImGui::SliderFloat("Float Slider", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("Clear Color", (float*)&m_ClearColor);

    if (ImGui::Button("Click Me!"))
        counter++;
    ImGui::SameLine();
    ImGui::Text("Counter = %d", counter);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

void Application::Shutdown()
{
    if (!m_Initialized)
        return;

    // 1. ImGui 종료
    ShutdownImGui();
    std::cout << "ImGui Shutdown" << std::endl;

    // 2. Window 종료 (GLFW, GLAD 포함)
    if (m_Window)
    {
        m_Window->Shutdown();
        // m_Window unique_ptr가 자동으로 메모리 해제
        std::cout << "Window Shutdown" << std::endl;
    }

    m_Running = false;
    m_Initialized = false;
    std::cout << "Application Shutdown Complete" << std::endl;
}

void Application::ShutdownImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

} // namespace Citadel