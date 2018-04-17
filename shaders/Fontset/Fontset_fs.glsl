#version 410 core

out vec4				color;

in vec2					vertex_tex;

uniform sampler2D		uniform_tex;
uniform vec3            uniform_color;

void main (void)
{
	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(uniform_tex, vertex_tex).r);
	color = vec4(uniform_color, 1.0) * sampled;
}
