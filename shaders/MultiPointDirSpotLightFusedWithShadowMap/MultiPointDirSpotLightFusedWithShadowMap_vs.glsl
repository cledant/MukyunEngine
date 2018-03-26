#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;
layout (location = 5) in mat4 instanceMatrix;
layout (location = 9) in mat4 instanceInverseMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4		uniform_mat_perspec_mult_view;

out                 vec2 tex_coord;

void main()
{
    FragPos = vec3(instanceMatrix * vec4(pos, 1.0));
    Normal = mat3(instanceInverseMatrix) * norm;
    TexCoords = texCoord;

    gl_Position = uniform_mat_perspec_mult_view * vec4(FragPos, 1.0);
}