#version 330 core

noperspective in vec2 vTexCoord;

uniform sampler2D uTexMap;

out vec4 f_color;

void main() {
    f_color = texture(uTexMap, vTexCoord);
    
}
