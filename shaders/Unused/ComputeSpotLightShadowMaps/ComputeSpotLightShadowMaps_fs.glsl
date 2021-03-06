#version 410 core

#define SHADOW_ACCENTUATION 0.2

out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 Tex_coord;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D shadowMap;

uniform vec3 uniform_lightPos;
uniform vec3 uniform_lightDir;
uniform vec2 uniform_cutoff;

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
    vec3 lightDir = normalize(uniform_lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            // check whether current frag pos is in shadow
            shadow += currentDepth - bias > pcfDepth  ? 1.0 - (closestDepth - SHADOW_ACCENTUATION) : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;

    //SpotLight
    float theta = dot(lightDir, normalize(-uniform_lightDir));
    float epsilon = uniform_cutoff.y - uniform_cutoff.x;
    float intensity = clamp((theta - uniform_cutoff.x) / epsilon, 0.0, 1.0);
    shadow *= intensity;

    return (shadow);
}

void main()
{
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    FragColor = vec4(vec3(shadow), 1.0);
}
