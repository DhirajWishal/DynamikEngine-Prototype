#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in float fragIntegrityValue;

layout(location = 0) out vec4 outColor;

float _calculateAlphaWithIntegrity(float integrity)
{
    if(integrity > 1.0f)
        return 1.0f;
    if(integrity < 0.0f)
    {
    float _newValue = integrity * (-1.0f);
        if(_newValue > 1.0f)
            return 1.0f;

        return _newValue;
    }

    return integrity;
}

void main() {
    outColor = texture(texSampler, fragTexCoord);
}
