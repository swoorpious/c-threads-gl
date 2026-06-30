// copyright 2026 swaroop.
#pragma once
#ifndef FRACTAL_ROOT_SHADER_H
#define FRACTAL_ROOT_SHADER_H
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/gl.h"
#include "core.h"
#include <render/shader_utils.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdio.h>

typedef struct {
    const char *texPath;
    GLuint tex;
    int loaded;
} CrudeShaderParams;


static GLuint loadTexture(const char *path) {
    int w, h, channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(path, &w, &h, &channels, 0);
    if (!data) {
        printf("[loadTexture] failed to load '%s': %s\n",
               path, stbi_failure_reason());
        return 0;
    }

    GLenum format = GL_RGB;
    if (channels == 1) format = GL_RED;
    else if (channels == 3) format = GL_RGB;
    else if (channels == 4) format = GL_RGBA;

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    printf("[loadTexture] loaded '%s' (%dx%d, %d ch) -> tex %u\n",
           path, w, h, channels, tex);

    return tex;
}

static void crudeSetupInitParams(
    unsigned int prog,
    int height,
    int width,
    void *params
    ) {
    glUseProgram(prog);
    glUniform2f(
        glGetUniformLocation(prog, "uResolution"),
        (float)width, (float)height);

    GLint loc0 = glGetUniformLocation(prog, "uTex0");
    printf("[crudeSetupInitParams] iChannel0 location: %d\n", loc0);
    glUniform1i(loc0, 0);

    CrudeShaderParams *p = (CrudeShaderParams *)params;
    if (p && !p->loaded) {
        p->tex = loadTexture(p->texPath);
        p->loaded = 1;
    }
}

static void crudeSetupDispatchParams(
    unsigned int prog,
    double deltaTime,
    void *params
    ) {
    static double elapsed = 0.0;
    elapsed += deltaTime;

    glUseProgram(prog);
    glUniform1f(glGetUniformLocation(prog, "uTime"), (float)elapsed);

    const CrudeShaderParams *p = (const CrudeShaderParams *)params;
    if (p && p->tex) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, p->tex);
    }
}

#endif // FRACTAL_ROOT_SHADER_H