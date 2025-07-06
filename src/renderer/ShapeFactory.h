#pragma once

#include "Mesh.h"

namespace Citadel
{

class ShapeFactory
{
public:
    static Mesh CreateTriangle();

    static Mesh CreateCircle(GLuint segments = 50, GLfloat radius = 0.5f);

    static void DestroyMesh(Mesh& mesh);
};

} // namespace Citadel