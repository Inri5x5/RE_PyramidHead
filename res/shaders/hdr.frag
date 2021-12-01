#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;

void main()
{             
    const float gamma = 5;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
   
    if(hdr)
    {
        // reinhard
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }

    
    // hdrColor = texture(hdrBuffer, TexCoords).rgb;
    // // reinhard tone mapping
    // vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // // gamma correction 
    // mapped = pow(mapped, vec3(1.0 / gamma));
    // FragColor = vec4(mapped, 1.0);

    hdrColor = texture(hdrBuffer, TexCoords).rgb;
    FragColor = vec4(hdrColor, 1.0);
}