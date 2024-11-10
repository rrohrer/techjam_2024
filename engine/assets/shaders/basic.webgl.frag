#version 300 es
precision highp float;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec4 color;
uniform vec4 ambient_dir;
uniform vec4 ambient_color;
uniform vec4 light_pos;
uniform vec4 light_color;
uniform vec4 camera_eye;
uniform vec4 fog_color;
// fog props x and y are near and far start/end for in camera fog
uniform vec4 fog_props;

float get_fog(float d) {
  if (d>= fog_props.y) return 1.0;
  if (d<= fog_props.x) return 0.0;
  return 1.0 - (fog_props.y - d) / (fog_props.y - fog_props.x);
}

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

    // fog
    float fog_dist = distance(camera_eye.xyz, FragPos);
    float fog_alpha = get_fog(fog_dist);

    vec3 lit_color = (light_albeto + ambient_albeto) * color.rgb;
    vec3 final_color = mix(lit_color, fog_color.rgb, fog_alpha);
    FragColor = vec4(final_color.r, final_color.g, final_color.b, 1.0f);
}

