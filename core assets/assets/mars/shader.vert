#version 450
#extension GL_ARB_separate_shader_objects : enable

#define lightCount 6

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 objectColor;
} Ubo;

layout(push_constant) uniform PushConsts {
	vec4 lightPos;
} pushConsts;

layout(location = 0) in vec3 aPos;  // this
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 aNormal;   // this

layout(location = 0) out vec3 fragPos;    // this
layout(location = 1) out vec3 fragTexCoord;
layout(location = 2) out vec3 fragNormal;   // this
layout(location = 3) out vec3 outLightPos;  // this
layout(location = 4) out vec3 outLightColor;

const float mapSize = 10.0f;

void main() {
    gl_Position = Ubo.proj * Ubo.view * Ubo.model * vec4(aPos, mapSize);

    fragPos = vec3(Ubo.view * Ubo.model * vec4(aPos, mapSize));

    fragTexCoord = vec3((239 / 256.0f), (142 / 256.0f), (56 / 256.0f));
    fragNormal = mat3(transpose(inverse(Ubo.view * Ubo.model))) * aNormal;
    outLightPos = vec3(Ubo.view * pushConsts.lightPos);
    outLightColor = Ubo.objectColor;
}

