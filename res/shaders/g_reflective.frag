#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

in vec3 v_tex_coord;
in vec3 vPos;
in vec3 vNormal;

uniform samplerCube u_texture;


void main() {
    gPosition = vPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(vNormal);
    // and the diffuse per-fragment color
    gAlbedo.rgb = texture(u_texture, v_tex_coord).rgb;

}
