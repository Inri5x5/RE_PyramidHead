#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec3 vPos;
out vec2 vTexCoords;
out vec3 vNormal;

uniform mat4 uModelView;
uniform mat4 uProjection;

void main()
{
    vTexCoords = aTexCoords;
    vec4 view_pos = uModelView * aPos;
    gl_Position = uProjection*view_pos;

    vNormal = mat3(uModelView) * aNormal;
    vPos = view_pos.xyz;

}