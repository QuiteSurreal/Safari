#version 460 core

uniform mat4 uni_proj;
uniform ivec3 uni_pos;
uniform ivec2 uni_size;

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;

out vec2 uv;

void main()
{
	uv = in_uv;
	gl_Position = uni_proj * vec4(in_pos * ivec3(uni_size, 1) + uni_pos, 1.0);
}