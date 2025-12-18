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

uniform mat4 lightSpaceMatrix;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 CameraPos;
out vec3 DirectionalLightDirection;
out vec3 DirectionalLightColor;
out float DirectionalLightIntensity;
out vec4 FragPosLightSpace;
out mat3 TBN;

uniform mat4 uModel;

void main()
{
    gl_Position = uModel * vec4(aPos, 1.0);
    gl_Position.x -= 1.0;
    gl_Position.y -= 1.0;
    FragPos = gl_Position.xyz;
    FragPosLightSpace = lightSpaceMatrix * uModel * vec4(FragPos, 1.0);
    // Properly transform normals for non-uniform scaling
    mat3 normalMatrix = mat3(transpose(inverse(uModel)));
    Normal = normalize(normalMatrix * aNormal);
    TexCoords = aTexCoords;
    CameraPos = cameraPos;

    DirectionalLightDirection = directionalLightDirection;
    DirectionalLightColor = directionalLightColor;
    DirectionalLightIntensity = directionalLightIntensity;
    vec3 T = normalize(vec3(uModel * vec4(aTangent,   0.0)));
    vec3 N = normalize(vec3(uModel * vec4(aNormal,    0.0)));
    
    // Re-orthogonalize T with respect to N (Gram-Schmidt process)
    T = normalize(T - dot(T, N) * N);
    
    // Calculate B to ensure right-handed coordinate system
    vec3 B = cross(N, T);
    
    // Check handedness and flip if necessary
    if (dot(cross(N, T), aBitangent) < 0.0) {
        B = -B;
    }
    
    TBN = transpose(mat3(T, B, N));
}