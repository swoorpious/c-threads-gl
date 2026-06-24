// copyright 2026 swaroop.

#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

// #include "GLFW/glfw3.h"

typedef struct {
    char *raw;
    char *vert;
    char *frag;

    unsigned int compiledVert;
    unsigned int compiledFrag;
} ShaderGroup;


void loadShader(const char *shaderPath, ShaderGroup *grp);
unsigned int createProgram(const ShaderGroup *grp);
// void compileShaderGroup(ShaderGroup *grp);


#endif // SHADER_UTILS_H