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
    GLFWwindow* windows[NUM_WINDOWS];
    Quad quads[NUM_WINDOWS];

    int height;
    int width;
} WindowData;

int setupWindows(WindowData* win_data);
void setupQuad(Quad *quad);


#endif //RENDER_SETUP_H