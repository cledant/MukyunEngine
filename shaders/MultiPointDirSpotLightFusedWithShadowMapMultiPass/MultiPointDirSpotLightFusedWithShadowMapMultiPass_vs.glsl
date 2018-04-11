#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;
layout (location = 5) in mat4 instanceMatrix;
layout (location = 9) in mat4 instanceInverseMatrix;

//In Out
out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 Tex_coord;
    vec4 FragPosLightSpace;
} vs_out;

//Uniforms
uniform mat4	uniform_mat_perspec_mult_view;
uniform mat4	uniform_lightSpaceMatrix;
uniform int		light_type;
uniform int		pass_type;

/*
 * Type List :
 *
 * 		Omnidirectional = 0
 * 		Directional = 1
 *		Spot = 2
 */

/*
 * Pass List :
 *
 * 		Ambient = 0
 * 		Light = 1
 */

void main()
{
    vs_out.FragPos = vec3(instanceMatrix * vec4(pos, 1.0));
    vs_out.Tex_coord = texCoord;
	vs_out.Normal = vec3(0.0);
    vs_out.FragPosLightSpace = vec4(0.0);
    if (pass_type > 0)
        vs_out.Normal = mat3(instanceInverseMatrix) * norm;
	if (light_type > 0 && pass_type > 0)
	    vs_out.FragPosLightSpace = uniform_lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = uniform_mat_perspec_mult_view * vec4(vs_out.FragPos, 1.0);
}