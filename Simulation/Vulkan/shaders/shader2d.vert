#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 outColor;

struct PointLight {
  	vec4 position; // ignore w
  	vec4 color; // w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo {
  	mat4 projection;
  	mat4 view;
  	mat4 invView;
  	vec4 ambientLightColor; // w is intensity
  	PointLight pointLights[10];
    vec4 aspectRatio;
  	int numLights;
} ubo;

layout(push_constant) uniform Push {
    vec4 position; // w is rotation
	vec2 scale;
} push;

void main() {
	mat2 rotationMat = {{cos(push.position.w), -sin(push.position.w)}, {sin(push.position.w), cos(push.position.w)}};
	vec2 newPosition;
	newPosition.x = position.x * push.scale.x * ubo.aspectRatio.y;
	newPosition.y = position.y * push.scale.y * ubo.aspectRatio.x;
	newPosition = rotationMat * newPosition;

	gl_Position = vec4(push.position.x + newPosition.x, push.position.y + newPosition.y, push.position.z, 1);
	outColor = color;
}