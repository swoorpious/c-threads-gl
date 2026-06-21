// copyright 2026 swaroop.

#ifndef LEARNINGTHREADS_PROGRAM_ARGS_H
#define LEARNINGTHREADS_PROGRAM_ARGS_H

typedef enum {
    /*
     * default render mode
     * default duration: 10s
     * default fps: 60 (16.67ms)
     *
     * renders for 10s at 60fps
     */
    SetDurationAtSetFPS = 0,

    /*
     * default frames: 600
     * default fps: 60 (16.67ms)
     *
     * renders 600 frames at 60fps
     */
    SetFramesAtSetFPS = 1,
} ProgRenderMode;

typedef struct {
    int height;
    int width;
    int targetFps;
    int targetDuration;
    int targetFrames;
    ProgRenderMode targetRenderMode;
    
} ProgArgs;

void parseArgs(ProgArgs *args, int argc, char *argv[]);

#endif //LEARNINGTHREADS_PROGRAM_ARGS_H