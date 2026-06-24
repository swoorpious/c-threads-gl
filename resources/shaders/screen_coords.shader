#shader VERTEX
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}


#SHADER FRAGMENT
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform float uTime;
void main() {
    FragColor = vec4(0.0, TexCoord.y, sin(uTime) * 0.5 + 0.5, 1.0);
}