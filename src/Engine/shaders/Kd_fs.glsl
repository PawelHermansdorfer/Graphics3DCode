#version 420

uniform sampler2D map_Kd; 

layout(std140, binding=0) uniform KdMaterial {
    vec4 Kd;
    bool use_vertex_color; 
    bool use_map_Kd;
};

layout(location=0) in vec3 vColor;
layout(location=1) in vec2 vertex_texcoord_0;

layout(location=0) out vec4 vFragColor;

vec3 srgb_gamma_correction(vec3 color) {
   color = clamp(color, 0.0, 1.0);
   color = mix(color * 12.92, (1.055 * pow(color, vec3(1.0 / 2.4))) - 0.055, step(0.0031308, color));
   return color;
}

void main() {
    vec4 color = Kd;

    if(use_vertex_color)
    {
        color = color * vec4(vColor, 0);
    }

    if(use_map_Kd)
    {
        vec4 texture_color = texture(map_Kd, vertex_texcoord_0);
        color = color * texture_color;
    }

    vFragColor.a = color.a;
    vFragColor.rgb = srgb_gamma_correction(color.rgb);
}
