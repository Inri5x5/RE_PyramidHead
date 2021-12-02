#version 330 core
out vec4 color;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
// Lighting parameters
uniform mat4 v;
uniform vec3 uLightAPos;
uniform vec3 uLightAColor;
uniform float uLightAAttenuation;
uniform vec3 uLightBPos;
uniform vec3 uLightBColor;
uniform float uLightBAttenuation;

// Convert an SRGB color to linear space
vec3 srgb_to_linear(vec3 color) {
    return pow(color, vec3(2.2));
}

// Convert a linear color to SRGB space
vec3 linear_to_srgb(vec3 color) {
    return pow(color, vec3(1.0 / 2.2));
}

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedo, TexCoords).rgb;
    float Specular = texture(gAlbedo, TexCoords).a;
    
    vec3 lighting  = Diffuse * 0.1; //

    int i; vec3 uLightPos; vec3 uLightColor; float uLightAttenuation; 
    vec3 total_color_linear = vec3(0,0,0);
    vec3 viewDir  = normalize(-FragPos);
    

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
        vec3 view_light_pos = (v * vec4(uLightPos, 1)).xyz;
        // diffuse
        vec3 lightDir = normalize(uLightPos - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * uLightColor;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = uLightColor * spec * Specular;
        // attenuation
        float distance = length(uLightPos - FragPos);
        float attenuation = 1.0 / (1.0 + uLightAttenuation * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;   


        // vec3 normal = normalize(Normal);
        // vec3 view_light_pos = (v * vec4(uLightPos, 1)).xyz;
        // vec3 light_dir = normalize(view_light_pos - FragPos);
        // float light_dist = length(view_light_pos - FragPos);
        // vec3 view_dir = normalize(-FragPos);
        // vec3 reflection = reflect(-light_dir, normal);
        // vec3 halfway = normalize(light_dir + view_dir);

        // vec3 light_col = srgb_to_linear(uLightColor);

        // vec3 diffuse = Diffuse * max(dot(normal, light_dir), 0) * light_col;

        // vec3 specular;
        // specular = Specular * pow(max(dot(normal, halfway), 0), 0.0) * light_col;
    
        // vec3 color_linear = vec3(0, 0, 0);

        // float attenuation = 1.0 / (1.0 + uLightAttenuation * light_dist);
        // color_linear += (diffuse + specular) * attenuation;
        // total_color_linear = total_color_linear + color_linear;
    }
    color = vec4(linear_to_srgb(total_color_linear), 1.0);
    color = vec4(lighting, 1.0);
    color = vec4(Diffuse, 1.0);
}
