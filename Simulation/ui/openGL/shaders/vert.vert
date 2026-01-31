#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

layout (location = 2) uniform mat4 modelMatrix;
layout (location = 3) uniform vec4 colorOverride;

layout (std140, binding = 0) uniform Matrices
{
	mat4 view;
	mat4 projection;
} matrices;

layout(location = 0) out vec4 fragColor;

void main()
{
  	vec4 positionWorld = modelMatrix * vec4(position, 1.0);
  	gl_Position = matrices.projection * matrices.view * positionWorld;
  	//fragNormalWorld = normalize(mat3(push.normalMatrix) * normal);
  	//fragPosWorld = positionWorld.xyz;
    if (colorOverride.a > 0.0)
        fragColor = colorOverride;
    else
  	    fragColor = vec4(color, 1.0);
}