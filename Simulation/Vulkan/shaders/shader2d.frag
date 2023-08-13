#version 450

layout(location = 0) in vec4 color;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 def;

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
	vec2 scale;
} push;

void main() {
	outColor = color;
	def = vec4(0,0,0,0);
}