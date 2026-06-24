// copyright 2026 swaroop.
#include "thread_pool.h"

#include <math.h>

#include "core.h"
#include <GLFW/glfw3.h>
#include <time.h>


typedef struct {
    int renderedFrames[NUM_SHADERS];
    // long double renderedDurations[NUM_SHADERS];

    /*
     * this number would be same for all the shaders
     * calculated in the rendererWorker
     */
    int targetFramesToRender;

    /*
     * tracks the number of shaders completed rendering.
     * adds up to NUM_SHADERS.
     * if equal to NUM_SHADERS, program should close.
     */
    int rendersCompleted;
} RenderProgress;

void* rendererWorker(void* arg) {
    RendererContextGroup* rcg = (RendererContextGroup*)arg;
    RenderContext* ctx = rcg->ctx;
    RenderProgress progress = {0};
    const ProgArgs* conf = rcg->progArgs;

    free(rcg);

    // also target deltaTime
    const double targetFrametime = 1000.0 / (double)conf->targetFps;
    double lastFrameTime = glfwGetTime() * 1000.0;

    // calc targetFramesToRender
    if (conf->targetRenderMode == SetDurationAtSetFPS) {
        const int frames = conf->targetDuration * conf->targetFps;
        progress.targetFramesToRender = frames;
    }
    else progress.targetFramesToRender = conf->targetFrames;


    // wait at most 8ms, or less
    // wait time for event polling
    const double waitTime = (__min(targetFrametime, 8.0)) / 1000.f;
    glfwWaitEventsTimeout(waitTime);

    // render loop
    glfwMakeContextCurrent(ctx->win);
    while (progress.rendersCompleted < NUM_SHADERS) {
        if (glfwWindowShouldClose(ctx->win)) break;
        // obtained deltaTime
        const double now = glfwGetTime() * 1000.0;
        if (now - lastFrameTime < targetFrametime) continue;
        lastFrameTime = now;

        glClearColor(0.f, 0.0f, 0.f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        RenderProgress* p = &progress;
        for (int i = 0; i < 1 /* NUM_SHADERS */; i++) {
            if (p->renderedFrames[i] < p->targetFramesToRender) {
                int nextFrame = ++p->targetFramesToRender;
                // renderFrame(nextFrame);
                renderProgram(ctx, i);
            }
        }

        glfwSwapBuffers(ctx->win);

        // dispatchThreads(rtg);
        // waitThreads(rtg);
        glfwWaitEventsTimeout(waitTime);
    }
    // glfwMakeContextCurrent(NULL);

    // destroyThreads(rtg);
    return NULL;
}

void setupThreads(RenderContext* ctx, ProgArgs* conf) {
    RendererContextGroup *grp = malloc(sizeof(RendererContextGroup));
    grp->ctx = ctx;
    grp->progArgs = conf;
    pthread_create(&ctx->renderThread, NULL, rendererWorker, grp);

}

void closeThreads(RenderContext* ctx) {
    pthread_join(ctx->renderThread, NULL);
}

// void* renderThreadWorker(void* arg) {
//     RenderThread* rt = (RenderThread*)arg;
//     unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
//     srand(seed);
//
//     glfwMakeContextCurrent(rt->winData->win);
//
//     while (!rt->should_close) {
//         int x = rand() % 100;
//
//         pthread_mutex_lock(&rt->render_mutex);
//         while (!rt->start_flagged)
//             pthread_cond_wait(&rt->start_cond, &rt->render_mutex);
//
//         rt->start_flagged = 0;
//         pthread_mutex_unlock(&rt->render_mutex);
//
//         /*
//          * add check for frames/render length to set should_close to true
//          */
//         if (/**/ 0) break;
//
//
//
//         pthread_mutex_lock(&rt->render_mutex);
//         rt->done_flagged = 1;
//         pthread_cond_signal(&rt->done_cond);
//         pthread_mutex_unlock(&rt->render_mutex);
//     }
//
//     return NULL;
// }


// void setupThreads(RenderThreadGroup* threadGroup, WindowView* winView) {
//     for (int i = 0; i < NUM_WINDOWS; i++) {
//         RenderThread* rt = &threadGroup->render_thread[i];
//         rt->winData = &winView->windows[i];
//         rt->start_flagged = 0;
//         rt->done_flagged = 0;
//         rt->start_flagged = 0;
//
//         pthread_mutex_init(&rt->render_mutex, NULL);
//         pthread_cond_init(&rt->start_cond, NULL);
//         pthread_cond_init(&rt->done_cond, NULL);
//
//         glfwMakeContextCurrent(NULL);
//         pthread_create(&rt->thread, NULL, renderThreadWorker, rt);
//     }
// }

/*
void dispatchThreads(RenderThreadGroup* threadGroup) {
    for (int i = 0; i < NUM_WINDOWS; i++) {
        RenderThread* rt = &threadGroup->render_thread[i];
        pthread_mutex_lock(&rt->render_mutex);
        rt->start_flagged = 1;
        pthread_cond_signal(&rt->start_cond);
        pthread_mutex_unlock(&rt->render_mutex);
    }
}

void waitThreads(RenderThreadGroup* threadGroup) {
    for (int i = 0; i < NUM_WINDOWS; i++) {
        RenderThread* rt = &threadGroup->render_thread[i];
        pthread_mutex_lock(&rt->render_mutex);
        while (!rt->done_flagged)
            pthread_cond_wait(&rt->done_cond, &rt->render_mutex);

        rt->done_flagged = 0;
        pthread_mutex_unlock(&rt->render_mutex);
    }
}

void destroyThreads(RenderThreadGroup* threadGroup) {
    for (int i = 0; i < NUM_WINDOWS; i++) {
        RenderThread* rt = &threadGroup->render_thread[i];
        pthread_join(rt->thread, NULL);
        pthread_mutex_destroy(&rt->render_mutex);
        pthread_cond_destroy(&rt->start_cond);
        pthread_cond_destroy(&rt->done_cond);
    }
}
*/
