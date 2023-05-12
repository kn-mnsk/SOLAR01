#version 450 core

out vec4 FragColor;
in VS_OUT {
    vec3 ModelPos;
    vec3 WorldPos;
    vec3 WorldSphereCenterPos;
    vec3 ModelNormal;
    vec3 WorldNormal;
    vec2 TexCoords;
    mat4 model;
} frag_in;


// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// material parameters
uniform sampler2D baseColorTexture;  // albedo
//uniform samplerCube baseColorCubemap;
uniform sampler2D metallicRoughnessTexture; // metallic and roughness
uniform sampler2D normalTexture; // normal
uniform sampler2D occlusionTexture; // ao
uniform sampler2D emissiveTexture; //what is this?? to be coded

uniform bool existBaseColorTexture;
uniform bool existMetallicRoughnessTexture;
uniform bool existNormalTexture;
uniform bool existOcclusionTexture;
uniform bool existEmissiveTexture;

uniform vec3 emissiveFactor;/// The factors for the emissive color of the material.
uniform float alphaCutoff; // The alpha cutoff value of the material.
uniform   bool doubleSided; // Specifies whether the material is double sided.

uniform vec4 baseColorFactor;// The factors for the base color of the material.
uniform float metallicFactor;// The factor for the metalness of the material.
uniform float roughnessFactor;// The factor for the roughness of the material.

uniform float scale;// The scalar parameter applied to each normal vector of the normal texture.

uniform float strength;  // A scalar multiplier controlling the amount of occlusion applied.

uniform  int mappingMethod; // not used 
 // The set index of texture's TEXCOORD attribute used for texture coordinate mapping. int32_t texCoord;
 //uniform uint textCoordIdx; // should be each texture??

 uniform int starId;  // to exclude sunlight effect for sun
 uniform float F0value;  // to exclude sunlight effect for sun

// lights
uniform mat4 modelsun;
//uniform vec3 SunPosition[9];
//uniform vec3 SunLight[9];

uniform vec3 camPos;

float PI = 3.1415926;
float PI2 = 2.0 * PI;
//float PIq = PI / 4.0;
//float PI2q = 2.0 * PIq;
//float PI3q = 3.0 * PIq;
//float PI4q = 4.0 * PIq;
//float PI5q = 5.0 * PIq;
//float PI6q = 6.0 * PIq;
//float PI7q = 7.0 * PIq;
//float SinePIq = sin(PIq);
//float TanPIq = tan(PIq);
//float SQRT2 = 1.41421356237;
//const float isqrt2 = SQRT2 / 2.0; //0.70710676908493042;
//
//float wLM = 0.0;//0.0056; // letf margin of width 
//float wRM = 0.0;//0.0056;
//float hTM = 0.0;//0.0056; // top margin of height
//float hBM = 0.0;//0.0056; // bottom margin of height
//float hDivide = 1.0 / 3.0;
//float wDivide = 1.0 / 4.0;


