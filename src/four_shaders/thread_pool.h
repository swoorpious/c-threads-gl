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
    RenderContext *ctx;
    pthread_t thread;
    pthread_mutex_t render_mutex;
    pthread_cond_t start_cond;
    pthread_cond_t done_cond;
    int start_flagged;
    int done_flagged;
    int should_close;
} RenderThreadArgs;


typedef struct {
    ProgArgs *progArgs;
    RenderContext *ctx;
} RendererContextGroup;

// thread workers
/*
 * holds the render loop
 */
void *rendererWorker(void *arg);
void *renderThreadWorker(void *arg);

void setupThreads(RenderContext *ctx, ProgArgs *conf);
void closeThreads(RenderContext *ctx);
// void dispatchThreads(RenderThreadGroup *threadGroup);
// void waitThreads(RenderThreadGroup *threadGroup);
// void destroyThreads(RenderThreadGroup *threadGroup);

#endif //THREAD_POOL_H