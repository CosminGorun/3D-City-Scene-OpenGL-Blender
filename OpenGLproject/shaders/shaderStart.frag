#version 410 core 
 
in vec3 color; 
in vec2 passTexture; 
in int lumina;
in vec4 fragPosLightSpace; 
in float fogDistance;
out vec4 fColor; 
 
uniform sampler2D diffuseTexture; 
uniform sampler2D depthMapTexture; 
uniform int lumina1;
uniform int lumina2;
uniform int ceata;
uniform float transp;

vec3 fogColor=vec3(0.5,0.5,0.5); 
float fogDensity=0.03;

float computeShadow() 
{ 
   vec3 normalizedCoords = fragPosLightSpace.xyz/fragPosLightSpace.w; 
   normalizedCoords = normalizedCoords * 0.5 + 0.5; 
   float closestDepth = texture(depthMapTexture, normalizedCoords.xy).r; 
   float currentDepth = normalizedCoords.z;
   if (normalizedCoords.z > 1.0f) 
         return 0.7; 
   float bias =  0.005f; 
   float shadow = currentDepth-bias > closestDepth  ? 0.7 : 0.0; 
   return shadow;
} 

void main() { 
    vec4 textColor=texture(diffuseTexture,passTexture);
    float fogFactor = exp(-fogDensity * fogDistance);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    float shadow = computeShadow(); 
    vec3 finalColor;
    if(lumina1==1 || lumina2==1){
         if(lumina1==0){
             shadow=0;
         } 
         finalColor=(1.0f - shadow)*textColor.rgb*color;
    }else{
         finalColor=textColor.rgb;
    }
    if(ceata==1){
     finalColor=mix(fogColor,finalColor,fogFactor);
     }
     fColor = vec4(finalColor,transp);
} 