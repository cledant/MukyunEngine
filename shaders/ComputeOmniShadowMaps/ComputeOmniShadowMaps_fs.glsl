#version 410 core

out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 Tex_coord;
} fs_in;

uniform samplerCube uniform_depthMap;
uniform vec3 uniform_lightPos;
uniform vec3 uniform_viewPos;
uniform float uniform_farPlane;

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
    vec3 fragToLight = fragPos - uniform_lightPos;

    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;

    float viewDistance = length(uniform_viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / uniform_farPlane)) / 25.0;

    int samples = 20;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(uniform_depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= uniform_farPlane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return (shadow);
}

void main()
{
    float shadow = ShadowCalculation(fs_in.FragPos);
    FragColor = vec4(vec3(shadow), 1.0);
}
