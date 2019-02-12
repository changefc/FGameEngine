#version 330 core
out vec4 color;
in vec2 TexCoord;
in vec3 TexNormal;
in vec3 FragPos;

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct LightDirection{
	vec3 direction;
	vec3 color;
};
uniform LightDirection lightDirection;

struct LightPoint{
	vec3 position;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};
#define NR_POINT_LIGHTS 2
uniform LightPoint pointLights[NR_POINT_LIGHTS];

struct LightSpot{
	vec3 direction;
	vec3 color;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	float cosInnerPhy;
	float cosOutterPhy;
};
uniform LightSpot lightSpot;

uniform vec3 objectColor;
uniform vec3 ambientColor;
uniform vec3 viewPos;

vec3 CalcDirectionPoint(LightDirection light,vec3 norm,vec3 viewVec);
vec3 CalcPointLight(LightPoint light,vec3 norm,vec3 viewVec,vec3 fragPos);
vec3 CalcSpotLight(LightSpot light,vec3 norm,vec3 viewVec,vec3 fragPos);

void main()
{
	vec3 norm = normalize(TexNormal);
	vec3 viewVec = normalize(viewPos - FragPos);

	vec3 resultColor = vec3(0,0,0);
	resultColor += CalcDirectionPoint(lightDirection,norm,viewVec);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
        resultColor += CalcPointLight(pointLights[i], norm, viewVec,FragPos);   
	resultColor += CalcSpotLight(lightSpot,norm,viewVec,FragPos);
	color = vec4(resultColor,1.0f);
}

//平行光
vec3 CalcDirectionPoint(LightDirection light,vec3 norm,vec3 viewVec){
	vec3 resultColor;
	//Diffuse（漫反射）
	vec3 lightDir = normalize(light.direction);//平行光
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = texture(material.diffuse,TexCoord).xyz * diff * light.color;

	//Ambient（环境）
    vec3 ambient = texture(material.diffuse,TexCoord).xyz * material.ambient * ambientColor;

	//specular（镜面反射）
	vec3 reflectVec = reflect(-lightDir,norm);
	float specularAmount = pow(max(dot(reflectVec,viewVec),0),material.shininess);
	vec3 specular = texture(material.specular,TexCoord).xyz * specularAmount * light.color;

	return (ambient + diffuse +  specular)*objectColor;
}

//点光源
vec3 CalcPointLight(LightPoint light,vec3 norm,vec3 viewVec,vec3 fragPos){
	//计算点光源衰减
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear*dist + light.quadratic * dist * dist);

	//Diffuse（漫反射）
	vec3 lightDir = viewVec;//点光源
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = texture(material.diffuse,TexCoord).xyz * diff * light.color;

	//Ambient（环境）
    vec3 ambient = texture(material.diffuse,TexCoord).xyz * material.ambient * ambientColor;

	//specular（镜面反射）
	vec3 reflectVec = reflect(-lightDir,norm);
	float specularAmount = pow(max(dot(reflectVec,viewVec),0),material.shininess);
	vec3 specular = texture(material.specular,TexCoord).xyz * specularAmount * light.color;

	return (ambient + (diffuse +  specular)*attenuation)*objectColor;
}

//聚光灯
vec3 CalcSpotLight(LightSpot light,vec3 norm,vec3 viewVec,vec3 fragPos){
		//计算点光源衰减
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear*dist + light.quadratic * dist * dist);

	//Diffuse（漫反射）
	vec3 lightDir = viewVec;//点光源
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = texture(material.diffuse,TexCoord).xyz * diff * light.color;

	//Ambient（环境）
    vec3 ambient = texture(material.diffuse,TexCoord).xyz * material.ambient * ambientColor;

	//specular（镜面反射）
	vec3 reflectVec = reflect(-lightDir,norm);
	float specularAmount = pow(max(dot(reflectVec,viewVec),0),material.shininess);
	vec3 specular = texture(material.specular,TexCoord).xyz * specularAmount * light.color;
	
	//聚光灯
	float theta = dot(-lightDir,normalize(-light.direction));
	float spotRatio = 1.0f;
	if(theta > light.cosInnerPhy){
		spotRatio = 1.0f;
	}
	else if(theta > light.cosOutterPhy){
		spotRatio = (theta - light.cosOutterPhy)/(light.cosInnerPhy - light.cosOutterPhy);
	}
	else{
		spotRatio = 0.0f;
	}

	return (ambient + (diffuse +  specular)*attenuation*spotRatio)*objectColor;
}