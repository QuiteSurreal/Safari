#version 460 core

uniform mat4 proj;
uniform mat4 view;

uniform float scale;
uniform vec3 offset;

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;

out vec2 uv;
out vec2 uvGlobal;

void main()
{
	uv = in_uv;
	uvGlobal = offset.xz + vec2(in_uv.x, in_uv.y) * scale;
	gl_Position = proj * view * vec4(offset + in_pos * scale, 1.0);
}