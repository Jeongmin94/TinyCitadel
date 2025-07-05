#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_model;
uniform mat4 u_projection;

void main()
{
    // gl_Position: 내장 변수로 버텍스 쉐이더의 출력 변수로 사용됨
    // 해당 변수에 저장된 값이 클립 공간 좌표로 출력되어 나머지 쉐이더에서 사용

    gl_Position = u_projection * u_model * vec4(aPos, 1.0);
    // gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
