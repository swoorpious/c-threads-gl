// copyright 2026 swaroop.
#include "core.h"
#include "glad_impl.h"
#include "stdatomic.h"
#include "render_setup.h"

#include "shaders/composite_shader_impl.h"
#include "shaders/crude_shader_impl.h"

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

static void framebufferSizeCallback(GLFWwindow* win, int width, int height) {
    RenderContext* ctx = (RenderContext*)glfwGetWindowUserPointer(win);
    atomic_store(&ctx->winWidth, width);
    atomic_store(&ctx->winHeight, height);
    atomic_store(&ctx->isResized, 1);
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
    int fbw, fbh;
    glfwGetFramebufferSize(ctx->win, &fbw, &fbh);
    atomic_store(&ctx->winWidth, fbw);
    atomic_store(&ctx->winHeight, fbh);
    setupQuad(&ctx->quad);
    glfwSetWindowUserPointer(ctx->win, ctx);
    glfwSetFramebufferSizeCallback(ctx->win, framebufferSizeCallback);
    glfwSetWindowSizeLimits(ctx->win, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);

#ifndef BUILD_PROFILE_DEBUG
    // glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
#endif

    // keeping context current on main thread
    //
    // main thread must make current context null before render thread makes it current
}


void addShader(
    RenderContext* ctx, const char* path,
    ShaderRole role,
    const FuncSetupInitParams setupInit,
    const FuncSetupDispatchParams setupDispatch,
    void* initParams, void* dispatchParams) {
    
    int i = ctx->shaderCount++;
    ShaderEntry *curr = &ctx->shaderGroups[i];
    curr->shaderPath = path;
    curr->setupInitParams = setupInit ? setupInit : &defaultSetupInitParams;
    curr->setupDispatchParams = setupDispatch ? setupDispatch : &defaultSetupDispatchParams;
    curr->initParams = initParams ? initParams : NULL;
    curr->dispatchParams = dispatchParams ? dispatchParams : NULL;

    char* p = getPathTo(path);
    loadShader(p, curr);
    free(p);

    GLuint *prog = &curr->prog;
    *prog = createProgram(curr);
    int h = ctx->renderHeight, w = ctx->renderWidth;

    if (role == ShaderRoleComposite) {
        ctx->compositeIndex = i;
        h = atomic_load(&ctx->winHeight);
        w = atomic_load(&ctx->winWidth);
    }

    curr->setupInitParams(*prog, h, w, initParams);
}


void setupRenderContext(RenderContext* ctx, ProgArgs* args) {
    ctx->renderWidth = args->width; 
    ctx->renderHeight = args->height;
    ctx->isResized = -1;

    setupWindow(ctx);
    
    glGenFramebuffers(NUM_SHADERS, ctx->fbos);
    glGenTextures(NUM_SHADERS, ctx->fullTex);
    for (int i = 0; i < NUM_SHADERS; i++) {
        glBindTexture(GL_TEXTURE_2D, ctx->fullTex[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ctx->renderWidth, ctx->renderHeight,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, ctx->fbos[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                ctx->fullTex[i], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            fprintf(stderr, "FBO %d incomplete\n", i);

    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    CrudeShaderParams *params_crude = calloc(1, sizeof(CrudeShaderParams));
    params_crude->texPath = getPathTo("textures/clay_roof_tiles_02_diff_1k.jpg");
    
    addShader(ctx, "shaders/BalatroBackground.shader",
        ShaderRolePattern,
        NULL, NULL, NULL, NULL);
    addShader(ctx, "shaders/Crude.shader",
        ShaderRolePattern,
        crudeSetupInitParams,
        crudeSetupDispatchParams,
        params_crude,
        params_crude); // using the same params here cuz im lazy
    addShader(ctx, "shaders/AbstractVortex.shader",
        ShaderRolePattern,
        NULL, NULL, NULL, NULL);
    addShader(ctx, "shaders/fract113.shader",
        ShaderRolePattern,
        NULL, NULL, NULL, NULL);

    CompositeShaderParams *params_composite = malloc(sizeof(CompositeShaderParams));
    for (int i = 0; i < 4; i++) // bind texture ids
        params_composite->tex[i] = ctx->fullTex[i];

    addShader(ctx, "shaders/composite.shader",
        ShaderRoleComposite,
        compositeSetupInitParams,
        compositeSetupDispatchParams,
        NULL,
        params_composite);
}

static void drawQuad(Quad* quad) {
    glBindVertexArray(quad->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void renderShaderProgram(RenderContext* ctx, const int i, const double deltaTime) {
    const GLuint currProg = ctx->shaderGroups[i].prog;
    const ShaderEntry* currGrp = &ctx->shaderGroups[i];

    glBindFramebuffer(GL_FRAMEBUFFER, ctx->fbos[i]);
    glViewport(0, 0, ctx->renderWidth, ctx->renderHeight);
    glUseProgram(currProg);
    currGrp->setupDispatchParams(currProg, deltaTime, currGrp->dispatchParams);
    drawQuad(&ctx->quad);
    glFinish();
}

void renderCompositeProgram(RenderContext* ctx) {
    const int i = ctx->compositeIndex;
    const GLuint currProg = ctx->shaderGroups[i].prog;
    const ShaderEntry* currGrp = &ctx->shaderGroups[i];

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, ctx->winWidth, ctx->winHeight);
    glUseProgram(currProg);
    currGrp->setupDispatchParams(currProg, 0.f, currGrp->dispatchParams);
    drawQuad(&ctx->quad);
    glFinish();
}
