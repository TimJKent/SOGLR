#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
    vec3 directionalLightDirection;
    vec3 directionalLightColor;
    float directionalLightIntensity;
};

out vec2 TexCoords;

uniform mat4 uModel;

void main()
{
    gl_Position = projection * view * uModel * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}