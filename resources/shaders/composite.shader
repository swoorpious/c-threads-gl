// VERTEX/FRAGMENT SHADER CREDITS
// source: oooo it's me!! i am a noob but i wrote this!!
// author: [swaroop.](https://github.com/swoorpious)


#shader VERTEX
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}

#shader FRAGMENT
#version 330 core
in vec2 TexCoord;
out vec4 fragColor;

uniform sampler2D uTex0;
uniform sampler2D uTex1;
uniform sampler2D uTex2;
uniform sampler2D uTex3;

void main() {
    vec2 uv = TexCoord;
    vec2 cellUV = fract(uv * 2.0);
    
    vec4 color;
    if (uv.x < 0.5 && uv.y >= 0.5)
        color = texture(uTex0, cellUV);     // top-left
    else if (uv.x >= 0.5 && uv.y >= 0.5)
        color = texture(uTex1, cellUV);     // top-right
    else if (uv.x < 0.5 && uv.y < 0.5)
        color = texture(uTex2, cellUV);     // bottom-left
    else
        color = texture(uTex3, cellUV);     // bottom-right

    fragColor = color;
}
