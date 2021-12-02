#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

noperspective in vec2 vTexCoord;
in vec3 vPos;

uniform sampler2D uTexMap;

void main() {
    gPosition = vPos;
    gAlbedo.rgb = texture(uTexMap, vTexCoord).rgb;
}
