// copyright 2026 swaroop.
#include "thread_pool.h"

#include <math.h>

#include "core.h"
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <time.h>


void* rendererWorker(void *arg) {
    RendererContextGroup *rcg = (RendererContextGroup *)arg;
    RenderThreadGroup *rtg = rcg->rtGroup;   // no & needed, already a pointer
    ProgArgs *conf = rcg->progArgs;

    const double targetFrametime = 1000.0 / (double)conf->targetFps;
    double lastFrameTime = glfwGetTime() * 1000.0;

    // wait at least 8ms, at most one frame duration
    double waitTime = targetFrametime > 8.0 ? targetFrametime : 8.0;
    glfwWaitEventsTimeout(waitTime / 1000.0);

    while (atomic_load(&rtg->all_should_close) < NUM_WINDOWS) {
        glfwWaitEventsTimeout(waitTime / 1000.0);

        const double now = glfwGetTime() * 1000.0;
        if (now - lastFrameTime < targetFrametime) continue;
        lastFrameTime = now;

        dispatchThreads(rtg);
        waitThreads(rtg);
    }

    destroyThreads(rtg);
    return NULL;
}

void *renderThreadWorker(void *arg) {
    RenderThread *rt = (RenderThread *)arg;
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
    srand(seed);
    
    glfwMakeContextCurrent(rt->winData->win);
    
    while (!rt->should_close) {
        int x = rand() % 100;
        
        pthread_mutex_lock(&rt->render_mutex);
        while(!rt->start_flagged)
            pthread_cond_wait(&rt->start_cond, &rt->render_mutex);
        
        rt->start_flagged = 0;
        pthread_mutex_unlock(&rt->render_mutex);

        /*
         * add check for frames/render length to set should_close to true
         */
        if (/**/ 0) break;

        glClearColor((float)sin(x), 0.0f, (float)cos(x), 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(rt->winData->win);

        pthread_mutex_lock(&rt->render_mutex);
        rt->done_flagged = 1;
        pthread_cond_signal(&rt->done_cond);
        pthread_mutex_unlock(&rt->render_mutex);

    }

    return NULL;
}


void setupThreads(RenderThreadGroup* threadGroup, WindowView* winView) {
    for (int i = 0; i < NUM_WINDOWS; i++) {
        RenderThread *rt = &threadGroup->render_thread[i];
        rt->winData = &winView->windows[i];
        rt->start_flagged = 0;
        rt->done_flagged = 0;
        rt->start_flagged = 0;

        pthread_mutex_init(&rt->render_mutex, NULL);
        pthread_cond_init(&rt->start_cond, NULL);
        pthread_cond_init(&rt->done_cond, NULL);

        glfwMakeContextCurrent(NULL);
        pthread_create(&rt->thread, NULL, renderThreadWorker, rt);
    }
}

void dispatchThreads(RenderThreadGroup* threadGroup) {
    for (int i = 0; i < NUM_WINDOWS; i++) {
        RenderThread *rt = &threadGroup->render_thread[i];
        pthread_mutex_lock(&rt->render_mutex);
        rt->start_flagged = 1;
        pthread_cond_signal(&rt->start_cond);
        pthread_mutex_unlock(&rt->render_mutex);
    }
}

void waitThreads(RenderThreadGroup* threadGroup) {
    for (int i = 0; i < NUM_WINDOWS; i++) {
        RenderThread *rt = &threadGroup->render_thread[i];
        pthread_mutex_lock(&rt->render_mutex);
        while (!rt->done_flagged)
            pthread_cond_wait(&rt->done_cond, &rt->render_mutex);

        rt->done_flagged = 0;
        pthread_mutex_unlock(&rt->render_mutex);
     }
}

void destroyThreads(RenderThreadGroup* threadGroup) {
    for (int i = 0; i < NUM_WINDOWS; i++) {
        RenderThread *rt = &threadGroup->render_thread[i];
        pthread_join(rt->thread, NULL);
        pthread_mutex_destroy(&rt->render_mutex);
        pthread_cond_destroy(&rt->start_cond);
        pthread_cond_destroy(&rt->done_cond);
        
    }
}

