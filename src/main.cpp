#include "pch.h"

#include "core/Application.h"

namespace CitadelPhysicsEngine2D
{
void RunPhysicsTests();
}

int main(int argc, char** argv)
{
    CitadelPhysicsEngine2D::RunPhysicsTests();

    Citadel::Application app;

    try
    {
        if (app.Init())
        {
            app.Run();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unhandled Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown unhandled exception" << std::endl;
        return EXIT_FAILURE;
    }

    // app 객체가 범위를 벗어나면 소멸자가 자동으로 Shutdown() 호출
    return EXIT_SUCCESS;
}