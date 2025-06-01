#version 460 core

uniform mat4 proj;
uniform mat4 view;

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;

out vec2 uv;
out float height;

void main()
{
	uv = in_uv;
	height = in_pos.y;
	gl_Position = proj * view * vec4(in_pos, 1.0);
}