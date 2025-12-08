#version 420

layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
};

layout(location=0) in vec4 a_vertex_position;
layout(location=5) in vec3 a_vertex_color;

layout(location=0) out vec3 vColor;


void main() {
    gl_Position =  PVM * a_vertex_position;
    vColor = a_vertex_color;
}
