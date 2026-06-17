// copyright 2026 swaroop.
#include "core.h"
#include "render_core.h"
#include "render_setup.h"
#include <stddef.h>

void setupWindows(WindowData* win_data) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

#ifndef BUILD_PROFILE_DEBUG
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
#endif

    for (int i = 0; i < NUM_WINDOWS; i++) {
        char title[32];
        snprintf(title, sizeof(title), "Shader %02d", i + 1);

        GLFWwindow *shared = (i == 0) ? NULL : win_data->windows[0];
        win_data->windows[i] = glfwCreateWindow(win_data->width, win_data->height, title, NULL, shared);
        if (!win_data->windows[i]) {
            fprintf(stderr, "failed to open GLFW window.\n");
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

            
        glfwMakeContextCurrent(win_data->windows[i]);
        gladLoadGL((GLADloadfunc)glfwGetProcAddress);
        setupQuad(&win_data->quads[i]);
    }
    
    glfwMakeContextCurrent(NULL);
}


void setupQuad(Quad *quad)
{
    glGenVertexArrays(1, &quad->vao);
    glGenBuffers(1, &quad->vbo);
    glGenBuffers(1, &quad->ebo);

    glBindVertexArray(quad->vao);

    glBindBuffer(GL_ARRAY_BUFFER, quad->vbo);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); // was missing
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
