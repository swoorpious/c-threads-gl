// copyright 2026 swaroop.
#include "core.h"
#include "GLFW/glfw3.h"
#include "render_setup.h"
#include <getopt.h>

void parseArgs(WindowData *win, int argc, char *argv[]) {
    static struct option long_options[] = {
        {"width",  required_argument, 0, 'w'},
        {"height", required_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "w:h:", long_options, NULL)) != -1) {
        switch (opt) {
        case 'w': win->width = clamp(atoi(optarg), 640, 7680); break;
        case 'h': win->height = clamp(atoi(optarg), 480, 4320); break;
        default: ;
        }
    }
}

int main(int argc, char* argv[]) {
    printf("running executable: %s\n\n", argv[0]);

    if (!glfwInit()) { fprintf(stderr, "failed to init GLFW\n"); exit(EXIT_FAILURE); }

    WindowData win_data;
    win_data.width = 640;
    win_data.height = 480;
    parseArgs(&win_data, argc, argv);
    setupWindows(&win_data);
    
    
}