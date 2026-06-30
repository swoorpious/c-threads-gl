// copyright 2026 swaroop.
#pragma once

#ifndef COMPOSITE_SHADER_H
#define COMPOSITE_SHADER_H
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/gl.h"
#include "core.h"
#include <render/shader_utils.h>


typedef struct {
    GLuint tex[4];
} CompositeShaderParams;

static void compositeSetupInitParams (
    unsigned int prog,
    int height,
    int width,
    void *params
    ) {
    glUseProgram(prog);
    glUniform2f(
        glGetUniformLocation(prog, "uResolution"),
        (float)width, (float)height);

    glUniform1i(glGetUniformLocation(prog, "uTex0"), 0);
    glUniform1i(glGetUniformLocation(prog, "uTex1"), 1);
    glUniform1i(glGetUniformLocation(prog, "uTex2"), 2);
    glUniform1i(glGetUniformLocation(prog, "uTex3"), 3);

}

static void compositeSetupDispatchParams (
    unsigned int prog,
    double deltaTime,
    void *params
    ) {
    const CompositeShaderParams *p = (CompositeShaderParams *)params;
    glUseProgram(prog);

    for (int i = 0; i < 4; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, p->tex[i]);
    }
}

#endif // COMPOSITE_SHADER_H
