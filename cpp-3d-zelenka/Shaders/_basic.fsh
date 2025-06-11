#version 330 core

//текстурный самплер карты глубины
uniform sampler2D depthTexture;

struct Material {
    vec4 ambient, diffuse, emission, specular;
    float shininess, opacity;
    bool texture_present, two_sided;
    sampler2D texture;
};

struct DirectLight{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

in vec3 v_normal;
in vec2 v_texel;
in vec3 v_light_dir, v_view_dir;
in vec4 v_smcoord;

out vec4 color;

uniform Material material;
uniform DirectLight direct_light;

float my_shadow();

void main() {
    // нормализуем полученные данные для коррекции интерполяции
    vec3 normal   = normalize(v_normal);
    vec3 light_dir = normalize(v_light_dir);
    vec3 view_dir  = normalize(v_view_dir);

    // добавим собственное свечение материала
    vec4 color1 = material.emission;

    // добавим фоновое освещение
    color1 += material.ambient * direct_light.ambient;

    // добавим рассеянный свет
    float NdotL = max(dot(normal, light_dir), 0.0);
    color1 += material.diffuse * direct_light.diffuse * NdotL;

    //добавим отраженный свет
    float RdotVpow = max(pow(dot(reflect(-light_dir, normal), view_dir), material.shininess), 0.0);
    color1 += material.specular * direct_light.specular * RdotVpow;

    // затенение
    float shadow = 1.0 ; // my_shadow();

    color = color1*texture(depthTexture, v_smcoord.xy).r;

    if(material.texture_present)
        color = color1*texture2D(material.texture, v_texel) * shadow;
    else
        color = color1*shadow;
}

// сглаживание границы тени, используем PCF
float my_shadow() {
//    const float kTransparency = 0.15;
//    float mapScale = 1.0/2048.0;

    // perform perspective divide
    vec3 projCoords = v_smcoord.xyz / v_smcoord.w;
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//    float closestDepth = texture2D(depthTexture, projCoords.xy).r;
    float closestDepth = texture(depthTexture, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;

//    float shadow;
//    shadow =  shadow2D(depthTexture, smcoord).x;
//    shadow += shadow2D(depthTexture, smcoord + vec3( mapScale,  mapScale, 0)).x;
//    shadow += shadow2D(depthTexture, smcoord + vec3( mapScale,  -mapScale, 0)).x;
//    shadow += shadow2D(depthTexture, smcoord + vec3( mapScale,  0, 0)).x;
//    shadow += shadow2D(depthTexture, smcoord + vec3( -mapScale,  mapScale, 0)).x;
//    shadow += shadow2D(depthTexture, smcoord + vec3( -mapScale,  -mapScale, 0)).x;
//    shadow += shadow2D(depthTexture, smcoord + vec3( -mapScale,  0, 0)).x;
//    shadow += shadow2D(depthTexture, smcoord + vec3( 0,  mapScale, 0)).x;
//    shadow += shadow2D(depthTexture, smcoord + vec3( 0,  -mapScale, 0)).x;
//    shadow = shadow / 9.0;

//    shadow += kTransparency;

    if(shadow > 0.6) shadow = 1.0; //убираем возможный муар
    return shadow;
}
