#version 410 core

out vec4		color;

uniform vec3    uniform_light_diffuse;

void main (void)
{
	color = vec4(uniform_light_diffuse, 1.0);
}