#version 330 core
out vec4 color;
in vec2 TexCoord;
in vec3 TexNormal;
in vec3 FragPos;

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

uniform sampler2D ourTexture;
uniform sampler2D ourFace;

uniform vec3 objectColor;
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	//Diffuse
	vec3 norm = normalize(TexNormal);
	vec3 lightDir = normalize(lightPos-FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = texture(material.diffuse,TexCoord).xyz * diff * lightColor;

	//Ambient
    vec3 ambient = material.ambient * ambientColor;

	//specular
	vec3 reflectVec = reflect(-lightDir,norm);
	vec3 viewVec = normalize(viewPos - FragPos);
	float specularAmount = pow(max(dot(reflectVec,viewVec),0),material.shininess);
	vec3 specular = material.specular * specularAmount * lightColor;

	//color = vec4(diffuse + ambient + specular,1.0f) * texture(ourTexture,TexCoord)*texture(ourFace,TexCoord);
	color = vec4((diffuse + ambient + specular)*objectColor,1.0f);
};