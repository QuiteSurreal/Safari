#version 460 core

uniform sampler2D tex;

uniform float lightStrength;

in vec2 uv;
in float height;

out vec4 out_color;

void main()
{
	vec4 color = texture(tex, uv);
	out_color = vec4(color.rgb * lightStrength * (1.0 - height / 25.0), color.a);
}