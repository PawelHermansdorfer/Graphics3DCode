#version 420

layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
};

layout(location=0) in vec4 a_vertex_position;
layout(location=1) in vec3 fColor;

layout(location=0) out vec3 vColor;


void main() {
    gl_Position =  PVM * a_vertex_position;
    vColor = fColor;
}
