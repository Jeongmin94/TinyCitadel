#include "pch.h"

#include "Renderer.h"

#include "Window.h"

namespace Citadel
{

Renderer::Renderer() {}

Renderer::~Renderer() {}

bool Renderer::Init(const Window* window)
{
    // 1. Window 초기화
    m_Window = window;

    // 2. Shader 초기화
    std::string vertexShaderCode =
        ReadFileAsString("assets/shaders/triangle.vert");
    std::string fragmentShaderCode =
        ReadFileAsString("assets/shaders/triangle.frag");

    if (vertexShaderCode.empty() || fragmentShaderCode.empty())
    {
        return false;
    }

    GLint success;
    GLchar infoLog[512];

    const char* vertexShaderCodePtr = vertexShaderCode.c_str();
    m_TriangleVertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_TriangleVertexShaderID, 1, &vertexShaderCodePtr, nullptr);
    glCompileShader(m_TriangleVertexShaderID);

    glGetShaderiv(m_TriangleVertexShaderID, GL_COMPILE_STATUS, &success);
    if (success == false)
    {
        glGetShaderInfoLog(m_TriangleVertexShaderID, 512, nullptr, infoLog);
        std::cerr << "Vertex Shader Compilation Error: " << infoLog
                  << std::endl;
        return false;
    }

    const char* fragmentShaderCodePtr = fragmentShaderCode.c_str();
    m_TriangleFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_TriangleFragmentShaderID, 1, &fragmentShaderCodePtr,
                   nullptr);
    glCompileShader(m_TriangleFragmentShaderID);

    glGetShaderiv(m_TriangleFragmentShaderID, GL_COMPILE_STATUS, &success);
    if (success == false)
    {
        glGetShaderInfoLog(m_TriangleFragmentShaderID, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader Compilation Error: " << infoLog
                  << std::endl;
        return false;
    }

    m_ShaderProgramID = glCreateProgram();
    glAttachShader(m_ShaderProgramID, m_TriangleVertexShaderID);
    glAttachShader(m_ShaderProgramID, m_TriangleFragmentShaderID);
    glLinkProgram(m_ShaderProgramID);

    glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &success);
    if (success == false)
    {
        glGetProgramInfoLog(m_ShaderProgramID, 512, nullptr, infoLog);
        std::cerr << "Shader Program Link Error: " << infoLog << std::endl;
        return false;
    }

    std::cout << "Shader Program Link Success" << std::endl;
    glDeleteShader(m_TriangleVertexShaderID);
    glDeleteShader(m_TriangleFragmentShaderID);

    // 3. VAO, VBO 초기화
    std::vector<glm::vec2> triangleVertices = CreateTriangleVertices();

    glGenVertexArrays(1, &m_TriangleVAO);
    glBindVertexArray(m_TriangleVAO);

    glGenBuffers(1, &m_TriangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_TriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, triangleVertices.size() * sizeof(glm::vec2),
                 triangleVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

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

void Renderer::RenderTriangle()
{
    // After BeginFrame

    // 1. 뷰포트 설정
    // BeginFrame에서 설정됨

    // 2. 쉐이더 프로그램 활성화

    glUseProgram(m_ShaderProgramID);

    // 3. VAO 활성화

    glBindVertexArray(m_TriangleVAO);

    // 4. 드로우 콜

    glDrawArrays(GL_TRIANGLES, 0, 3);

    // 5. 해제

    glBindVertexArray(0);
    glUseProgram(0);
}

std::vector<glm::vec2> Renderer::CreateTriangleVertices()
{
    using namespace CitadelPhysicsEngine2D;

    std::vector<glm::vec2> triangleVertices;

    triangleVertices.push_back({-0.25f, -0.25f}); // 왼쪽 아래
    triangleVertices.push_back({0.25f, -0.25f});  // 오른쪽 아래
    triangleVertices.push_back({0.0f, 0.25f});    // 위쪽 중앙

    return triangleVertices;
}

std::string Renderer::ReadFileAsString(const std::string& filePath)
{
    std::ifstream inputFileStream(filePath);

    if (inputFileStream.is_open() == false)
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return std::string();
    }

    std::stringstream stringStream;
    stringStream << inputFileStream.rdbuf();
    inputFileStream.close();

    return stringStream.str();
}

} // namespace Citadel
