#pragma once

#include <algorithm>
#include <filesystem> // std::filesystem::current_path() C++17 이상
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// clang-format on

#include <CitadelPhysicsEngine2D/core.h>
