#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 inUVW;

layout(location = 0) out vec4 outColor;

vec3 Uncharted2Tonemap(vec3 color) {
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	return ((color*(A*color+C*B)+D*E)/(color*(A*color+B)+D*F))-E/F;
}

void main() {
    outColor = texture(texSampler, fragTexCoord);

	//vec3 color = texture(texSampler, inUVW).rgb;

	// Tone mapping
	//color = Uncharted2Tonemap(color * 0.05f);
	//color = color * (1.0f / Uncharted2Tonemap(vec3(11.2f)));	
	// Gamma correction
	//color = pow(color, vec3(1.0f / 0.05f));
	
	//outColor = vec4(color, 1.0);
}
