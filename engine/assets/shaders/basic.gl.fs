#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec4 color;
uniform vec4 ambient_dir;
uniform vec4 ambient_color;
uniform vec4 light_pos;
uniform vec4 light_color;

void main()
{
    vec3 n = normalize(Normal);

    // ambient light
    vec3 ambient_n = normalize(-ambient_dir.xyz);
    float ambient_power = max(dot(n, ambient_n), 0.0);
    vec3 ambient_albeto = vec3(ambient_power * ambient_color);

    // point light
    vec3 light_n = vec3(normalize(light_pos.xyz - FragPos));
    float dist = distance(light_pos.xyz, FragPos);
    float light_power = max(dot(n, light_n), 0.0); // * (1.0 / (dist * dist + 1.0));
    vec3 light_albeto = vec3(light_power * light_color);

    vec3 final_color = (light_albeto + ambient_albeto) * color.rgb;
    FragColor = vec4(final_color.r, final_color.g, final_color.b, 1.0f);
}
