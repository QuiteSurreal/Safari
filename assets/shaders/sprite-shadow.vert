#version 460 core

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 offset;

uniform float shadowOffset;

uniform vec4 light;

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;

out vec2 uv;
out float lightStrength;

void main()
{
	uv = in_uv;
	lightStrength = light.w;
	
	vec4 pos = model * vec4(in_pos + offset, 1.0);
	float t = pos.y / light.y;
	pos -= t * vec4(light.xyz, 0);
	pos.y = shadowOffset;
	
	gl_Position = proj * view * pos;
}