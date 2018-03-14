#version 410 core

#define NB_MAX_DIR_LIGHT 10
#define NB_MAX_POINT_LIGHT 20
#define NB_MAX_SPOT_LIGHT 10

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct material
{
    sampler2D tex_diffuse;
    sampler2D tex_specular;
    vec3 mat_ambient;
    vec3 mat_diffuse;
    vec3 mat_specular;
    float shininess;
};

struct PointLightDataGL
{
	vec4 pos;
	vec4 attenuation_coeff;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

struct DirLightDataGL
{
	vec4 dir;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

struct SpotLightDataGL
{
	vec4 pos;
	vec4 dir;
	vec4 attenuation_coeff;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
	vec4 cutoff;
};

layout (std140) uniform uniform_DirLight
{
    DirLightDataGL dir[NB_MAX_DIR_LIGHT];
};

layout (std140) uniform uniform_PointLight
{
    PointLightDataGL point[NB_MAX_POINT_LIGHT];
};

layout (std140) uniform uniform_SpotLight
{
    SpotLightDataGL spot[NB_MAX_SPOT_LIGHT];
};

//Camera Pos
uniform vec3 viewPos;

//Actual Light number
uniform int nb_point_light;
uniform int nb_dir_light;
uniform int nb_spot_light;

//material
uniform material uniform_material;

vec3 CalcDirLight(DirLightDataGL light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLightDataGL light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLightDataGL light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0f);

    //1) DirLight
//    for(int i = 0; i < nb_dir_light; i++)
//        result = CalcDirLight(dir[i], norm, viewDir);

    //2) PointLight
    for(int i = 0; i < nb_point_light; i++)
        result += CalcPointLight(point[i], norm, FragPos, viewDir);

    //3) SpotLight
//    for(int i = 0; i < nb_spot_light; i++)
  //      result += CalcSpotLight(spot[i], norm, FragPos, viewDir);

    //Final Color
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLightDataGL light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-vec3(light.dir));

    //Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    //Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uniform_material.shininess);

    //All
    vec3 ambient = vec3(light.ambient_color) * vec3(texture(uniform_material.tex_diffuse, TexCoords)) * uniform_material.mat_ambient;
    vec3 diffuse = vec3(light.diffuse_color) * diff * vec3(texture(uniform_material.tex_diffuse, TexCoords)) * uniform_material.mat_diffuse;
    vec3 specular = vec3(light.specular_color) * spec * vec3(texture(uniform_material.tex_specular, TexCoords)) * uniform_material.mat_specular;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLightDataGL light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(vec3(light.pos) - fragPos);

    //Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    //Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uniform_material.shininess);

    //Attenuation
    float distance = length(vec3(light.pos) - fragPos);
    float attenuation = 1.0 / (light.attenuation_coeff.x + light.attenuation_coeff.y * distance + light.attenuation_coeff.z * pow(distance, 2));

    //All
    vec3 ambient = vec3(light.ambient_color) * vec3(texture(uniform_material.tex_diffuse, TexCoords)) * uniform_material.mat_ambient;
    vec3 diffuse = vec3(light.diffuse_color) * diff * vec3(texture(uniform_material.tex_diffuse, TexCoords)) * uniform_material.mat_diffuse;
    vec3 specular = vec3(light.specular_color) * spec * vec3(texture(uniform_material.tex_specular, TexCoords)) * uniform_material.mat_specular;
//    vec3 specular = vec3(spec);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
//    return (vec3(spec));
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLightDataGL light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(vec3(light.pos) - fragPos);

    //Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    //Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uniform_material.shininess);

    //Attenuation
    float distance = length(vec3(light.pos) - fragPos);
    float attenuation = 1.0 / (light.attenuation_coeff.x + light.attenuation_coeff.y * distance + light.attenuation_coeff.z * pow(distance, 2));

    //SpotLight
    float theta = dot(lightDir, normalize(-vec3(light.dir)));
    float epsilon = light.cutoff.y - light.cutoff.x;
    float intensity = clamp((theta - light.cutoff.x) / epsilon, 0.0, 1.0);

    //All
    vec3 ambient = vec3(light.ambient_color) * vec3(texture(uniform_material.tex_diffuse, TexCoords)) * uniform_material.mat_ambient;
    vec3 diffuse = vec3(light.diffuse_color) * diff * vec3(texture(uniform_material.tex_diffuse, TexCoords)) * uniform_material.mat_diffuse;
    vec3 specular = vec3(light.specular_color) * spec * vec3(texture(uniform_material.tex_specular, TexCoords)) * uniform_material.mat_specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}