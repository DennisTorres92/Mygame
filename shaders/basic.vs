#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normals;
layout(location = 2) in vec2 a_texture;

out vec3 v_position;
out vec2 v_texturecord;
out vec3 v_normals;
uniform mat4 u_modelViewProj;
uniform mat4 u_modelView;
uniform mat4 u_invModelView;
uniform mat4 u_model;

void main(){
    gl_Position = u_modelViewProj * vec4(a_position, 1.0f);
    v_position = vec3(u_modelView * vec4(a_position, 1.0f));
    v_texturecord = a_texture;
    v_normals = mat3(u_invModelView) * a_normals;
}
