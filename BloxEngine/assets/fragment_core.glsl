#version 330 core

struct Material {
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};


#define MAX_POINT_LIGHTS 20

struct PointLight {
	vec3 position;

	float k0;
	float k1;
	float k2;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform int noPointLights;

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;


#define MAX_SPOT_LIGHTS 20

struct SpotLight {
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	float k0;
	float k1;
	float k2;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];
uniform int noSpotLights;

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;

uniform Material material;

uniform vec3 viewPos;	

vec3 calcSpotLight(int id, vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap);
vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap);
vec3 calcPointLight(int id, vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap);

void main() 
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 diffMap = vec3(texture(material.diffuse, texCoord));
	vec3 specMap = vec3(texture(material.specular, texCoord));

	vec3 result;

	result = calcDirLight(norm, viewDir, diffMap, specMap); 

	for(int i = 0; i < noPointLights; i++)
	{
		result += calcPointLight(i, norm, viewDir, diffMap, specMap); 
	}

	for(int i = 0; i < noSpotLights; i++)
	{
		result += calcSpotLight(i, norm, viewDir, diffMap, specMap); 
	}

	FragColor = vec4(result, 1.0);
}

vec3 calcSpotLight(int id, vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap)
{
	vec3 ambient = spotLight[id].ambient * diffMap;
	vec3 lightDir = normalize(spotLight[id].position - FragPos);

	float theta = dot(lightDir, normalize(-spotLight[id].direction));
	if(theta > spotLight[id].outerCutOff)
	{
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = spotLight[id].diffuse * (diff * diffMap);

		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
		vec3 specular = spotLight[id].specular * (spec * specMap);

		float intensity = (theta - spotLight[id].outerCutOff) / (spotLight[id].cutOff - spotLight[id].outerCutOff);
		intensity = clamp(intensity, 0.0, 1.0);
		diffuse *= intensity;
		specular *= intensity;
		
		float dist = length(spotLight[id].position - FragPos);
		float attenuation = 1.0/(spotLight[id].k0 + spotLight[id].k1 * dist + spotLight[id].k2 * (dist * dist));

		return vec3(ambient + diffuse + specular) * attenuation;
	}
	else 
	{
		return ambient;
	}

}

vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap)
{
	vec3 ambient = dirLight.ambient * diffMap;
	
	vec3 lightDir = normalize(-dirLight.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = dirLight.diffuse * (diff * diffMap);

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
	vec3 specular = dirLight.specular * (spec * specMap);

	return vec3(ambient + diffuse + specular);
}

vec3 calcPointLight(int id, vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap)
{
	vec3 ambient = pointLight[id].ambient * diffMap;
	
	vec3 lightDir = normalize(pointLight[id].position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pointLight[id].diffuse * (diff * diffMap);

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
	vec3 specular = pointLight[id].specular * (spec * specMap);

	float dist = length(pointLight[id].position - FragPos);
	float attenuation = 1.0/(pointLight[id].k0 + pointLight[id].k1 * dist + pointLight[id].k2 * (dist * dist));

	return vec3(ambient + diffuse + specular) * attenuation;
}