// copyright 2026 swaroop.
#include "core.h"
#include "glad_impl.h"
#include "render_setup.h"

static void setupQuad(Quad* quad) {
    glGenVertexArrays(1, &quad->vao);
    glGenBuffers(1, &quad->vbo);
    glGenBuffers(1, &quad->ebo);

    glBindVertexArray(quad->vao);

    glBindBuffer(GL_ARRAY_BUFFER, quad->vbo);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); // was missing
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}


static void setupWindow(RenderContext* ctx) {
    GLFWmonitor* mon = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(mon);
    int screenRes[2] = {mode->width, mode->height};

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    /*
     * - set window maximized if requested render height for a single shader is
     * larger than the screen size.
     *
     * - if not we just use 1280x720
     * - values cannot be zero
     */
    int w = ctx->renderWidth ? ctx->renderWidth : 1280;
    int h = ctx->renderHeight ? ctx->renderHeight : 720;
    if (screenRes[0] < ctx->renderWidth && screenRes[1] < ctx->renderHeight) {
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        w = screenRes[0] > 1280 ? 1280 : 800;
        h = screenRes[1] > 720 ? 720 : 600;
    }

    ctx->win = glfwCreateWindow(w, h, "4shaders", NULL, NULL);
    if (!ctx->win) {
        fprintf(stderr, "failed to open GLFW window.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(ctx->win);
    gladLoadGL((GLADloadfunc)glfwGetProcAddress);
    setupQuad(&ctx->quad);

#ifndef BUILD_PROFILE_DEBUG
    // glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
#endif

    // we need to make current context null before the render thread sets it
    glfwMakeContextCurrent(NULL);
}

static char* getPathTo(const char* path) {
    size_t len = strlen(RESOURCE_PATH) + strlen(path) + 1;

    char* result = malloc(len);
    if (!result)
        return NULL;

    snprintf(result, len, "%s%s", RESOURCE_PATH, path);
    return result;
}

void setupRenderContext(RenderContext* ctx, ProgArgs* args) {
    ctx->renderWidth = args->width;
    ctx->renderHeight = args->height;

    setupWindow(ctx);

    char* p = getPathTo("shaders/screen_coords.shader");
    loadShader(p, &ctx->shaderGroups[0]);
    free(p);

    ctx->glProgs[0] = createProgram(&ctx->shaderGroups[0]);

    /*
     * add shaders etc 
     */
}

static void drawQuad(Quad* quad) {
    glBindVertexArray(quad->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void renderProgram(RenderContext* ctx, const int index) {
    glUseProgram(ctx->glProgs[index]);
    drawQuad(&ctx->quad);
}
