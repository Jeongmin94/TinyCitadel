#include "pch.h"

#include "ShapeRenderer.h"

namespace Citadel
{

void ShapeRenderer::Init()
{
    // Shader 초기화
    std::string vertexShaderCode =
        ReadFileAsString("assets/shaders/triangle.vert");
    std::string fragmentShaderCode =
        ReadFileAsString("assets/shaders/triangle.frag");

    if (vertexShaderCode.empty() || fragmentShaderCode.empty())
    {
        return;
    }

    GLint success;
    GLchar infoLog[512];

    const char* vertexShaderCodePtr = vertexShaderCode.c_str();
    m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_VertexShaderID, 1, &vertexShaderCodePtr, nullptr);
    glCompileShader(m_VertexShaderID);

    glGetShaderiv(m_VertexShaderID, GL_COMPILE_STATUS, &success);
    if (success == false)
    {
        glGetShaderInfoLog(m_VertexShaderID, 512, nullptr, infoLog);
        std::cerr << "Vertex Shader Compilation Error: " << infoLog
                  << std::endl;
        return;
    }

    const char* fragmentShaderCodePtr = fragmentShaderCode.c_str();
    m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_FragmentShaderID, 1, &fragmentShaderCodePtr, nullptr);
    glCompileShader(m_FragmentShaderID);

    glGetShaderiv(m_FragmentShaderID, GL_COMPILE_STATUS, &success);
    if (success == false)
    {
        glGetShaderInfoLog(m_FragmentShaderID, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader Compilation Error: " << infoLog
                  << std::endl;
        return;
    }

    m_ShaderProgramID = glCreateProgram();
    glAttachShader(m_ShaderProgramID, m_VertexShaderID);
    glAttachShader(m_ShaderProgramID, m_FragmentShaderID);
    glLinkProgram(m_ShaderProgramID);

    glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &success);
    if (success == false)
    {
        glGetProgramInfoLog(m_ShaderProgramID, 512, nullptr, infoLog);
        std::cerr << "Shader Program Link Error: " << infoLog << std::endl;
        return;
    }

    std::cout << "Shader Program Link Success" << std::endl;
    glDeleteShader(m_VertexShaderID);
    glDeleteShader(m_FragmentShaderID);
}

void ShapeRenderer::Shutdown()
{
    if (m_ShaderProgramID != 0)
    {
        glDeleteProgram(m_ShaderProgramID);
        m_ShaderProgramID = 0;
    }
}

void ShapeRenderer::Draw(const Mesh& mesh, const glm::vec2& position,
                         const glm::vec2& size, float rotation,
                         const glm::vec4& color)
{
    // Use the shader program
    glUseProgram(m_ShaderProgramID);

    // Bind the VAO for the mesh
    glBindVertexArray(mesh.VAO);

    // Set uniforms for position, size, rotation, and color
    // This is a placeholder for actual uniform setting code.

    // Draw the mesh
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (void*)0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

std::string ShapeRenderer::ReadFileAsString(const std::string& filePath)
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
