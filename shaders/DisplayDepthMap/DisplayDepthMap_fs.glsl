#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D uniform_tex;

void main()
{
    float depthValue = texture(uniform_tex, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}
