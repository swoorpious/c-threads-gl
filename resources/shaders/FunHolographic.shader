// FRAGMENT SHADER CREDITS
// source: https://www.shadertoy.com/view/7c2XRt
// author: [Frostbyte_](https://www.shadertoy.com/user/Frostbyte_)

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

uniform float uTime;
uniform vec2 uResolution;

void main() {
    vec2 C = gl_FragCoord.xy;
    vec3 r = vec3(uResolution, 0.0);
    float i = 0.0, d = 0.0, z = 0.0;
    vec3 p;
    vec4 O = vec4(0.0);
    for (; i < 1e2; i += 1.0) {
        p = z * normalize(vec3(C.xy - 0.5 * r.xy, r.y));
        p = abs(fract(vec3(mat2(cos(cos(z * 0.5) + vec4(0, 11, 33, 0))) * p.xy * 2.0, p.z - uTime)) - 0.5) - 0.5;
        d = abs(length(max(p, 0.25)) + max(p.x, max(p.y, p.z * 3.0))) / 3.0 + abs(sin(z * 0.7 - uTime)) * 0.001;
        z += d * 0.8;
        O += 1.0 / d;
    }
    vec3 color = (p + 0.8) * z / 10;
    fragColor = vec4(color, 1.0);
}