#version 300 es
precision highp float;

out vec4 FragColor;

uniform vec4 color;

void main()
{
    FragColor = vec4(color.r, color.g, color.b, 1.0f);
}

