// copyright 2026 swaroop.
#pragma once

#ifndef RENDER_SETUP_H
#define RENDER_SETUP_H

#include "GLFW/glfw3.h"
#include "render_data.h"

#define NUM_WINDOWS 4
#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480


typedef struct {
    GLFWwindow* win;
    Quad quad;

} WindowData;

typedef struct {
    WindowData windows[NUM_WINDOWS];
    int height;
    int width;
} WindowView;


void setupWindows(WindowView* win_data);
void setupQuad(Quad *quad);

void startRender(WindowView* win_data);

void dispatchRenderThreads();
void *renderThread(void *data);



#endif //RENDER_SETUP_H