#include "pch.h"

#include "ImGuiLayer.h"

#include "core/Window.h"

namespace Citadel
{

ImGuiLayer::ImGuiLayer() {}

ImGuiLayer::ImGuiLayer(const Window* window) { m_WindowHandle = window; }

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach()
{
    m_GlslVersion = "#version 410"; // macOS 호환성 고려 버전 4.1 지정

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
    if (ImGui_ImplGlfw_InitForOpenGL(m_WindowHandle->GetNativeWindow(), true) ==
        false)
    {
        std::cerr << "Failed to initialize ImGui GLFW backend" << std::endl;
        // return false;
    }
    if (ImGui_ImplOpenGL3_Init(m_GlslVersion) == false)
    {
        std::cerr << "Failed to initialize ImGui OpenGL3 backend" << std::endl;
        ImGui_ImplGlfw_Shutdown(); // GLFW 백엔드 정리
        // return false;
    }

    // return true;
}

void ImGuiLayer::OnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnUpdate(float deltaTime)
{
    // --- ImGui 새 프레임 ---
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

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

void ImGuiLayer::OnRender()
{
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
}

} // namespace Citadel
