#version 410 core

in vec4 FragPos;

uniform vec3 uniform_lightPos;
uniform float uniform_farPlane;

void main()
{
    float lightDistance = length(FragPos.xyz - uniform_lightPos);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / uniform_farPlane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}
