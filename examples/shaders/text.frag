#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffusedTexture;


void main()
{
    vec2 TexCoordsFlipped = TexCoords;
    TexCoordsFlipped.y = 1.0 - TexCoords.y;

    vec3 textColor = vec3(0.0,0.0,0.0);
    float textAlpha = texture(diffusedTexture, TexCoordsFlipped).r;
    FragColor =  vec4(textColor.rgb, textAlpha);
}  