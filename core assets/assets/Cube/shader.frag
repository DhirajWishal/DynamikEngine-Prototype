#version 450
#extension GL_ARB_separate_shader_objects : enable

#define lightCount 6

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 inColor;	    // this
layout(location = 1) in vec3 lightColor;    // this
layout(location = 2) in vec3 inNormal;	    // this
layout(location = 3) in vec3 LightPos;	    // this
layout(location = 4) in vec4 inLightVec[lightCount];	// this

layout(location = 0) out vec4 outColor;

#define MAX_LIGHT_DIST 9.0 * 9.0

void main() {
	vec3 lightColor[lightCount];
	lightColor[0] = vec3(1.0, 0.0, 0.0);
	lightColor[1] = vec3(0.0, 1.0, 0.0);
	lightColor[2] = vec3(0.0, 0.0, 1.0);
	lightColor[3] = vec3(1.0, 0.0, 1.0);
	lightColor[4] = vec3(0.0, 1.0, 1.0);
	lightColor[5] = vec3(1.0, 1.0, 0.0);
	
	vec3 diffuse = vec3(0.0);
	// Just some very basic attenuation
	for (int i = 0; i < lightCount; ++i) {				
		float lRadius =  MAX_LIGHT_DIST * inLightVec[i].w;
	
		float dist = min(dot(inLightVec[i], inLightVec[i]), lRadius) / lRadius;
		float distFactor = 1.0 - dist;		
	
		diffuse += inNormal * distFactor;		
	}
			
	outColor.rgb = diffuse;
}