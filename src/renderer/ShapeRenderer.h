#pragma once

#include "Mesh.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

#include "core/Transform.h"

namespace Citadel
{

class ShapeRenderer
{
public:
    void Init();

    void Shutdown();

    void Draw(const Mesh& mesh, const glm::vec2& position,
              const glm::vec2& size, float rotation, const glm::vec4& color);

    // using transform
    void Draw(const Mesh& mesh, const Transform& transform,
              const glm::vec4& color);

private:
    GLuint m_ShaderProgramID = 0;
    GLuint m_VertexShaderID = 0;
    GLuint m_FragmentShaderID = 0;

    // Render Item properties
    Transform m_Transform;
};

} // namespace Citadel