#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 texNormal;
out vec2 TexCoord;
out vec3 TexNormal;
out vec3 FragPos;

uniform mat4 transform;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
	gl_Position = projectionMat*viewMat*modelMat*transform*vec4(position.x, position.y, position.z, 1.0);
	FragPos = vec3(modelMat * vec4(position,1.0));
	TexCoord = texCoord;
	TexNormal = mat3(transpose(inverse(modelMat)))*texNormal;
};