#include <glad/glad.h> // GLAD는 GLFW보다 먼저 include 되어야 합니다.
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream> // 에러 출력용

// GLFW 에러 콜백 함수
void glfw_error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

int main(int, char **)
{
    // --- GLFW 초기화 ---
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // OpenGL 4.1 Core Profile 설정 (macOS 호환성)
    const char *glsl_version = "#version 410"; // GLSL 버전 명시
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS에서 필요
#endif

    // --- GLFW 윈도우 생성 ---
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Citadel Physics Engine 2D", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync 활성화

    // --- GLAD 초기화 ---
    // gladLoadGLLoader는 현재 OpenGL 컨텍스트가 설정된 후에 호출되어야 합니다.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // --- ImGui 초기화 ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 키보드 네비게이션 활성화 (옵션)
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // 도킹 활성화 (ImGui docking 브랜치 사용 시)
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // 멀티 뷰포트 활성화 (ImGui docking 브랜치 사용 시)

    ImGui::StyleColorsDark(); // 기본 다크 스타일 적용

    // ImGui Platform 및 Renderer 백엔드 초기화
    ImGui_ImplGlfw_InitForOpenGL(window, true); // 콜백 설치 활성화
    ImGui_ImplOpenGL3_Init(glsl_version);

    // --- 메인 루프 ---
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // 배경색

    while (!glfwWindowShouldClose(window))
    {
        // 이벤트 처리
        glfwPollEvents();

        // ImGui 새 프레임 시작
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- ImGui UI 렌더링 ---
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // 간단한 ImGui 창 시작

            ImGui::Text("This is some useful text.");                // 텍스트 출력
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // 슬라이더
            ImGui::ColorEdit3("clear color", (float *)&clear_color); // 색상 편집기

            if (ImGui::Button("Button")) // 버튼
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End(); // 창 끝
        }
        // ImGui::ShowDemoWindow(); // ImGui 데모 창 표시 (주석 해제하여 확인 가능)

        // --- 렌더링 ---
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);                                                                   // 프레임버퍼 크기 가져오기
        glViewport(0, 0, display_w, display_h);                                                                                   // 뷰포트 설정
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w); // 배경색 설정
        glClear(GL_COLOR_BUFFER_BIT);                                                                                             // 화면 클리어

        // ImGui 그리기
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // ImGui 뷰포트 처리 (도킹/뷰포트 활성화 시)
        // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        //     GLFWwindow* backup_current_context = glfwGetCurrentContext();
        //     ImGui::UpdatePlatformWindows();
        //     ImGui::RenderPlatformWindowsDefault();
        //     glfwMakeContextCurrent(backup_current_context);
        // }

        // 버퍼 스왑
        glfwSwapBuffers(window);
    }

    // --- 정리 ---
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}