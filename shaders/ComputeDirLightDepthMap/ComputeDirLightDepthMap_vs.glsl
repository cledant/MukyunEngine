#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;
layout (location = 5) in mat4 instanceMatrix;
layout (location = 9) in mat4 instanceInverseMatrix;

layout (std140) uniform uniform_lightSpaceMatrix
{
	mat4 lightSpaceMatrix;
};

void main()
{
    gl_Position = lightSpaceMatrix * instanceMatrix * vec4(pos, 1.0);
}
