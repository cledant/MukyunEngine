#version 410 core

out vec4				color;

in vec2					tex_coord;

uniform sampler2D		uniform_tex_diffuse;

void main (void)
{
	color = texture(uniform_tex_diffuse, tex_coord);
	if (color.a < 0.5f)
	    discard;
}