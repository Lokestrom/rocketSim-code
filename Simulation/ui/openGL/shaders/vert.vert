#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

layout (location = 2) uniform mat4 modelMatrix;

layout (std140, binding = 0) uniform Matrices
{
	mat4 view;
	mat4 projection;
} matrices;

layout(location = 0) out vec3 fragColor;

void main()
{
  	vec4 positionWorld = modelMatrix * vec4(position, 1.0);
  	gl_Position = matrices.projection * matrices.view * positionWorld;
  	//fragNormalWorld = normalize(mat3(push.normalMatrix) * normal);
  	//fragPosWorld = positionWorld.xyz;
  	fragColor = color;
}