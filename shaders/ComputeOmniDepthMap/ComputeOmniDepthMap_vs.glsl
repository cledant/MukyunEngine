#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;
layout (location = 5) in mat4 instanceMatrix;
layout (location = 9) in mat4 instanceInverseMatrix;

void main()
{
    gl_Position = instanceMatrix * vec4(pos, 1.0);
}
