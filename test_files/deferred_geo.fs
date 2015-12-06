#version 330

uniform vec3 color;

uniform mat4 model;
uniform mat3 normal_matrix;

in vec2 texCoord;
in vec3 normal;
in vec3 pos;

layout (location = 0) out vec3 posBuff;
layout (location = 1) out vec3 colorBuff;
layout (location = 2) out vec3 normalBuff;

void main()
{
    posBuff = vec3(model * vec4(pos, 1));
    colorBuff = color;
    normalBuff = normalize(normal_matrix * normal);
}