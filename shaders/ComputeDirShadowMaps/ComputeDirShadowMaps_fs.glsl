#version 410 core

#define NB_MAX_DIR_LIGHT 10

out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 Tex_coord;
    vec4 FragPosLightSpace;
} fs_in;

struct DirLightDataGL
{
    vec4 pos;
	vec4 dir;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

uniform sampler2D shadowMap;

layout (std140) uniform uniform_DirLight
{
    DirLightDataGL dir_light[NB_MAX_DIR_LIGHT];
};

uniform int i;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(vec3(dir_light[i].pos) - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return (1.0 - shadow);
}

void main()
{
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    FragColor = vec4(vec3(shadow), 1.0);
}
