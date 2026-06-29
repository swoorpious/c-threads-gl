// copyright 2026 swaroop.

#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

// #include "GLFW/glfw3.h"
typedef void (*FuncSetupInitParams)(
        unsigned int prog, // takes gl program from the RenderContext
        int height,
        int width,
        void *params
        );
typedef void (*FuncSetupDispatchParams)(
        unsigned int prog, // takes gl program from the RenderContext
        double deltaTime,
        void *params
        );
typedef struct {
    const char *shaderPath;

    FuncSetupInitParams setupInitParams;
    FuncSetupDispatchParams setupDispatchParams;
    /*
     * struct for custom shader data
     * must be implemented in each shaders dispatch function
     * otherwise have pass in NULL for defaultDispatch method
     */
    void *initParams;
    void *dispatchParams;

    // buffer location
    int uTime;
    int uResolution;

    char *raw;
    char *vert;
    char *frag;

    unsigned int compiledVert;
    unsigned int compiledFrag;
} ShaderGroup;

void defaultSetupInitParams (
    unsigned int prog,
    int height,
    int width,
    void *params
    );
void defaultSetupDispatchParams (
    unsigned int prog,
    double deltaTime,
    void *params
    );

void loadShader(const char *shaderPath, ShaderGroup *grp);
unsigned int createProgram(const ShaderGroup *grp);
// void compileShaderGroup(ShaderGroup *grp);


#endif // SHADER_UTILS_H