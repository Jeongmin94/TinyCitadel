#pragma once

/**
 * Mesh
 * - 렌더링에 필요한 OpenGL ID를 담기 위한 구조체
 * - 데이터 컨테이너의 역할을 담당함
 */

#include <glad/glad.h>

struct Mesh
{
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    GLuint indexCount = 0;
};