#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;
layout (location = 5) in mat4 instanceMatrix;

layout (std140) uniform uniform_mat_perspec_mult_view
{
	mat4	mat_perspec_mult_view;
};

void main (void)
{
	gl_Position = mat_perspec_mult_view * instanceMatrix * vec4(pos, 1.0f);
}