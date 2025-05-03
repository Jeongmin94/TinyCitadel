#pragma once

#include <string>

// Forward declaration
struct GLFWwindow;

namespace Citadel
{

// Window 속성 구조체 (클래스 밖으로 이동)
struct WindowProps
{
    std::string Title = "Citadel Physics Engine 2D";
    unsigned int Width = 1280;
    unsigned int Height = 720;
};

class Window
{
public:
    Window(const WindowProps& props = WindowProps());
    virtual ~Window();

    // 복사 및 이동 금지
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    bool Init();
    void Shutdown();

    void OnUpdate(); // PollEvents + SwapBuffers

    bool ShouldClose() const;
    GLFWwindow* GetNativeWindow() const { return m_Window; }
    unsigned int GetWidth() const { return m_Data.Width; }
    unsigned int GetHeight() const { return m_Data.Height; }

private:
    static void GlfwErrorCallback(int error, const char* description);

    GLFWwindow* m_Window = nullptr;
    WindowProps m_Data; // 구조체 인스턴스 멤버
    bool m_Initialized = false;

    static bool s_GLFWInitialized; // 여러 Window 인스턴스 방지용
};

} // namespace Citadel
