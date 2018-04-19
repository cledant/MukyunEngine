#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices= 3) out;

uniform vec3 uniform_viewPos;

in VS_OUT
{
    vec2 tex_coords;
} gs_in[];

out GS_OUT
{
	vec2 TexCoords;
} gs_out;

vec3 GetNormal(void)
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main()
{
	vec3 norm = GetNormal();
	vec3 dir = normalize(-uniform_viewPos);

	if (dot(norm, dir) > 0.0)
	{
		gl_Position = gl_in[0].gl_Position;
		gs_out.TexCoords = gs_in[0].tex_coords;
		EmitVertex();
		gl_Position = gl_in[1].gl_Position;
		gs_out.TexCoords = gs_in[1].tex_coords;
		EmitVertex();
		gl_Position = gl_in[2].gl_Position;
		gs_out.TexCoords = gs_in[2].tex_coords;
		EmitVertex();
		EndPrimitive();
	}
}