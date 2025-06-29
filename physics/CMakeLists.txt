set(PHYSICS_LIB CitadelPhysicsEngine2D)

add_library(${PHYSICS_LIB} STATIC)

file(GLOB PHYSICS_CORE_SOURCE "src/*.cpp")
file(GLOB PHYSICS_SHAPES_SOURCE "src/shapes/*.cpp")
file(GLOB PHYSICS_MATH_SOURCE "src/math/*.cpp")

# 물리 엔진 소스 파일 추가
target_sources(${PHYSICS_LIB} PRIVATE
    ${PHYSICS_CORE_SOURCE}
    ${PHYSICS_SHAPES_SOURCE}
    ${PHYSICS_MATH_SOURCE}
)

target_include_directories(${PHYSICS_LIB} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
    $<INSTALL_INTERFACE:include>
)

target_compile_features(${PHYSICS_LIB} PUBLIC cxx_std_17)