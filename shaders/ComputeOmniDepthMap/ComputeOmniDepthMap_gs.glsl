#version 410 core

#define NB_MAX_POINT_LIGHT 20

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

struct OmniProjMatrices
{
	mat4 mat[6];
};

layout (std140) uniform uniform_shadowMatrices
{
	OmniProjMatrices proj[NB_MAX_POINT_LIGHT];
};

uniform int index;

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = proj[index].mat[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}
