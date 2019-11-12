#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform terrainUniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} terrainUbo;

layout(set = 1, binding = 0) uniform UniformBufferObjectA {
    mat4 model;
    mat4 view;
    mat4 proj;
} uboA;

layout(location = 0) in vec3 inPositionTerrain;
layout(location = 1) in vec3 inColorTerrain;
layout(location = 2) in vec2 inTexCoordTerrain;

layout(location = 3) in vec3 inPositionA;
layout(location = 4) in vec3 inColorA;
layout(location = 5) in vec2 inTexCoordA;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

const float mapSize = 10.0f;

void main() {
//    gl_Position = ((terrainUbo.proj) 
//				* (terrainUbo.view) 
//				* (terrainUbo.model) 
//				* vec4(inPosition, mapSize))
//;
//				+ ((uboA.proj) 
//				* (uboA.view) 
//				* (uboA.model) 
//				* vec4(inPositionA, mapSize));

    //gl_Position = terrainUbo.proj * terrainUbo.view * terrainUbo.model * vec4(inPositionTerrain, mapSize);

    gl_Position = uboA.proj * uboA.view * uboA.model * vec4(inPositionA, mapSize);

    fragColor = inColorTerrain;
    //fragTexCoord = inTexCoordTerrain;
}


