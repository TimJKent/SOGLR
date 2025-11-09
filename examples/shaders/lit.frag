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
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
   vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
   projCoords = projCoords * 0.5 + 0.5;
   float currentDepth = projCoords.z;
   float bias = max(0.05 * (1.0 - dot(Normal, DirectionalLightDirection)), 0.005);  

   float shadow = 0.0;
   vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
   for(int x = -1; x <= 1; ++x)
   {
      for(int y = -1; y <= 1; ++y)
      {
         float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
         shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
      }    
   }
   shadow /= 9.0;

   if(projCoords.z > 1.0)
      shadow = 0.0;
   return shadow;
}  

void main()
{
   float specularStrength = 1.0;
   float ambientStrength = 0.05;
   vec4 ambient = vec4(ambientStrength * DirectionalLightColor, 1.0);

   vec3 normal = texture(normalTexture, TexCoords).rgb;
   normal = normal * 2.0 - 1.0;   
   // Transform world-space vectors to tangent space
   vec3 lightDir = normalize(TBN * normalize(-DirectionalLightDirection));
   vec3 viewDir  = normalize(TBN * normalize(CameraPos - FragPos)); 
   vec3 halfwayDir = normalize(lightDir + viewDir);

   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(normal, halfwayDir), 0.0), 256.0);
   vec4 specular = vec4(specularStrength * spec * DirectionalLightColor, 1.0);  

   float diff = max(dot(normal, lightDir), 0.0);
   vec4 diffuse = vec4(diff * DirectionalLightColor, 1.0);
   
   vec4 texColor = texture(diffusedTexture, TexCoords);
   if(texColor.a < 0.1)
        discard;

   float shadow = ShadowCalculation(FragPosLightSpace);
   FragColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * texColor;
}  