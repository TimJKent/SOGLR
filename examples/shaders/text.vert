#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;


uniform mat4 uOrthoProjection;
uniform mat4 uModel;

out vec2 TexCoords;


void main()
{
    gl_Position =  uOrthoProjection * uModel  * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}