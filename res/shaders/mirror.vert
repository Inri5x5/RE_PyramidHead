#version 330 core

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec2 aNormal;

noperspective out vec2 vTexCoord;

void main() {

    mat4 mv = v * m;
    vec4 view_pos = mv * vec4(aPos.x, aPos.y, aPos.z, 1);
    gl_Position = p * view_pos;
    vTexCoord = (gl_Position.xy / gl_Position.w) * 0.5 + vec2(0.5);
    vTexCoord.x = 1 - vTexCoord.x;

}