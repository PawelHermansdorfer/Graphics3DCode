#version 410


layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec3 fColor;

layout(location=0) out vec3 vColor;

void main() {
    gl_Position = a_vertex_position;
    vColor = fColor;
}
