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

#define AA 4.
#define PI 3.141592653589793

vec2 CRTCurveUV(vec2 uv) {
    uv = uv * 2.0 - 1.0;
    vec2 offset = abs(uv.yx) / vec2(6.0, 4.0);
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}

void DrawVignette(inout vec3 color, vec2 uv) {
    float vignette = uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y);
    vignette = clamp(pow(16.0 * vignette, 0.3), 0.0, 1.0);
    color *= vignette;
}

void DrawScanline(inout vec3 color, vec2 uv) {
    float scanline = clamp(0.95 + 0.05 * cos(3.14 * (uv.y + 0.008 * uTime) * 240.0), 0.0, 1.0);
    float grille   = 0.85 + 0.15 * clamp(1.5 * cos(3.14 * uv.x * 640.0), 0.0, 1.0);
    color *= scanline * grille * 1.2;
}

float atanp(in vec2 p) { return atan(p.y, p.x); }
float cube_root(float x) { return sign(x) * pow(abs(x), 1.0 / 3.0); }
float sq(float x) { return x * x; }

vec3 margarita(in vec2 p) {
    float z = length(p) - 3.5 * atanp(p) + sin(p.x) + cos(p.y);
    if (mod(z, 7.0 * PI) < PI / 2.0) return vec3(1, 0, 0);
    if (mod(z, 1.0 * PI) < PI / 2.0) return vec3(0);
    return vec3(1);
}

vec3 digital_bacteria(in vec2 p) {
    p /= 4.0;
    float x = sq(sin(p.x) + p.y)       + sq(cos(p.y) + p.x);
    float y = cos(10.0 * p.x) + cos(10.0 * p.y) - sin(p.x * p.y);
    float z = sq(sin(floor(p.x)) + floor(p.y)) + sq(cos(floor(p.y)) + floor(p.x));
    if (17.0 < x && x < 21.0 && 17.0 < z && z < 21.0 && y < 0.0)
        return vec3(1.0, 1.0, 85.0 / 256.0);
    if (17.0 < z && z < 21.0) return vec3(85.0 / 256.0, 0.0, 0.0);
    if (17.0 < x && x < 21.0) return vec3(170.0 / 256.0, 170.0 / 256.0, 0.0);
    return vec3(85.0 / 256.0, 85.0 / 256.0, 0.0);
}

vec3 threesome(in vec2 p) {
    p /= 3.0;
    float z = 1.0;
    z *= sin(length(p + vec2(5, 0)))  * cos(8.0 * atanp(p + vec2(5, 0)));
    z *= sin(length(p - vec2(5, 5)))  * cos(8.0 * atanp(p - vec2(5, 5)));
    z *= sin(length(p + vec2(0, 5)))  * cos(8.0 * atanp(p + vec2(0, 5)));
    if ((-0.1 < z && z < 0.0) || 0.2 < z) return vec3(0);
    return vec3(1);
}

vec3 plaid_meltdown(in vec2 p) {
    p /= 15.0;
    p += 7.0;
    float a = 2.0 * sin(p.x * sin(p.y) + p.y * sin(p.x));
    float b = cube_root(sin(2.5 * sqrt(2.0) * (p.x - p.y)));
    float c = cube_root(sin(2.5 * sqrt(2.0) * (p.x + p.y)));
    float d = sin(80.0 * p.x) + sin(80.0 * p.y);
    if (0.25 * (a + b + c) > 0.5 * d) return vec3(0);
    return vec3(1);
}

vec3 sunlight_revealed(in vec2 p) {
    p /= 6.0;
    p.x += 2.0;
    float a = length(vec2(3.0 - p.x, p.y)) + abs(p.y) + abs(1.0 - p.x);
    float f = atan(p.y, p.x - 1.0);
    float c = atan(p.y, p.x - 3.0);
    float R = sq(p.x - 1.0) + sq(p.y);
    vec3 col = vec3(0);
    bool mix = false;
    if (5.0 < a && a < 7.0 && mod(f, PI / 7.0) < PI / 14.0) {
        col += vec3(0.0, 82.0 / 256.0, 173.0 / 256.0);
        if (mix) col /= 2.0;
        mix = true;
    }
    if (5.0 < a && a < 7.0 && mod(c, PI / 9.0) < PI / 18.0) {
        col += vec3(1, 0, 0);
        if (mix) col /= 2.0;
        mix = true;
    }
    if (5.0 < a && a < 7.0 && mod(f, PI / 8.0) < PI / 16.0) {
        col += vec3(1, 1, 0);
        if (mix) col /= 2.0;
        mix = true;
    }
    if ((45.0 - 3.0 * p.x) * PI / 180.0 < f && f < (47.0 - p.x) * PI / 180.0
        && p.y > 0.1 * p.x && mod(log(R) / log(f), 2.0) < 1.0) {
        col += vec3(1);
        if (mix) col /= 2.0;
    }
    return col;
}

void main() {
    // fragCoord equivalent: pixel position from gl_FragCoord
    vec2 fragCoord = gl_FragCoord.xy;

    float t = mod(uTime, 10.0);
    vec3 color = vec3(0);

    for (float i = 0.0; i < AA * AA - 0.5; i += 1.0) {
        vec2 uv = (fragCoord + vec2(floor(i / AA), mod(i, AA)) / AA) / uResolution.xy;
        vec2 crtUV = CRTCurveUV(uv);
        if (crtUV.x < 0.0 || crtUV.x > 1.0 || crtUV.y < 0.0 || crtUV.y > 1.0) continue;

        vec2 p = 50.0 * crtUV - 25.0;
        p *= 0.75 + 0.05 * mod(uTime, 10.0);
        p += mod(uTime, 10.0) - 5.0;
        p.x *= uResolution.x / uResolution.y;

        if (t < 2.0 || 8.0 < t) {
            float fade  = smoothstep(0.0, 2.0, t) - smoothstep(8.0, 10.0, t);
            float scale = uResolution.y / 50.0 * float(AA) * fade + 1.0;
            p = floor(p.xy * scale) / scale;
        }

        vec3 c;
        if      (mod(0.1 * uTime, 5.0) < 1.0) c = margarita(p);
        else if (mod(0.1 * uTime, 5.0) < 2.0) c = plaid_meltdown(p);
        else if (mod(0.1 * uTime, 5.0) < 3.0) c = sunlight_revealed(p);
        else if (mod(0.1 * uTime, 5.0) < 4.0) c = threesome(p);
        else                                   c = digital_bacteria(p);

        DrawVignette(c, crtUV);
        DrawScanline(c, uv);
        color += c / float(AA * AA);
    }

    fragColor = vec4(color, 1.0);
}
