#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in  vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out VS_OUT {
    vec3 ModelPos;
    vec3 WorldPos;
    vec3 WorldSphereCenterPos;
    vec3 ModelNormal;
    vec3 WorldNormal;
    vec2 TexCoords;
    mat4 model;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vs_out.TexCoords = aTexCoords;
    vs_out.ModelPos = aPos;
    vs_out.WorldPos = vec3(model * vec4(aPos, 1.0));
    vs_out.WorldSphereCenterPos = vec3(model * vec4(0.0, 0.0, 0.0, 1.0));
    vs_out.ModelNormal =  aNormal;    
    vs_out.WorldNormal = normalize(vec3(transpose(inverse(model)) * vec4(aNormal, 0.0)));   
    vs_out.model =  model;    

    gl_Position =  projection * view * model * vec4(aPos, 1.0);
}