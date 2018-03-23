#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord;

out vec2 vertex_tex;

void main (void)
{
    vertex_tex = tex_coord;
	gl_Position = vec4(pos, 1.0);
}
