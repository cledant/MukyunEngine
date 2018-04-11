#version 410 core

#define SHADOW_ACCENTUATION 0.2
#define NB_MAX_DIR_LIGHT 10
#define NB_MAX_POINT_LIGHT 10
#define NB_MAX_SPOT_LIGHT 10

//In Out
out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 Tex_coord;
    vec4 FragPosLightSpace;
} fs_in;

//Structures
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
    vec4 pos;
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

//Uniforms
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

//Shadow
uniform sampler2D depth2D;
uniform samplerCube depthCube;
uniform float uniform_farPlane;
uniform int	light_type;
uniform int	pass_type;
uniform int index;

//Functions Protoypes

//Ambient
vec3 AmbientPass(void);
vec3 CalcDirLightAmbient(DirLightDataGL light);
vec3 CalcPointLightAmbient(PointLightDataGL light);
vec3 CalcSpotLightAmbient(SpotLightDataGL light);

//Light
vec3 LightPass(void);
vec3 CalcDirLight(DirLightDataGL light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLightDataGL light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLightDataGL light, vec3 normal, vec3 fragPos, vec3 viewDir);
float DirectionalShadowValue(vec4 fragPosLightSpace, vec3 lightPos);
float OmniDirectionalShadowValue(vec4 fragPosLightSpace, vec3 lightPos);

//Program
void main()
{
	if (pass_type == 0)
		FragColor = vec4(AmbientPass(), 1.0);
	else
		FragColor = vec4(LightPass(), 1.0);
}

//Ambiant
vec3 AmbientPass(void)
{
    vec3 result = vec3(0.0f);

    for(int i = 0; i < nb_dir_light; i++)
        result += CalcDirLightAmbient(dir[i]);
    for(int i = 0; i < nb_point_light; i++)
        result += CalcPointLightAmbient(point[i]);
    for(int i = 0; i < nb_spot_light; i++)
        result += CalcSpotLightAmbient(spot[i]);
	return (result);
}

vec3 CalcDirLightAmbient(DirLightDataGL light)
{
    vec3 ambient = vec3(light.ambient_color) * vec3(texture(uniform_material.tex_diffuse, fs_in.Tex_coord)) *
    				uniform_material.mat_ambient;
    return (ambient);
}

vec3 CalcPointLightAmbient(PointLightDataGL light)
{
    //Attenuation
    float distance = length(vec3(light.pos) - fs_in.FragPos);
    float attenuation = 1.0 / (light.attenuation_coeff.x + light.attenuation_coeff.y * distance +
    					light.attenuation_coeff.z * pow(distance, 2));

    //All
    vec3 ambient = vec3(light.ambient_color) * vec3(texture(uniform_material.tex_diffuse, fs_in.Tex_coord)) *
    				uniform_material.mat_ambient;
    ambient *= attenuation;
    return (ambient);
}

vec3 CalcSpotLightAmbient(SpotLightDataGL light)
{
    vec3 lightDir = normalize(vec3(light.pos) - fs_in.FragPos);

    //Attenuation
    float distance = length(vec3(light.pos) - fs_in.FragPos);
    float attenuation = 1.0 / (light.attenuation_coeff.x + light.attenuation_coeff.y * distance +
    					light.attenuation_coeff.z * pow(distance, 2));

    //SpotLight
    float theta = dot(lightDir, normalize(-vec3(light.dir)));
    float epsilon = light.cutoff.y - light.cutoff.x;
    float intensity = clamp((theta - light.cutoff.x) / epsilon, 0.0, 1.0);

    //All
    vec3 ambient = vec3(light.ambient_color) * vec3(texture(uniform_material.tex_diffuse, fs_in.Tex_coord)) *
    				uniform_material.mat_ambient;
    ambient *= attenuation * intensity;
    return (ambient);
}

//Light

vec3 LightPass(void)
{
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 result = vec3(0.0f);

	if (light_type == 0)
		result = CalcPointLight(point[index], norm, fs_in.FragPos, viewDir);
	else if (light_type == 1)
        result = CalcDirLight(dir[index], norm, viewDir);
	else if (light_type == 2)
        result = CalcSpotLight(spot[index], norm, fs_in.FragPos, viewDir);
    return (result);
}

