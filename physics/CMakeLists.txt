set(PHYSICS_LIB CitadelPhysicsEngine2D)

add_library(${PHYSICS_LIB} STATIC)

# 물리 엔진 소스 파일 추가
target_sources(${PHYSICS_LIB} PRIVATE
    src/core.cpp
)

target_include_directories(${PHYSICS_LIB} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(${PHYSICS_LIB} PUBLIC cxx_std_17)