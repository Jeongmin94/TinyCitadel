#pragma once

#include "Mesh.h"

namespace Citadel
{

class ShapeFactory
{
public:
    static Mesh CreateTriangle();

    static Mesh CreateCircle(int segments = 50);

    static void DestroyMesh(Mesh& mesh);
};

} // namespace Citadel