vec3 CalcDirLight(DirLightDataGL light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-vec3(light.dir));

    //Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    //Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uniform_material.shininess);

	//Shadow value
	float shadowVal = DirectionalShadowValue(fs_in.FragPosLightSpace, vec3(light.pos));

    //All
    vec3 diffuse = vec3(light.diffuse_color) * diff * vec3(texture(uniform_material.tex_diffuse, fs_in.Tex_coord)) *
                    uniform_material.mat_diffuse * shadowVal;
    vec3 specular = vec3(light.specular_color) * spec * vec3(texture(uniform_material.tex_specular, fs_in.Tex_coord)) *
                    uniform_material.mat_specular * shadowVal;
    return (diffuse + specular);
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

	//Shadow Value
	float shadowVal = OmniDirectionalShadowValue(fs_in.FragPosLightSpace, vec3(light.pos));

    //All
    vec3 diffuse = vec3(light.diffuse_color) * diff * vec3(texture(uniform_material.tex_diffuse, fs_in.Tex_coord)) *
    				uniform_material.mat_diffuse;
    vec3 specular = vec3(light.specular_color) * spec * vec3(texture(uniform_material.tex_specular, fs_in.Tex_coord)) *
    				uniform_material.mat_specular;
    diffuse *= attenuation * shadowVal;
    specular *= attenuation * shadowVal;
    return (diffuse + specular);
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
    float attenuation = 1.0 / (light.attenuation_coeff.x + light.attenuation_coeff.y * distance + light.attenuation_coeff.z *
    					pow(distance, 2));

    //SpotLight
    float theta = dot(lightDir, normalize(-vec3(light.dir)));
    float epsilon = light.cutoff.y - light.cutoff.x;
    float intensity = clamp((theta - light.cutoff.x) / epsilon, 0.0, 1.0);

	//Shadow Value
	float shadowVal = DirectionalShadowValue(fs_in.FragPosLightSpace, vec3(light.pos));

    //All
    vec3 diffuse = vec3(light.diffuse_color) * diff * vec3(texture(uniform_material.tex_diffuse, fs_in.Tex_coord)) *
    				uniform_material.mat_diffuse;
    vec3 specular = vec3(light.specular_color) * spec * vec3(texture(uniform_material.tex_specular, fs_in.Tex_coord)) *
    				uniform_material.mat_specular;
    diffuse *= attenuation * intensity * shadowVal;
    specular *= attenuation * intensity * shadowVal;
    return (diffuse + specular);
}

float DirectionalShadowValue(vec4 fragPosLightSpace, vec3 lightPos)
{
    // perform perspective divide
    vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depth2D, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

	//Basic shadow
//	float shadow = (currentDepth - bias > closestDepth) ? closestDepth - SHADOW_ACCENTUATION : currentDepth;

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depth2D, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depth2D, projCoords.xy + vec2(x, y) * texelSize).r;
            // check whether current frag pos is in shadow
            shadow += currentDepth - bias > pcfDepth  ? 1.0 - (closestDepth - SHADOW_ACCENTUATION) : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return (shadow);
}

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float OmniDirectionalShadowValue(vec4 fragPosLightSpace, vec3 lightPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fs_in.FragPos - lightPos;

    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = tan(acos(dot(normal, lightDir))) * 0.005;
    bias *= uniform_farPlane;

    float viewDistance = length(viewPos - fs_in.FragPos);
    float diskRadius = (1.0 + (viewDistance / uniform_farPlane)) / 25.0;

    float shadow = 0.0;
    int samples = 20;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthCube, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        float unmapped_closestDepth = closestDepth * uniform_farPlane;   // undo mapping [0;1]
        if (currentDepth - bias > unmapped_closestDepth)
            shadow += currentDepth / uniform_farPlane + SHADOW_ACCENTUATION;
    }
    shadow /= float(samples);
    return (shadow);
}