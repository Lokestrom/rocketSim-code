#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

layout (location = 0) out vec4 outColor;

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
    vec4 position;
    float scale;
} push;

void main() {
	gl_Position = vec4(push.position.x + (position.x * push.scale * ubo.aspectRatio.y), push.position.y + (position.y * push.scale * ubo.aspectRatio.x), 0, 1);

    outColor = color;
}