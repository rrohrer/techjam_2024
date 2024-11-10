#version 300 es
in vec3 aPos;
in vec3 aColor;

uniform mat4 view_proj;

out vec3 Color;

void main()
{
    vec4 vert = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    gl_Position = view_proj * vert;
    Color = aColor;
}

