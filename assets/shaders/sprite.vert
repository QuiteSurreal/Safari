#version 460 core

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 offset;

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;

out vec2 uv;

void main()
{
	uv = in_uv;
	gl_Position = proj * view * model * vec4(in_pos + offset, 1.0);
}