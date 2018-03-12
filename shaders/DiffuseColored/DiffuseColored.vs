#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;
layout (location = 5) in mat4 instanceMatrix;
layout (location = 9) in vec3 instanceDiffuse;

uniform mat4		uniform_mat_perspec_mult_view;

out                 vec2 tex_coord;
out                 vec3 diffuse_color;

void main (void)
{
    tex_coord = texCoord;
    diffuse_color = instanceDiffuse;
	gl_Position = uniform_mat_perspec_mult_view * instanceMatrix * vec4(pos, 1.0f);
}