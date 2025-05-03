#include "pch.h"

#include "core/Application.h"

#include <CitadelPhysicsEngine2D/core.h>

int main(int argc, char** argv)
{
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