#version 410 core 

layout(location=0) in vec3 vPosition; 
layout(location=1) in vec3 vNormal; 
layout(location=2) in vec2 textcoord; 

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection; 
uniform mat3 normalMatrix; 

uniform vec3 lightDirLiniar; 
uniform vec3 lightColorLiniar; 

uniform vec3 lightDirPunct; 
uniform vec3 lightColorPunct; 

uniform vec3 lightPosR1; 
uniform vec3 lightPosR2; 
uniform vec3 lightPosB1; 
uniform vec3 lightPosB2; 

uniform int stareLumina;
uniform int umbre;

uniform int lumina1;
uniform int lumina2;

out vec3 color; 
out vec2 passTexture;
out float fogDistance;

vec3 ambientL; 
vec3 diffuseL; 
vec3 specularL; 



float ambientStrength = 0.1f;
float specularStrength = 0.5f; 
float shininess = 32.0f; 

float constant = 1.0f; 
float linear = 10.09f; 
float quadratic = 1.032f; 

float ambientStrengthP = 100;
float specularStrengthP = 20; 
float shininessP = 32.0f; 

out vec4 fragPosLightSpace; 
uniform mat4 lightSpaceTrMatrix;

vec3 ambient=vec3(0,0,0); 
vec3 diffuse=vec3(0,0,0); 
vec3 specular=vec3(0,0,0);

void sorce(vec3 lPosEye,vec3 lightColor){
    vec3 normalEye = normalize(normalMatrix * vNormal);
    vec4 vertPosEye = model * vec4(vPosition, 1.0f);
    vec3 viewDir = normalize(-vertPosEye.xyz);
  
    vec3 lightDirPunctN = normalize(lPosEye - vertPosEye.xyz);
    float dist = length(lPosEye - vertPosEye.xyz);  
    float att = 1.0f / (constant + linear * dist + quadratic * (dist * dist));
    vec3 ambientP = att * ambientStrengthP * lightColor; 
    vec3 diffuseP = att * max(dot(normalEye, lightDirPunctN), 0.0f) * lightColor;
    vec3 reflectDirP = normalize(reflect(-lightDirPunctN, normalEye)); 
    float specCoeffP = pow(max(dot(viewDir, reflectDirP), 0.0f), shininessP);
    vec3 specularP = att * specularStrengthP * specCoeffP * lightColor;
    if(dist<2){
    ambientP*=2;
    diffuseP*=2;
    specularP*=2;
    }
    if(dist <3){
    ambient+=ambientP;
    diffuse+=diffuseP;
    specular+=specularP;
    }
}

void main() 
{   
    if(umbre==1){
       fragPosLightSpace = lightSpaceTrMatrix * model * vec4(vPosition, 1.0f);

       vec3 normalEye = normalize(normalMatrix * vNormal);
       vec4 vertPosEye = view * model * vec4(vPosition, 1.0f);
       vec3 viewDir = normalize(-vertPosEye.xyz);

       ambientL = ambientStrength * lightColorLiniar; 
       vec3 lightDirLiniarN = normalize(lightDirLiniar);  
       diffuseL = max(dot(normalEye, lightDirLiniarN), 0.0f) * lightColorLiniar;  
       vec3 reflectDirL = normalize(reflect(-lightDirLiniarN, normalEye)); 
       float specCoeffL = pow(max(dot(viewDir, reflectDirL), 0.0f), shininess); 
       specularL = specularStrength * specCoeffL * lightColorLiniar;

       if(lumina1==1){
          ambient+=ambientL;
          diffuse+=diffuseL;
          specular+=specularL;
       }
       if(lumina2==1){
          if(stareLumina==1){
             sorce(lightPosR1,vec3(1,0,0));
             sorce(lightPosB2,vec3(0,0,1));
          }else{
             sorce(lightPosB1,vec3(0,0,1));
             sorce(lightPosR2,vec3(1,0,0));
          }
       }
       color = min(ambient + diffuse + specular, 1.0f);
    }else{
       color= vec3(1,1,1);
    }
    fogDistance = length((view * model * vec4(vPosition, 1.0f)).xyz);
    passTexture = textcoord;

    gl_Position = projection * view * model * vec4(vPosition, 1.0f); 
}
