#version 330

in vec3 pos;

uniform vec3 lightPos;

out float fragColor;

void main()
{
    vec3 L = pos - lightPos;
    fragColor = length(L);
	//float d = length(L);
	//fragColor = vec4(d/100, d/100, d/100, 1.0);
}