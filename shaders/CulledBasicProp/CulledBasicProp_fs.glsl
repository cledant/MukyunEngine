#version 410 core

out vec4 color;

in GS_OUT
{
	vec2 TexCoords;
} fs_in;

uniform sampler2D		uniform_tex_diffuse;

void main (void)
{
	color = texture(uniform_tex_diffuse, fs_in.TexCoords);
	if (color.a < 0.5f)
	    discard;
}