#version 460 core

uniform vec4 uni_color;
uniform ivec4 uni_border_size;
uniform vec4 uni_border_color;

in vec2 uv;
flat in ivec2 size;

out vec4 out_color;

void main()
{
	vec4 border_uv = vec4(uni_border_size) / vec4(size.xx, size.yy);
	if (uv.x < border_uv.x || uv.x > 1.0 - border_uv.y || uv.y < border_uv.z || uv.y > 1.0 - border_uv.w) {
		out_color = uni_border_color;
	}
	else {
		out_color = uni_color;
	}	
}