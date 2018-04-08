#version 410 core

#define NB_MAX_POINT_LIGHT 20

out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 Tex_coord;
} fs_in;

struct PointLightDataGL
{
	vec4 pos;
	vec4 attenuation_coeff;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

uniform samplerCube depthMap;

layout (std140) uniform uniform_lightPos
{
    PointLightDataGL point[NB_MAX_POINT_LIGHT];
};

uniform float uniform_farPlane;

layout (std140) uniform uniform_view_pos
{
	vec3	viewPos;
};

uniform int i;

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - vec3(point[i].pos);

    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(vec3(point[i].pos) - fs_in.FragPos);
    float bias = tan(acos(dot(normal, lightDir))) * 0.005;
    bias *= uniform_farPlane;

    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / uniform_farPlane)) / 25.0;

    float shadow = 0.0;
    int samples = 20;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= uniform_farPlane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return (1.0 - shadow);
}

void main()
{
    float shadow = ShadowCalculation(fs_in.FragPos);
    FragColor = vec4(vec3(shadow), 1.0);
}
