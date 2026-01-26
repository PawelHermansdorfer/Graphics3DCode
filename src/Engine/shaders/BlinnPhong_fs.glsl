#version 420

struct PointLight {
    vec3 position;
    float radius;
    vec3 color;
    float intensity;
};

uniform sampler2D map_Kd;

layout(std140, binding=0) uniform Material {
    vec4 Ka;
    vec4 Kd;
    vec3 Ks;
    float Ns;
    bool use_vertex_color; 
    bool use_map_Kd;
};

const int MAX_POINT_LIGHTS = 16;
layout(std140, binding = 2) uniform Lights {
    vec3 ambient;
    int n_lights;
    PointLight lights[MAX_POINT_LIGHTS];
};

layout(location=0) in vec3 vColor;
layout(location=1) in vec2 vertex_texcoord_0;
layout(location=2) in vec3 vertex_normal_vs;
layout(location=3) in vec3 vertex_position_vs;

layout(location=0) out vec4 vFragColor;

vec3 srgb_gamma_correction(vec3 color) {
    color = clamp(color, 0.0, 1.0);
    color = mix(color * 12.92, (1.055 * pow(color, vec3(1.0 / 2.4))) - 0.055, step(0.0031308, color));
    return color;
}

void main() {
    vec3 normal = normalize(vertex_normal_vs);
    vec4 color = Kd;

    if(!gl_FrontFacing) {
        normal = -normal;
    }

    if(use_vertex_color) {
        color = color * vec4(vColor, 0);
    }

    if(use_map_Kd) {
        vec4 texture_color = texture(map_Kd, vertex_texcoord_0);
        color = color * texture_color;
    }

    vFragColor.a = color.a;
    vFragColor.rgb = color.rgb * ambient;

#define PI 3.1415926535897932384626433832795
#define INV_PI (1.0/PI)

    for(int i = 0; i < n_lights; ++i) {
        vec3 light_dir = lights[i].position - vertex_position_vs;
        float light_distance = length(light_dir);
        light_dir = normalize(light_dir);

        vec3 view_dir = normalize(-vertex_position_vs);
        vec3 half_dir = normalize(light_dir + view_dir);

        float diffuse = max(dot(normal, light_dir), 0.0);

        float specular = pow(max(dot(normal, half_dir), 0.0), Ns);

        float r = max(lights[i].radius, light_distance);
        float attenuation = 1.0 / (r * r);

        vec3 diffuse_color = INV_PI * color.rgb * lights[i].color * lights[i].intensity * diffuse * attenuation;
        vec3 specular_color = Ks * lights[i].color * lights[i].intensity * specular * attenuation;
        vFragColor.rgb += diffuse_color + specular_color;
    }
}
