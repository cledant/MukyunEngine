#version 410 core

#define NB_MAX_DIR_LIGHT 10

out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec4 FragPosLightSpace[NB_MAX_DIR_LIGHT];
    vec3 Normal;
    vec2 Tex_coord;
} fs_in;

struct material
{
    sampler2D tex_diffuse;
    sampler2D tex_specular;
    vec3 mat_ambient;
    vec3 mat_diffuse;
    vec3 mat_specular;
    float shininess;
};

struct DirLightDataGL
{
	vec4 dir;
	vec4 pos;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

layout (std140) uniform uniform_DirLight
{
    DirLightDataGL dir_light[NB_MAX_DIR_LIGHT];
};

//Camera Pos
uniform vec3 viewPos;

//Actual Light number
uniform int nb_dir_light;

//material
uniform material uniform_material;

uniform sampler2DArray shadowMapArray;

float ShadowCalculation();
vec3 CalcDirLight(vec3 normal, vec3 viewDir);

void main()
{
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 result = vec3(0.0f);

    //1) DirLight
    result = CalcDirLight(norm, viewDir);

    //Final Color
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(vec3 normal, vec3 viewDir)
{
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    float shadow = 0.0;

    //Ambient, Diffuse and Specular for every Light
    for (int i = 0; i < nb_dir_light; i++)
    {
        vec3 lightDir = normalize(-vec3(dir_light[i].dir));

        //Diffuse
        float diff = max(dot(normal, lightDir), 0.0);

        //Specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), uniform_material.shininess);

        //All
        ambient += vec3(dir_light[i].ambient_color) * vec3(texture(uniform_material.tex_diffuse, fs_in.Tex_coord)) * uniform_material.mat_ambient;
        diffuse += vec3(dir_light[i].diffuse_color) * diff * vec3(texture(uniform_material.tex_diffuse, fs_in.Tex_coord)) * uniform_material.mat_diffuse;
        specular += vec3(dir_light[i].specular_color) * spec * vec3(texture(uniform_material.tex_specular, fs_in.Tex_coord)) * uniform_material.mat_specular;
    }

    //Compute shadow coeff
    shadow = ShadowCalculation();

    //Total color
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
    return (lighting);
}

float ShadowCalculation()
{
    //Constants
    float total_shadow = 0.0;
    vec3 normal = normalize(fs_in.Normal);

    for (int i = 0; i < nb_dir_light; ++i)
    {
        // perform perspective divide
        vec3 projCoords = fs_in.FragPosLightSpace[i].xyz / fs_in.FragPosLightSpace[i].w;

        // transform to [0,1] range
        projCoords = projCoords * 0.5 + 0.5;

        // get depth of current fragment from light's perspective
        float currentDepth = projCoords.z;

        // calculate bias (based on depth map resolution and slope)
        vec3 lightDir = normalize(dir_light[i].pos.xyz - fs_in.FragPos);
        float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

        // check whether current frag pos is in shadow
        // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
        // PCF
        float shadow = 0.0;
        vec2 texelSize = 1.0 / vec2(textureSize(shadowMapArray, 0));
        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(shadowMapArray, vec3(vec2(projCoords.xy + vec2(x, y) * texelSize), i)).r;
                shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
            }
        }
        shadow /= 9.0;

       // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
        if(projCoords.z > 1.0)
            shadow = 0.0;
        total_shadow += shadow;
    }
    if (total_shadow > 1.0)
        total_shadow = 1.0;
    return (total_shadow);
}