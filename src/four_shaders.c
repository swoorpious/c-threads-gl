// copyright 2026 swaroop.
#include "core.h"
#include "four_shaders/render/render_setup.h"
#include <getopt.h>
#include "thread_pool.h"


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char* argv[]) {
    printf("running executable: %s\n\n", argv[0]);

    if (!glfwInit()) { fprintf(stderr, "failed to init GLFW\n"); exit(EXIT_FAILURE); }

    ProgArgs conf;
    parseArgs(&conf, argc, argv);
    
    // glfwMakeContextCurrent(NULL);
    RenderContext ctx;
    setupRenderContext(&ctx, &conf);
    setupThreads(&ctx, &conf);

    glfwSetKeyCallback(ctx.win, keyCallback);

    // poll for other window related events
    while (!glfwWindowShouldClose(ctx.win)) glfwWaitEvents();
    
    glfwTerminate();
    
}


void parseArgs(ProgArgs *args, int argc, char *argv[]) {
    static struct option long_options[] = {
        {"width", required_argument, 0, 'w'},
        {"height", required_argument, 0, 'h'},
        {"render_mode", required_argument, 0, 'r'},
        {"fps", required_argument, 0, 'f'},
        {"duration", required_argument, 0, 'd'},
        {"frames", required_argument, 0, 'n'},
        // shorthands
        {"tf", required_argument, 0, 'n'},
        {"td", required_argument, 0, 'd'},
        {0, 0, 0, 0}
    };

    // defaults
    args->width = 640;
    args->height = 480;
    args->targetFps = 60;
    args->targetDuration = 10;
    args->targetFrames = 600;
    args->targetRenderMode = SetDurationAtSetFPS;

    int opt;
    while ((opt = getopt_long(argc, argv, "w:h:r:f:d:n:", long_options, NULL)) != -1) {
        switch (opt) {
        case 'w': args->width = clamp(atoi(optarg), 640, 7680);  break;
        case 'h': args->height = clamp(atoi(optarg), 480, 4320);  break;
        case 'f': args->targetFps = clamp(atoi(optarg), 1, 240);   break;
        case 'd': args->targetDuration = clamp(atoi(optarg), 1, 3600);  break;
        case 'n': args->targetFrames = clamp(atoi(optarg), 1, 216000);break;
        case 'r':
            if (atoi(optarg) == 0) args->targetRenderMode = SetDurationAtSetFPS;
            else if (atoi(optarg) == 1) args->targetRenderMode = SetFramesAtSetFPS;
            else fprintf(stderr, "unknown render mode: %s\n", optarg);
            break;
        case '?':
            fprintf(stderr, "unknown option, usage:\n"
                "  --width/-w      window width  (640-7680)\n"
                "  --height/-h     window height (480-4320)\n"
                "  --fps/-f        target fps    (1-240)\n"
                "  --duration/-d   duration secs (1-3600)\n"
                "  --frames/-n     target frames (1-216000)\n"
                "  --render_mode/-r 0=duration 1=frames\n"
                "  shorthands: --tf=frames --td=duration\n");
            break;
        default: break;
        }
    }

    // derive missing values based on render mode
    if (args->targetRenderMode == SetDurationAtSetFPS)
        args->targetFrames = args->targetDuration * args->targetFps;
    else
        args->targetDuration = args->targetFrames / args->targetFps;
}