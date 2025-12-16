#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;
in vec3 CameraPos;
in vec3 DirectionalLightDirection;
in vec3 DirectionalLightColor;
in float DirectionalLightIntensity;
in mat3 TBN;
in vec4 FragPosLightSpace;

uniform sampler2D diffusedTexture;
uniform sampler2D normalTexture;


void main()
{
    vec3 textColor = vec3(0.0,0.0,0.0);
    vec2 TexCoordsFlipped = TexCoords;
    TexCoordsFlipped.y = 1.0 - TexCoords.y;
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(diffusedTexture, TexCoordsFlipped).r);
    FragColor = vec4(textColor, 1.0) * sampled;
}  