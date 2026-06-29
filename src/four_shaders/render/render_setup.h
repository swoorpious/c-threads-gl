// copyright 2026 swaroop.
#pragma once

#ifndef RENDER_SETUP_H
#define RENDER_SETUP_H

#include "render/render_core.h"

#include "program_args.h"
#include <stdatomic.h>
#include "render_data.h"
#include "shader_utils.h"
#include "pthread.h"

#define NUM_WINDOWS 4
#define NUM_SHADERS 4
#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480


typedef struct {
    GLFWwindow* win;
    Quad quad; // main window quad
    pthread_t renderThread;

    _Atomic(int) winHeight;
    _Atomic(int) winWidth;
    int renderHeight;
    int renderWidth;
    _Atomic(int) isResized;

    // raw shader strings will be freed once all shaders are compiled
    ShaderGroup shaderGroups[NUM_SHADERS];
    GLuint glProgs[NUM_SHADERS];
    GLuint fbos[NUM_SHADERS];
    GLuint fullTex[NUM_SHADERS];
    /*
     * preview textures are rendered at an effective resolution
     * to reduce unnecessary resource usage
     */
    GLuint previewTex[NUM_SHADERS];
    
    GLuint compositeProg;
} RenderContext;


void addShader(
    RenderContext *ctx,
    const char *path,
    // function pointers
    FuncSetupInitParams setupInit,
    FuncSetupDispatchParams setupDispatch,
    void *initParams,
    void *dispatchParams
);
void setupRenderContext(RenderContext *ctx, ProgArgs *args);
void renderProgram(RenderContext *ctx, int index, double deltaTime);


#endif //RENDER_SETUP_H