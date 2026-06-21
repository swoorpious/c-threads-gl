// copyright 2026 swaroop.
#include "render_data.h"

float vertices[] = {
    -1.0f, 1.0f, 0.0f, 0.0f, // top-left
     1.0f, 1.0f, 1.0f, 0.0f, // top-right
    1.0f, -1.0f, 1.0f, 1.0f, // bottom-right
    -1.0f, -1.0f, 0.0f, 1.0f, // bottom-left
};

unsigned int indices[] = {
    0, 1, 2, // first triangle
    2, 3, 0  // second triangle
};