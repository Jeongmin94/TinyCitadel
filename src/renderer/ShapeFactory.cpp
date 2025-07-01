#include "pch.h"

#include "ShapeFactory.h"

namespace Citadel
{

Mesh ShapeFactory::CreateTriangle()
{
    Mesh mesh;
    mesh.drawCount = 3;

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom left
        0.0f,  0.5f,  0.0f, // top middle
        0.5f,  -0.5f, 0.0f  // bottom right
    };

    unsigned int indices[] = {0, 1, 2};

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return mesh;
};

Mesh ShapeFactory::CreateCircle(int segments) { return Mesh(); }

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
