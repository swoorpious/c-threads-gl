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

    printf("[compositeSetupInitParams] setting up init params for composite\n");

    GLint loc0 = glGetUniformLocation(prog, "uTex0");
    GLint loc1 = glGetUniformLocation(prog, "uTex1");
    GLint loc2 = glGetUniformLocation(prog, "uTex2");
    GLint loc3 = glGetUniformLocation(prog, "uTex3");
    printf("uTex locations: %d %d %d %d\n", loc0, loc1, loc2, loc3);

    
    glUniform1i(loc0, 0);
    glUniform1i(loc1, 1);
    glUniform1i(loc2, 2);
    glUniform1i(loc3, 3);

}

static void compositeSetupDispatchParams (
    unsigned int prog,
    double deltaTime,
    void *params
    ) {
    const CompositeShaderParams *p = (CompositeShaderParams *)params;
    glUseProgram(prog);
    printf("composite tex ids: %u %u %u %u\n", p->tex[0], p->tex[1], p->tex[2], p->tex[3]);

    for (int i = 0; i < 4; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, p->tex[i]);
    }
}

#endif // COMPOSITE_SHADER_H
