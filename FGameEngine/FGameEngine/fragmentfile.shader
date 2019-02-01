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

uniform vec3 objectColor;
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 lightdirection;
uniform vec3 viewPos;

void main()
{
	//Diffuse（漫反射）
	vec3 norm = normalize(TexNormal);
	vec3 lightDir = normalize(lightdirection);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = texture(material.diffuse,TexCoord).xyz * diff * lightColor;

	//Ambient（环境）
    vec3 ambient = texture(material.diffuse,TexCoord).xyz * material.ambient * ambientColor;

	//specular（镜面反射）
	vec3 reflectVec = reflect(-lightDir,norm);
	vec3 viewVec = normalize(viewPos - FragPos);
	float specularAmount = pow(max(dot(reflectVec,viewVec),0),material.shininess);
	vec3 specular = texture(material.specular,TexCoord).xyz * specularAmount * lightColor;

	color = vec4((diffuse + ambient + specular)*objectColor,1.0f);
};