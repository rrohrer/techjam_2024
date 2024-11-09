#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 view_proj;
uniform mat4 model;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    vec4 vert = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    gl_Position = view_proj * model * vert;
    FragPos = vec3(model * vert);
    Normal = aNormal;
}
