#version 410 core

out vec4				color;

in vec2					vertex_tex;

uniform sampler2D		uniform_tex;

void main (void)
{
	color = texture(uniform_tex, vertex_tex);
}
