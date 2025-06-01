#version 460 core

uniform sampler2D tex;

uniform float lightStrength;
uniform vec4 pointLights[512];
uniform int pointLightsCount;

in vec2 uv;
in vec2 uvGlobal;

out vec4 out_color;

void main()
{
	vec4 color = texture(tex, uv);

	float lightValue = 0.0;
	for (int i = 0; i < pointLightsCount; ++i) {
		const vec3 lightPosition = vec3(pointLights[i]);
		const float lightRadius = pointLights[i].w;
		const float dist = length(uvGlobal - lightPosition.xz);
		if (dist <= lightRadius) {
			const float value = 1.0 - smoothstep(0 , lightRadius, dist);
			lightValue += value;
		}
	}
	lightValue = min(lightValue, 1.0);

	float light = mix(lightValue, 1.0, lightStrength);
	out_color = vec4(color.rgb * light, color.a);
}