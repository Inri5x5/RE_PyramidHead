#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

in vec2 vTexCoords;
in vec3 vPos;
in vec3 vNormal;

uniform sampler2D uTexMap;
uniform sampler2D uSpecMap;
uniform float uTexFactor;
uniform vec4 uColor;
uniform float uSpecFactor;
uniform vec4 uSpecColor;

// Model parameters
// uniform vec3 uDiffuse;
// uniform vec3 uSpecular;
// uniform vec3 uEmissive;

// // Convert an SRGB color to linear space
// vec3 srgb_to_linear(vec3 color) {
//     return pow(color, vec3(2.2));
// }

// // Convert a linear color to SRGB space
// vec3 linear_to_srgb(vec3 color) {
//     return pow(color, vec3(1.0 / 2.2));
//}

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = vPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(vNormal);
    // and the diffuse per-fragment color
    // gAlbedo.rgb = srgb_to_linear(uDiffuse * mix(uColor, texture(uTexMap, vTexCoords), uTexFactor).rgb);
    // gAlbedo.rgb += srgb_to_linear(uEmissive);
    gAlbedo.rgb = (mix(uColor, texture(uTexMap, vTexCoords), uTexFactor).rgb);
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedo.a = (mix(uSpecColor, texture(uSpecMap, vTexCoords), uSpecFactor).rgb).r;
}