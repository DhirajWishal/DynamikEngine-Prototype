#version 450 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} Ubo;

layout (location = 0) out vec3 outColor;

void main()
{
	gl_Position = Ubo.proj * Ubo.view * Ubo.model * vec4(inPosition, 1.0f);
    outColor = inColor;
}