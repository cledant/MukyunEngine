#version 410 core

out vec4				color;

in vec2					tex_coord;
in vec3                 diffuse_color;

uniform sampler2D		uniform_tex_diffuse;

void main (void)
{
	color = vec4(diffuse_color, 1.0);

	vec4 useless = texture(uniform_tex_diffuse, tex_coord);
	useless *= vec4(2.0);

	if (color.a < 0.5f)
	    discard;
}