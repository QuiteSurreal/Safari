#version 460 core

uniform sampler2D tex;

uniform float lightStrength;
uniform float pointLightSum;

uniform float opacity;

in vec2 uv;

out vec4 out_color;

void main()
{
	if (opacity == 1.0) {
		discard;
		return;
	}
	vec4 color = texture(tex, vec2(uv.x, 1.0 - uv.y));
	if (color.a != 1.0) {
		discard;
	}
	const float light = min(lightStrength + min(pointLightSum, 0.9 + 0.1 * lightStrength), 1.0);
	out_color = vec4(color.rgb * light, 1.0 - opacity);
}