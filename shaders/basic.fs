#version 330 core
layout(location = 0) out vec4  f_color;

in vec3 v_position;
in vec2 v_texturecord;
in vec3 v_normal;
in mat3 v_tbn;

struct Material{
    vec3 specular;
    vec3 diffuse;
    vec3 emissive;
    float shininess;
};
struct Directionallight{
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};
struct PointLight {
    vec3 position;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float linear;
    float quadratic;
};
struct SpotLight {
    vec3 position;
    vec3 direction;
    float innerCone;
    float outerCone;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

uniform Material u_material;
uniform Directionallight u_directional;
uniform PointLight u_pointlight;
uniform sampler2D u_diffusemap;
uniform sampler2D u_normalmap;
uniform SpotLight u_spotlight;
uniform float u_iftextured;

void main(){
    vec3 view = normalize(-v_position);
    vec3 normal;
    vec4 diffuseColor;
    if(u_iftextured == 1.0f){
        normal = texture(u_normalmap, v_texturecord).rgb;
        normal = normalize(normal * 2.0 - 1.0f);
        normal = normalize(v_tbn * normal);
        diffuseColor = texture(u_diffusemap, v_texturecord);
        if(diffuseColor.w < 0.9) {
        discard;
    }
    }else{
        diffuseColor = vec4(u_material.diffuse, 1.0f);
        normal = normalize(v_normal);
    }
    
    
    vec3 light = normalize(-u_directional.direction); //lightposition
    //Color and directionallight(sun)
    vec3 reflection = reflect(u_directional.direction, normal);
    vec3 ambient = u_directional.ambient * diffuseColor.xyz;
    vec3 diffuse = u_directional.diffuse * max(dot(normal, light), 0.000001) * diffuseColor.xyz;
    vec3 specular = u_directional.specular * pow(max(dot(reflection, light), 0), u_material.shininess) * u_material.specular;
    //Pointlights
    light = normalize(u_pointlight.position - v_position);
    reflection = reflect(-light, normal);
    float distance = length(u_pointlight.position - v_position);
    float attentuation = 1.0 / ((1.0) + (u_pointlight.linear*distance) + (u_pointlight.quadratic*distance*distance));
    ambient += attentuation * u_pointlight.ambient * diffuseColor.xyz;
    diffuse += attentuation * u_pointlight.diffuse * max(dot(normal, light), 0.0) * diffuseColor.xyz;
    specular += attentuation * u_pointlight.specular * pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular;
    //Spotlights
    light = normalize(u_spotlight.position - v_position);
    reflection = reflect(-light, normal);
    float theta = dot(light, u_spotlight.direction);
    float epsilon = u_spotlight.innerCone - u_spotlight.outerCone;
    float intensity = clamp((theta - u_spotlight.outerCone) / epsilon, 0.0f, 1.0f);
    if(theta > u_spotlight.outerCone) {
        ambient += u_spotlight.ambient * diffuseColor.xyz;
        diffuse += intensity * u_spotlight.diffuse * max(dot(normal, light), 0.0) * diffuseColor.xyz;
        specular += intensity * u_spotlight.specular * pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular;
    } else {
        ambient += u_spotlight.ambient * diffuseColor.xyz;
    }

    f_color = vec4(ambient + diffuse + specular + u_material.emissive, 1.0f) ;
}
