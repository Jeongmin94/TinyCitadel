#pragma once

#include "Mesh.h"

namespace Citadel
{

/**
 * Mesh created by ShapeFactory has counter clockwise winding order.
 */
class ShapeFactory
{
public:
    static Mesh CreateTriangle();

    static Mesh CreateRectangle(GLfloat width, GLfloat height);

    static Mesh CreateCircle(GLuint segments = 50, GLfloat radius = 0.5f);

    static void DestroyMesh(Mesh& mesh);
};

} // namespace Citadel