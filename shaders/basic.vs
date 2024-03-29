#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normals;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec2 a_texture;

out vec3 v_position;
out vec2 v_texturecord;
out vec3 v_normal;
out mat3 v_tbn;
uniform mat4 u_modelViewProj;
uniform mat4 u_modelView;
uniform mat4 u_invModelView;
uniform mat4 u_model;

void main(){
    gl_Position = u_modelViewProj * vec4(a_position, 1.0f);
    v_position = vec3(u_modelView * vec4(a_position, 1.0f));
    v_texturecord = a_texture;
    vec3 t = normalize(mat3(u_invModelView) * a_tangent);
    vec3 n = normalize(mat3(u_invModelView) * a_normals);
    t = normalize(t - dot(t, n) * n); 
    vec3 b = normalize(mat3(u_invModelView) * cross(n, t));
    mat3 tbn = transpose(mat3(t, b, n)); 
    v_tbn = tbn;
    v_normal = mat3(u_invModelView) * a_normals;
}
