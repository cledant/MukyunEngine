#version 410 core

#define NB_MAX_POINT_LIGHT 20

in vec4 FragPos;

struct PointLightDataGL
{
	vec4 pos;
	vec4 attenuation_coeff;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

layout (std140) uniform uniform_lightPos
{
	PointLightDataGL light[NB_MAX_POINT_LIGHT];
};

uniform float uniform_farPlane;
uniform int index;

void main()
{
    float lightDistance = length(FragPos.xyz - vec3(light[index].pos));

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / uniform_farPlane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}
