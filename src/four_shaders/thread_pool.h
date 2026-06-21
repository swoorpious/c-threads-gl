// copyright 2026 swaroop.
#pragma once

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

// #include "core.h"
#include "render/render_setup.h"
#include "pthread.h"
#include "stdatomic.h"
#include "program_args.h"

/*
 * setupThreads(...) will add references to each win
 */
typedef struct {
    WindowData *winData;
    pthread_t thread;
    pthread_mutex_t render_mutex;
    pthread_cond_t start_cond;
    pthread_cond_t done_cond;
    int start_flagged;
    int done_flagged;
    int should_close;
} RenderThread;

typedef struct {
    RenderThread render_thread[NUM_WINDOWS];
    /*
     *  adds up to num_threads whenever RenderThread should close and join back
     *  whenever a RenderThread is done rendering on its thread, it closes its window & frees memory
     */
    atomic_int all_should_close;
} RenderThreadGroup;

typedef struct {
    ProgArgs *progArgs;
    RenderThreadGroup *rtGroup;
} RendererContextGroup;

// thread workers
void *rendererWorker(void *arg);
void *renderThreadWorker(void *arg);

void setupThreads(RenderThreadGroup *threadGroup, WindowView *winView);
void dispatchThreads(RenderThreadGroup *threadGroup);
void waitThreads(RenderThreadGroup *threadGroup);
void destroyThreads(RenderThreadGroup *threadGroup);

#endif //THREAD_POOL_H