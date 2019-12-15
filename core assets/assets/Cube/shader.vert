#version 450
#extension GL_ARB_separate_shader_objects : enable

#define lightCount 6

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 objectColor[lightCount];
} Ubo;

layout(push_constant) uniform PushConsts {
	vec4 lightPos[lightCount];
} pushConsts;

layout(location = 0) in vec3 inPos;  // this
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;   // this

layout(location = 0) out vec3 outColor;          // this
layout(location = 1) out vec3 fragTexCoord;     // this
layout(location = 2) out vec3 outNormal;       // this
layout(location = 3) out vec3 outLightPos;      // this
layout(location = 4) out vec4 outLightVec[lightCount];    // this

const float mapSize = 10.0f;

void main() {
	outNormal = inNormal;
	outColor = inColor;

    gl_Position = Ubo.proj * Ubo.view * Ubo.model * vec4(inPos, mapSize);

	for (int i = 0; i < lightCount; ++i) {	
		vec4 worldPos =  Ubo.model * vec4(inPos.xyz, 1.0);
		outLightVec[i].xyz = pushConsts.lightPos[i].xyz - inPos.xyz;		
		// Store light radius in w
		outLightVec[i].w = pushConsts.lightPos[i].w;
	}
}