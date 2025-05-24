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

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, m_TriangleVertexShaderID);
    glAttachShader(shaderProgram, m_TriangleFragmentShaderID);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == false)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader Program Link Error: " << infoLog << std::endl;
        return false;
    }

    std::cout << "Shader Program Link Success" << std::endl;
    glDeleteShader(m_TriangleVertexShaderID);
    glDeleteShader(m_TriangleFragmentShaderID);

    // 3. VAO, VBO 초기화
    std::vector<Vec2> triangleVertices = CreateTriangleVertices();

    glGenVertexArrays(1, &m_TriangleVAO);
    glBindVertexArray(m_TriangleVAO);

    glGenBuffers(1, &m_TriangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_TriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, triangleVertices.size() * sizeof(Vec2),
                 triangleVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

void Renderer::BeginFrame()
{
    int display_w, display_h;
    glfwGetFramebufferSize(m_Window->GetNativeWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(m_ClearColor.x * m_ClearColor.w,
                 m_ClearColor.y * m_ClearColor.w,
                 m_ClearColor.z * m_ClearColor.w, m_ClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

std::vector<Vec2> Renderer::CreateTriangleVertices()
{
    using namespace CitadelPhysicsEngine2D;

    std::vector<Vec2> triangleVertices;

    float halfWidth = 0.5f;
    float height = 0.5f;

    Vec2 center = {m_Window->GetWidth() / 2.0f, m_Window->GetHeight() / 2.0f};

    // left bottom
    triangleVertices.push_back(
        {center.x - halfWidth, center.y - height / 2.0f});

    // right bottom
    triangleVertices.push_back(
        {center.x + halfWidth, center.y - height / 2.0f});

    // center top
    triangleVertices.push_back({center.x, center.y + height / 2.0f});

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
