// copyright 2026 swaroop.

#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

typedef enum {
    ShaderRolePattern = 0,
    ShaderRoleComposite = 1,
} ShaderRole;

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
    ShaderRole role;

    FuncSetupInitParams setupInitParams;
    FuncSetupDispatchParams setupDispatchParams;
    /*
     * struct for custom shader data
     * must be implemented in each shaders dispatch function
     * otherwise have pass in NULL for defaultDispatch method
     */
    void *initParams;
    void *dispatchParams;

    char *raw;
    char *vert;
    char *frag;
    unsigned int compiledVert;
    unsigned int compiledFrag;

    unsigned int prog;
} ShaderEntry;


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

void loadShader(const char *shaderPath, ShaderEntry *grp);
unsigned int createProgram(const ShaderEntry *grp);
// void compileShaderGroup(ShaderGroup *grp);


#endif // SHADER_UTILS_H