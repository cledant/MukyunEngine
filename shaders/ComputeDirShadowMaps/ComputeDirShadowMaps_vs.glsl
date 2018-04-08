#version 410 core

#define NB_MAX_DIR_LIGHT 10

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;
layout (location = 5) in mat4 instanceMatrix;
layout (location = 9) in mat4 instanceInverseMatrix;

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 Tex_coord;
    vec4 FragPosLightSpace;
} vs_out;

layout (std140) uniform uniform_mat_perspec_mult_view
{
	mat4 mat_perspec_mult_view;
};

layout (std140) uniform uniform_lightSpaceMatrix
{
	mat4 lightSpaceMatrix[NB_MAX_DIR_LIGHT];
};

uniform int i;

void main()
{
    vs_out.FragPos = vec3(instanceMatrix * vec4(pos, 1.0));
    vs_out.Normal = mat3(instanceInverseMatrix) * norm;
    vs_out.Tex_coord = texCoord;
    vs_out.FragPosLightSpace = lightSpaceMatrix[i] * vec4(vs_out.FragPos, 1.0);
    gl_Position = mat_perspec_mult_view * vec4(vs_out.FragPos, 1.0);
}
