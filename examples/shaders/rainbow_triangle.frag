#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
  
void main()
{
    vec3 shiftedOurColor = ourColor;
    shiftedOurColor += 0.33;
    FragColor = vec4(shiftedOurColor, 1.0);
}