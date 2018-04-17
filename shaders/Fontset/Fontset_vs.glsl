#version 410 core

layout (location = 0) in vec4 vertex;

uniform mat4		uniform_mat_proj;

out                 vec2 vertex_tex;

void main (void)
{
    vertex_tex = vertex.zw;
	gl_Position = uniform_mat_proj * vec4(vertex.xy, 0.0f, 1.0f);
}
