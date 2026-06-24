// copyright 2026 swaroop.
#pragma once

#ifndef RENDER_SETUP_H
#define RENDER_SETUP_H

#include "program_args.h"
#include "GLFW/glfw3.h"
#include "render_data.h"

#define NUM_WINDOWS 4
#define NUM_SHADERS 4
#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480



typedef struct {
    GLFWwindow* win;
    Quad quad; // main window quad

    int winHeight;
    int winWidth;
    int renderHeight;
    int renderWidth;

    // raw shader strings will be freed once all shaders are compiled
    char *rawVertShaders[NUM_SHADERS];
    char *rawFragShaders[NUM_SHADERS];
    GLuint compiledVertShaders[NUM_SHADERS];
    GLuint compiledFragShaders[NUM_SHADERS];
    GLuint fbos[NUM_SHADERS];
    GLuint fullTex[NUM_SHADERS];
    /*
     * preview textures are rendered at an effective resolution
     * to reduce unnecessary resource usage
     */
    GLuint previewTex[NUM_SHADERS];
    
    
    GLuint compositeProg;
} RenderContext;


void setupRenderContext(RenderContext *ctx, ProgArgs *args);


#endif //RENDER_SETUP_H