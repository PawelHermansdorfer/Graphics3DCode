#version 410

layout(location=0) in vec3 vColor;

layout(location=0) out vec4 vFragColor;

void main() {
    vFragColor = vec4(vColor.xyz, 1.0);
}
