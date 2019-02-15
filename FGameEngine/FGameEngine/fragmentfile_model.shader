#version 330 core
out vec4 color;
in vec2 TexCoord;
in vec3 TexNormal;
in vec3 FragPos;

struct Material{
	vec3 ambient;
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
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

vec3 CalcLight(vec3 lightDir,vec3 lightColor,vec3 norm,vec3 viewVec,vec3 vecTexDiffuse,vec3 vecTexSpecular,float attenuation);
vec3 CalcDirectionPoint(LightDirection light,vec3 norm,vec3 viewVec,vec3 vecTexDiffuse,vec3 vecTexSpecular);
vec3 CalcPointLight(LightPoint light,vec3 norm,vec3 viewVec,vec3 fragPos,vec3 vecTexDiffuse,vec3 vecTexSpecular);
vec3 CalcSpotLight(LightSpot light,vec3 norm,vec3 viewVec,vec3 fragPos,vec3 vecTexDiffuse,vec3 vecTexSpecular);

void main()
{
	vec3 norm = normalize(TexNormal);
	vec3 viewVec = normalize(viewPos - FragPos);
	vec3 vecTexDiffuse = texture(material.texture_diffuse1,TexCoord).xyz;
	vec3 vecTexSpecular = texture(material.texture_specular1,TexCoord).xyz;

	vec3 resultColor = vec3(0,0,0);
	resultColor += CalcDirectionPoint(lightDirection,norm,viewVec,vecTexDiffuse,vecTexSpecular);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
        resultColor += CalcPointLight(pointLights[i], norm, viewVec,FragPos,vecTexDiffuse,vecTexSpecular);   
	resultColor += CalcSpotLight(lightSpot,norm,viewVec,FragPos,vecTexDiffuse,vecTexSpecular);
	color = vec4(resultColor,1.0f);
}

//���ռ���
vec3 CalcLight(vec3 lightDir,vec3 lightColor,vec3 norm,vec3 viewVec,
			vec3 vecTexDiffuse,vec3 vecTexSpecular,float attenuation,float spotRatio){
	//Diffuse�������䣩
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = vecTexDiffuse * diff * lightColor;

	//Ambient��������
    vec3 ambient = vecTexDiffuse * material.ambient * ambientColor;

	//specular�����淴�䣩
	vec3 reflectVec = reflect(-lightDir,norm);
	float specularAmount = pow(max(dot(reflectVec,viewVec),0),material.shininess);
	vec3 specular = vecTexSpecular * specularAmount * lightColor;

	return (ambient + (diffuse +  specular)*attenuation*spotRatio)*objectColor;
}

//ƽ�й�
vec3 CalcDirectionPoint(LightDirection light,vec3 norm,vec3 viewVec,vec3 vecTexDiffuse,vec3 vecTexSpecular){
	return CalcLight(normalize(light.direction),light.color,norm,viewVec,vecTexDiffuse,vecTexSpecular,1.0,1.0);
}

//���Դ
vec3 CalcPointLight(LightPoint light,vec3 norm,vec3 viewVec,vec3 fragPos,vec3 vecTexDiffuse,vec3 vecTexSpecular){
	//������Դ˥��
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear*dist + light.quadratic * dist * dist);
	
	return CalcLight(normalize(light.position - fragPos),light.color,norm,viewVec,vecTexDiffuse,vecTexSpecular,attenuation,1.0);
}

//�۹��
vec3 CalcSpotLight(LightSpot light,vec3 norm,vec3 viewVec,vec3 fragPos,vec3 vecTexDiffuse,vec3 vecTexSpecular){
	//������Դ˥��
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear*dist + light.quadratic * dist * dist);

	vec3 lightDir = normalize(light.position - fragPos);
	//�۹��
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

	return CalcLight(lightDir,light.color,norm,viewVec,vecTexDiffuse,vecTexSpecular,attenuation,spotRatio);
}