vec3 getNormalFromMap(vec2 texcoord)
{
    vec3 tangentNormal = textureLod(normalTexture, texcoord, 0.0).xyz * 2.0 - 1.0;

    //vec3 Q1  = dFdx(frag_in.ModelPos);
    vec3 Q1  = dFdx(frag_in.WorldPos);
    //vec3 Q2  = dFdy(frag_in.ModelPos);
    vec3 Q2  = dFdy(frag_in.WorldPos);
    //vec2 st1 = dFdx(frag_in.TexCoords);
    vec2 st1 = dFdx(texcoord);
    //vec2 st2 = dFdy(frag_in.TexCoords);
    vec2 st2 = dFdy(texcoord);

    vec3 N   = normalize(frag_in.WorldNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    //vec3 B  = normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   

mat3 getMat3TBN(vec3 pos, vec3 tangentnormal, vec2 texcoord)
{ // note: pos in World space, tangentnormal  from normal texture
// texcoord for normal texture sampling
    
    vec3 Q1  = dFdx(pos);
    vec3 Q2  = dFdy(pos);
    vec2 st1 = dFdx(texcoord);
    vec2 st2 = dFdy(texcoord);

    vec3 N   = normalize(tangentnormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    //vec3 B  = normalize(cross(N, T));
    mat3 TBN = transpose(mat3(T, B, N));

    return TBN;
}

mat3 getMat3TBN2(vec3 worldpos, vec3 worldnormal, vec2 texcoord)
{ // note: pos in World space, tangentnormal  from normal texture
// texcoord for normal texture sampling
    vec3 Q1  = dFdx(worldpos);
    //vec3 Q1  = dFdx(worldpos -  frag_in.WorldSphereCenterPos);
    vec3 Q2  = dFdy(worldpos);
    //vec3 Q2  = dFdy(worldpos -  frag_in.WorldSphereCenterPos);
    vec2 st1 = dFdx(texcoord);
    vec2 st2 = dFdy(texcoord);

    vec3 N   = worldnormal;
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    //vec3 B  = normalize(cross(N, T));
    //T = normalize(cross(B, N));
    //mat3 TBN = transpose(mat3(T, B, N));
    mat3 TBN = mat3(T, B, N);
    //mat3 TBN = mat3(T, N, B);

    return TBN;
}

mat3 getMat3TBN3(vec3 modelpos, vec3 modelnormal, vec2 texcoord)
{ // note: pos in World space, tangentnormal  from normal texture
// texcoord for normal texture sampling
    vec3 Q1  = dFdx(modelpos);
    vec3 Q2  = dFdy(modelpos);
    vec2 st1 = dFdx(texcoord);
    vec2 st2 = dFdy(texcoord);

    float f = 1.0f / (st1.x * st2.y - st2.x * st1.y);
    vec3 T  = normalize(f * (st2.y * Q1 - st1.y * Q2));
    vec3 B  =normalize(f * (-st2.x * Q1 + st1.x * Q2));
     vec3 N   = modelnormal;

//     T = normalize(vec3(frag_in.model * vec4(T,   0.0)));
//     B = normalize(vec3(frag_in.model * vec4(B,   0.0)));
//     N = normalize(vec3(frag_in.model * vec4(N,   0.0)));
     
//      T = normalize(vec3(frag_in.model * vec4(T,   0.0)));
//     B = normalize(vec3(frag_in.model * vec4(B,   0.0)));
//     N = normalize(vec3(transpose(inverse(frag_in.model)) * vec4(N, 0.0)));

     T = normalize(vec3(transpose(inverse(frag_in.model)) * vec4(T, 0.0)));
     B = normalize(vec3(transpose(inverse(frag_in.model)) * vec4(B, 0.0)));
     N = normalize(vec3(transpose(inverse(frag_in.model)) * vec4(N, 0.0)));

    mat3 TBN = mat3(T, B, N);

    return TBN;
}

float radicalInverse_VdC(uint bits);
vec2 hammersley2d(uint i, uint N);
vec3 hemisphereSample_uniform(float u, float v);
vec3 hemisphereSample_cos(float u, float v);

void main()
{		
    vec3 albedo, N, V, R, F0, F, kS, kD, emissive, ambient, diffuse, specular,  prefiltered, irradiance, 
                color, radiance;
    vec2 brdf, texcoord;
    float metallic, roughness, ao;
    mat3 TBN;

    texcoord = frag_in.TexCoords;

//    vec3 tangentNormal = normalize(texture(normalTexture, texcoord).xyz * 2.0 - 1.0);
//    TBN = getMat3TBN(frag_in.WorldPos, frag_in.WorldNormal, texcoord);
//
    if (existBaseColorTexture)
    {       
        //albedo     = pow(textureLod(baseColorTexture, texcoord, 0.0).rgb, vec3(2.2));
        albedo     = textureLod(baseColorTexture, texcoord, 0.0).rgb;
        //albedo     = texture(baseColorTexture, texcoord).rgb;
    } else
    {
        //albedo     =  pow(baseColorFactor.rgb, vec3(2.2));
        albedo     =  baseColorFactor.rgb;
    }


    if (existMetallicRoughnessTexture)
    {
        metallic  = texture(metallicRoughnessTexture, texcoord).r;
        roughness = texture(metallicRoughnessTexture, texcoord).r;
    } else
    {
        metallic = metallicFactor;
        roughness = roughnessFactor;
    }
     
    if (existNormalTexture)
    {
    // obtain normal from normal map in range [0,1] and transform normal vector to range [-1,1]
        vec3 tangentNormal = normalize(textureLod(normalTexture, texcoord, 0.0).xyz * 2.0 - 1.0);
        //vec3 tangentNormal = normalize(texture(normalTexture, texcoord).xyz * 2.0 - 1.0);
        //TBN = getMat3TBN(frag_in.WorldPos, frag_in.WorldNormal, texcoord);
        //TBN = getMat3TBN2(frag_in.WorldPos, frag_in.WorldNormal, texcoord);
        TBN = getMat3TBN3(frag_in.ModelPos, frag_in.ModelNormal, texcoord);
        N = normalize(TBN * tangentNormal); // transformation to World Space
        //V = normalize(TBN * (camPos - frag_in.WorldPos)); // looks better, why?
        V = normalize((camPos - frag_in.WorldPos));
    } else
    {
        //N = normalize(frag_in.WorldNormal);
        N = frag_in.WorldNormal;
        V = normalize(camPos - frag_in.WorldPos);
    }
    R = reflect(-V, N); 


    if (existOcclusionTexture)
    {
        //ao        = texture(occlusionTexture, texcoord).r;
        ao        = textureLod(occlusionTexture, texcoord, 0.0).r;
    } else
    {
        ao = strength;
    }
 
 if (existEmissiveTexture)
    {
        //emissive        = texture(emissiveTexture, texcoord).rgb;
        emissive        = textureLod(emissiveTexture, texcoord, 0.0).rgb;
    } else
    {
        emissive = vec3(0.0, 0.0, 0.0); //;emissiveFactor;
    }

//    V = normalize(TBN * (camPos - frag_in.WorldPos));
//    //V = normalize(camPos - frag_in.WorldPos);
//    R = reflect(-V, N); 

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    //F0 = vec3(0.29); //vec3(0.04);  // should vary accpording to each star  earth = 0.29
    F0 = vec3(F0value); // each star  earth 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation

    // Sun light calculation
    const uint SAMPLE_COUNT = 512u;
    vec3 Lights = vec3(0.0);
     vec3 SunLight = vec3(1.0, 1.0, 1.0);
    vec3 SunLightPos[2];
    vec3 L[2];
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        // calculate per-light radiance
        vec2 Xi = hammersley2d(i, SAMPLE_COUNT);
       float scaling = 3.0; 
        //SunLightPos[0] = hemisphereSample_uniform(Xi.x, Xi.y) * scaling;
        SunLightPos[0] = hemisphereSample_cos(Xi.x, Xi.y) * scaling;
        SunLightPos[1] = vec3(SunLightPos[0].x, -SunLightPos[0].y, SunLightPos[0].z);

        SunLightPos[0] = vec3(modelsun * vec4(SunLightPos[0], 1.0));
        SunLightPos[1] = vec3(modelsun * vec4(SunLightPos[1], 1.0));

        for (uint j = 0; j<2; ++j)
        {
             L[j]= normalize(SunLightPos[j] - frag_in.WorldPos);

             if (dot(L[j], N) <= 0) continue;

            vec3 H = normalize(V + L[j]);  // halfway direction
            float distance = length(L[j]);
            float attenuation = 1.0 / (distance * distance);

            //float intensity = 2.0 / (2.0 *SAMPLE_COUNT);
            float intensity = 1.1 / (2.0 *SAMPLE_COUNT);
            radiance =  SunLight * attenuation * intensity;

            // Cook-Torrance BRDF
            float NDF = DistributionGGX(N, H, roughness);   
            float G   = GeometrySmith(N, V, L[j], roughness);      
            vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
            vec3 nominator    = NDF * G * F; 
            float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L[j]), 0.0) + 0.001; // 0.001 to prevent divide by zero.
            vec3 specular = nominator / denominator;
        
            // kS is equal to Fresnel
            vec3 kS = F;
            // for energy conservation, the diffuse and specular light can't
            // be above 1.0 (unless the surface emits light); to preserve this
            // relationship the diffuse component (kD) should equal 1.0 - kS.
            vec3 kD = 1.0 - kS;
            // multiply kD by the inverse metalness such that only non-metals 
            // have diffuse lighting, or a linear blend if partly metal (pure metals
            // have no diffuse light).
            kD *= 1.0 - metallic;	  

            // scale light by NdotL
            float NdotL = max(dot(N, L[j]), 0.0);        

            // add to outgoing radiance Lights
            //Lights += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
            //Lights += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
            Lights += (kD * albedo + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
        }
    }   
    
     // ambient lighting (we now use IBL as the ambient term)
    F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    kS = F;
    kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
   // vec3 irradiance = texture(irradianceMap, N).rgb;
    //diffuse      = irradiance * albedo;
    //diffuse      = kD * albedo / PI;  // also looks okay
    diffuse      = kD * albedo;  // also looks okay
    
     //const float MAX_REFLECTION_LOD = 4.0; // original 4.0
    //prefiltered = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
    //prefiltered = textureLod(prefilterMap, Pos,  roughness * MAX_REFLECTION_LOD).rgb;
    //prefiltered = texture(prefilterMap, Pos).rgb;

    //brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    //specular = prefiltered * (F * brdf.x + brdf.y);
    //specular = prefiltered * kD;// * 0.5 + 0.5);
    //specular = prefiltered * (kD * brdf.x + brdf.y);


    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    //ambient = (kD * diffuse + specular) * ao;;
    ambient = diffuse * ao;
  

    if (starId == 0) // Sun; eleiminate light effect of itself
    {
        ambient /= PI;
        ambient = pow(ambient, vec3(2.2)); 
        //color = emissive + ambient;
    } else
    {
        //ambient /= PI;
        //ambient = pow(ambient, vec3(2.2)); 
        ambient += Lights;
       }

    color = emissive + ambient;

    FragColor = vec4(color, 1.0);

    }


// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap(vec3 pos, vec3 normal, vec2 texcoord)
{
    vec3 tangentNormal = normalize(texture(normalTexture, texcoord).xyz * 2.0 - 1.0);

    vec3 Q1  = dFdx(pos);
    vec3 Q2  = dFdy(pos);
    vec2 st1 = dFdx(texcoord);
    vec2 st2 = dFdy(texcoord);

    vec3 N   = normalize(normal);
    
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = transpose(mat3(T, B, N));

    return normalize(TBN * tangentNormal);
}

float radicalInverse_VdC(uint bits) {
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
 }

 vec2 hammersley2d(uint i, uint N) {
     return vec2(float(i)/float(N), radicalInverse_VdC(i));
 }

 vec3 hemisphereSample_uniform(float u, float v) {
     float phi = v * 2.0 * PI;
     float cosTheta = 1.0 - u;
     float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
     return vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
 }
    
 vec3 hemisphereSample_cos(float u, float v) {
     float phi = v * 2.0 * PI;
     float cosTheta = sqrt(1.0 - u);
     float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
     return vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
 }
