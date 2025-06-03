#include "pch.h"

#include "Window.h"

namespace Citadel
{

bool Window::s_GLFWInitialized = false;

void Window::GlfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

Window::Window(const WindowProps& props) : m_Data(props) {}

Window::~Window()
{
    Shutdown(); // 소멸 시 자동으로 정리
}

bool Window::Init()
{
    if (m_Initialized == true)
    {
        std::cerr << "Window already initialized!" << std::endl;
        return true;
    }

    std::cout << "Creating window " << m_Data.Title << " (" << m_Data.Width
              << ", " << m_Data.Height << ")" << std::endl;

    // --- GLFW 초기화 ---
    if (s_GLFWInitialized == false)
    {
        glfwSetErrorCallback(GlfwErrorCallback);
        if (glfwInit() == false)
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }
        s_GLFWInitialized = true;
        std::cout << "GLFW Initialized" << std::endl;
    }

    // OpenGL 4.1 Core Profile 설정 (macOS 호환성)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS에서 필요
#endif

    // --- GLFW 윈도우 생성 ---
    m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height,
                                m_Data.Title.c_str(), NULL, NULL);
    if (m_Window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        // GLFW 초기화 성공 후 창 생성 실패 시 GLFW 종료는
        // Application::Shutdown에서 처리
        return false;
    }
    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1); // VSync 활성화

    // --- GLAD 초기화 (GLFW 창 생성 및 컨텍스트 설정 후) ---
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false)
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
        return false;
    }
    std::cout << "GLAD Initialized" << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // 콜백에서 데이터 접근용
    glfwSetWindowUserPointer(m_Window, &m_Data);

    // 필요한 콜백 설정 (예: 창 크기 변경)
    // // 물리 사이즈 변경
    // glfwSetFramebufferSizeCallback(
    //     m_Window,
    //     [](GLFWwindow* window, int width, int height)
    //     {
    //         glViewport(0, 0, width, height);
    //         std::cout << "Framebuffer resized via callback: (" << width << ",
    //         "
    //                   << height << ")" << std::endl;
    //     });

    // // 논리 사이즈 변경
    // glfwSetWindowSizeCallback(m_Window,
    //                           [](GLFWwindow* window, int width, int height)
    //                           {
    //                               std::cout << "Window resized via callback:
    //                               ("
    //                                         << width << ", " << height << ")"
    //                                         << std::endl;
    //                           });

    m_Initialized = true;
    return true;
}

void Window::Shutdown()
{
    if (m_Initialized == false)
        return;

    if (m_Window != nullptr)
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
        std::cout << "GLFW Window Destroyed" << std::endl;
    }

    // 애플리케이션 종료 시 한번만 호출되도록 관리 필요
    // 만약 여러 윈도우를 관리한다면 s_GLFWInitialized 관리가 더 복잡해짐
    if (s_GLFWInitialized == true)
    {
        glfwTerminate();
        s_GLFWInitialized = false;
        std::cout << "GLFW Terminated" << std::endl;
    }
    m_Initialized = false;
}

void Window::OnUpdate()
{
    if (m_Initialized == false)
        return;

    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

bool Window::ShouldClose() const
{
    if (m_Initialized == false || m_Window == nullptr)
        return true;

    return glfwWindowShouldClose(m_Window);
}

} // namespace Citadel
