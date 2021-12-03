#version 330 core

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vPos;

// Maps
uniform mat4 v;
uniform sampler2D uTexMap;
uniform sampler2D uSpecMap;
uniform float uTexFactor;
uniform vec4 uColor;
uniform float uSpecFactor;
uniform vec4 uSpecColor;

// Lighting parameters
uniform vec3 uLightAPos;
uniform vec3 uLightAColor;
uniform float uLightAAttenuation;
uniform vec3 uLightBPos;
uniform vec3 uLightBColor;
uniform float uLightBAttenuation;

// Model parameters
uniform vec3 uDiffuse;
uniform vec3 uSpecular;
uniform vec3 uEmissive;
uniform float uShininess;

out vec4 color;

// Convert an SRGB color to linear space
vec3 srgb_to_linear(vec3 color) {
    return pow(color, vec3(2.2));
}

// Convert a linear color to SRGB space
vec3 linear_to_srgb(vec3 color) {
    return pow(color, vec3(1.0 / 2.2));
}

void main() {
    vec4 fFragColor = mix(uColor, texture(uTexMap, vTexCoord), uTexFactor);
    vec4 specColor = mix(uSpecColor, texture(uSpecMap, vTexCoord), uSpecFactor);
    vec3 frag3 = srgb_to_linear(vec3(fFragColor.x, fFragColor.y, fFragColor.z));
    vec3 spec3 = srgb_to_linear(vec3(specColor.x, specColor.y, specColor.z));

    vec3 diff_col = srgb_to_linear(uDiffuse * frag3.rgb);
    vec3 spec_col = srgb_to_linear(uSpecular * spec3.rgb);
    vec3 emit_col = srgb_to_linear(uEmissive);

    int i; vec3 uLightPos; vec3 uLightColor; float uLightAttenuation; 
    vec3 total_color_linear = vec3(0,0,0);

    // calculate point light contribution
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            uLightPos = uLightAPos;
            uLightColor = uLightAColor;
            uLightAttenuation = uLightAAttenuation;
        } else {
            uLightPos = uLightBPos;
            uLightColor = uLightBColor;
            uLightAttenuation = uLightBAttenuation;
        }

        vec3 normal = normalize(vNormal);
        vec3 view_light_pos = (v * vec4(uLightPos, 1)).xyz;
        vec3 light_dir = normalize(view_light_pos - vPos);
        float light_dist = length(view_light_pos - vPos);
        vec3 view_dir = normalize(-vPos);
        vec3 reflection = reflect(-light_dir, normal);
        vec3 halfway = normalize(light_dir + view_dir);

        vec3 light_col = srgb_to_linear(uLightColor);

        vec3 diffuse = diff_col * max(dot(normal, light_dir), 0) * light_col;

        vec3 specular;
        bool use_blinn = true;
        if (use_blinn) {
            specular = spec_col * pow(max(dot(normal, halfway), 0), uShininess) * light_col;
        } 

        vec3 color_linear = vec3(0, 0, 0);

        float attenuation = 1.0 / (1.0 + uLightAttenuation * light_dist);
        color_linear += (diffuse + specular) * attenuation;
        total_color_linear = total_color_linear + color_linear;
    }
    // total_color_linear += (sun_diffuse + sun_specular);

    // calculate emissive contribution
    total_color_linear += emit_col;
    // linear -> sRGB conversion
    color = vec4(linear_to_srgb(total_color_linear), 1.0);
    //color = mix(uColor, texture(uTexMap, vTexCoord), uTexFactor);

}
