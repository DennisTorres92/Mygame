#version 330 core
layout(location = 0) in vec3 a_positions;
layout(location = 1) in vec3 a_normals;
layout(location = 2) in vec2 a_texture;

out vec3 v_position;
out vec2 v_texturecord;
out vec3 v_normals;
uniform mat4 u_modelViewProj;
uniform mat4 u_ModelView;
uniform mat4 u_invModelView;

void main(){
    gl_Position = u_modelViewProj * vec4(a_positions, 1.0f);
    v_position = vec3(u_ModelView) * a_positions;
    v_texturecord = a_texture;
    v_normals = mat3(u_invModelView) * a_normals;
}