#include "pch.h"

#include "ShapeFactory.h"

namespace Citadel
{

Mesh ShapeFactory::CreateTriangle()
{
    Mesh mesh;

    GLuint indices[] = {0, 1, 2};
    std::vector<glm::vec3> vertices{
        glm::vec3(0.5f, -0.5f, 0.0f),  // right bottom
        glm::vec3(0.0f, 0.5f, 0.0f),   // center top
        glm::vec3(-0.5f, -0.5f, 0.0f), // left bottom
    };

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(GLuint),
                 indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    mesh.indexCount = 3;

    return mesh;
};

Mesh ShapeFactory::CreateCircle(GLuint segments)
{
    Mesh mesh;
    GLfloat radius = 0.5f;

    std::vector<glm::vec3> vertices;
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

    // vertices
    for (GLuint i = 0; i <= segments; i++)
    {
        GLfloat angle = (2 * M_PI) * ((GLfloat)i / (GLfloat)segments);

        GLfloat x = radius * cosf(angle);
        GLfloat y = radius * sinf(angle);
        GLfloat z = 0.0f;

        // counter clockwise
        vertices.push_back(glm::vec3(x, y, z));
    }

    // indices
    std::vector<GLuint> indices;
    for (GLuint i = 0; i < segments; i++)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }

    mesh.indexCount = indices.size();

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return mesh;
}

void ShapeFactory::DestroyMesh(Mesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.VAO);
    glDeleteBuffers(1, &mesh.VBO);
    if (mesh.EBO != 0)
    {
        glDeleteBuffers(1, &mesh.EBO);
    }
}

} // namespace Citadel
