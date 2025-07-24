#include "core/Transform.h"
#include "pch.h"

#include "ShapeRenderer.h"

#include "utils/FileUtils.h"

namespace Citadel
{

void ShapeRenderer::Init()
{
    // Shader 초기화
    // TODO: 쉐이더를 선택해서 받을 수 있게 해야함
    std::string vertexShaderCode =
        FileUtils::ReadFileAsString("assets/shaders/triangle.vert");
    std::string fragmentShaderCode =
        FileUtils::ReadFileAsString("assets/shaders/triangle.frag");

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

    // Test
    {
        // transform
        m_Transform = Transform::Default();
    }
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
    {
        // model(scale, rotate, translate)
        // OpenGL - column major order
        glm::mat4 model = glm::mat4(1.0f); // identity matrix

        model = glm::translate(model, glm::vec3(position, 0.0f));

        // glm::rotate(model, radian, axis)
        // - 2D라서 Z 기준으로만 회전
        model = glm::rotate(model, glm::radians(rotation),
                            glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(size, 1.0f));

        // projection
        float aspectRatio = 1280.0f / 720.0f;
        glm::mat4 projection =
            glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);

        glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, "u_model"),
                           1, GL_FALSE, &model[0][0]);

        glUniformMatrix4fv(
            glGetUniformLocation(m_ShaderProgramID, "u_projection"), 1,
            GL_FALSE, &projection[0][0]);

        glUniform4fv(glGetUniformLocation(m_ShaderProgramID, "u_color"), 1,
                     &color[0]);
    }

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

void ShapeRenderer::Draw(const Mesh& mesh, const Transform& transform,
                         const glm::vec4& color)
{
    ShapeRenderer::Draw(mesh, transform.m_Position, transform.m_Scale,
                        transform.m_Rotation, color);
}

} // namespace Citadel
