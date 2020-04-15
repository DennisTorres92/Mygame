#version 330 core
layout(location = 0) out vec4  f_color;

in vec3 v_position;
in vec2 v_texturecord;
in vec3 v_normals;

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

uniform Material u_material;
uniform Directionallight u_directional;
uniform sampler2D u_texture;

void main(){
    vec3 view = normalize(-v_position);
    vec3 normal = normalize(v_normals);
    vec3 light = normalize(-u_directional.direction); //lightposition

    vec3 reflection = reflect(u_directional.direction, normal);
    vec3 ambient = u_directional.ambient * u_material.diffuse;
    vec3 diffuse = u_directional.diffuse * max(dot(normal, light), 0.000001) * u_material.diffuse;
    vec3 specular = u_directional.specular * pow(max(dot(reflection, light), 0), u_material.shininess) * u_material.specular;
    f_color = vec4(ambient + diffuse + specular + u_material.emissive, 1.0f) ;

    vec4 texColor = texture(u_texture, v_texturecord);
    
    
}
