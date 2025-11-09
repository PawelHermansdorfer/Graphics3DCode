#version 420

layout(std140, binding = 0) uniform Mixer {
    float strength;
    vec3  mix_color; 
};

layout(location=0) in vec3 vColor;

layout(location=0) out vec4 vFragColor;

void main() {
    vec3 mixed = mix(vColor, mix_color, strength);
    vFragColor = vec4(mixed, 1.0);
}
