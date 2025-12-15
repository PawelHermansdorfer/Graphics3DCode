#version 420

layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
    mat4 VM;
    mat3 VM_normal;
};

layout(location=0) in vec4 a_vertex_position;
layout(location=1) in vec3 a_vertex_normal;
layout(location=3) in vec2 a_vertex_texcoord_0;
layout(location=5) in vec3 a_vertex_color;

layout(location=0) out vec3 vColor;
layout(location=1) out vec2 vertex_texcoord_0;
layout(location=2) out vec3 vertex_normal_vs;
layout(location=3) out vec3 vertex_position_vs;


void main() {
    gl_Position =  PVM * a_vertex_position;
    vColor = a_vertex_color;
    vertex_texcoord_0 = a_vertex_texcoord_0;

    vertex_normal_vs = normalize(VM_normal * a_vertex_normal);

    vec4 a_vertex_position_vs = VM * a_vertex_position;
    vertex_position_vs = a_vertex_position_vs.xyz/a_vertex_position_vs.w;
}
