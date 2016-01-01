#version 330

in vec3 pos;

uniform vec3 lightPos;

out float fragColor;

void main()
{
    fragColor = length(pos - lightPos);
}