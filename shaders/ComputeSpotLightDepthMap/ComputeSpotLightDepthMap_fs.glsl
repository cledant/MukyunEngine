#version 410 core

in vec3 FragPos;

uniform vec4 uniform_lightDir;
uniform vec4 uniform_lightPos;
uniform vec4 uniform_cutoff;

void main()
{
    vec3 lightDir = normalize(vec3(uniform_lightPos) - FragPos);
    float theta = dot(lightDir, normalize(-vec3(uniform_lightDir)));
    float epsilon = uniform_cutoff.y - uniform_cutoff.x;
    float intensity = clamp((theta - uniform_cutoff.x) / epsilon, 0.0, 1.0);

    gl_FragDepth = gl_FragCoord.z * intensity;
}
