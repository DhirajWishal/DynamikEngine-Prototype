#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} Ubo;

layout(location = 0) in vec3 inPositionTerrain;
layout(location = 1) in vec3 inColorTerrain;
layout(location = 2) in vec2 inTexCoordTerrain;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

const float mapSize = 10.0f;

void main() {

    gl_Position = Ubo.proj * Ubo.view * Ubo.model * vec4(inPosition, mapSize);

    fragColor = inColor;
    fragTexCoord = inTexCoord;
}


