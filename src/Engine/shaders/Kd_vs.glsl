#version 420

layout(std140, binding=0) uniform KdMaterial {
    vec4 Kd;
    bool use_vertex_color; 
    bool use_map_Kd;
};

layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
};

layout(location=0) in vec4 a_vertex_position;
layout(location=3) in vec2 a_vertex_texcoord_0;
layout(location=5) in vec3 a_vertex_color;

layout(location=0) out vec3 vColor;
layout(location=1) out vec2 vertex_texcoord_0;


void main() {
    gl_Position =  PVM * a_vertex_position;
    vColor = a_vertex_color;
    vertex_texcoord_0 = a_vertex_texcoord_0;
}
