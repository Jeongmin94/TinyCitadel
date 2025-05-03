cmake_minimum_required(VERSION 3.15) # CMake 버전에 맞게 조정

project(TinyCitadel LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17) # C++ 표준 설정 (필요에 따라 변경)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 의존성 찾기 ---

# GLFW 찾기 (Homebrew 경로에서 찾도록 시도)
find_package(glfw3 REQUIRED)

# OpenGL 찾기 (macOS 기본 제공)
find_package(OpenGL REQUIRED)

# --- GLAD 설정 ---
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad)
add_library(glad STATIC ${GLAD_DIR}/src/glad.c) # glad.c 를 정적 라이브러리로 빌드
target_include_directories(glad PUBLIC ${GLAD_DIR}/include) # glad 헤더 경로 추가

# --- ImGui 설정 ---
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    # ImGui 백엔드 (GLFW + OpenGL3) 추가
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# ImGui 백엔드가 GLAD를 사용하도록 컴파일 매크로(-D) 추가
target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
# GLAD와 GLFW 헤더/라이브러리를 ImGui에서 사용할 수 있도록 링크
target_link_libraries(imgui PUBLIC glad glfw)

# --- 물리 엔진 설정 ---
add_subdirectory(physics)

# --- 애플리케이션 설정 ---
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/core/Application.cpp
    src/core/Window.cpp
    src/core/Layer.cpp
    src/core/LayerStack.cpp
    src/layers/ImGuiLayer.cpp
    src/core/Renderer.cpp
)

# 애플리케이션에 필요한 include 경로 추가
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    # ${GLFW_INCLUDE_DIRS} # glfw3::glfw 링크 통해 상속됨
    # ${GLAD_DIR}/include # glad 링크 통해 상속됨
    # ${IMGUI_DIR} # imgui 링크 통해 상속됨
    # ${IMGUI_DIR}/backends # imgui 링크 통해 상속됨

    # 명시적으로 필요한 다른 경로가 있다면 여기에 추가
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h"
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    imgui
    ${OPENGL_LIBRARIES}
    CitadelPhysicsEngine2D
)

# --- 플랫폼별 설정 (macOS) ---
if(APPLE)
    # macOS 프레임워크 링크
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa -framework IOKit -framework OpenGL -framework CoreVideo")
endif()