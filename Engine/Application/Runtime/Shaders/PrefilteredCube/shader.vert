#version 450

layout (location = 0) in vec3 inPos;

layout(push_constant) uniform PushConsts {
	layout (offset = 0) mat4 mvp;
} pushConsts;

layout (binding = 0) uniform UBO 
{
	mat4 model;
    mat4 view;
	mat4 projection;
} Camera;

layout (location = 0) out vec3 outUVW;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() 
{
	outUVW = inPos;
	gl_Position = pushConsts.mvp * vec4(inPos.xyz, 1.0);
}