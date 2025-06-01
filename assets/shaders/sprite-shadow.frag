#version 460 core

uniform sampler2D tex;

in vec2 uv;
in float lightStrength;

uniform float opacity;

out vec4 out_color;

void main()
{
	vec4 color = texture(tex, vec2(uv.x, 1.0 - uv.y));
	if (color.a == 0) {
		discard;
	}
	out_color = vec4(vec3(0.0), (1.0 - opacity) * lightStrength * 0.3);
}