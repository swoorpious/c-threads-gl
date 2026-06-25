#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"

static inline int clamp(const int x, const int lower, const int upper)
{
    return x < lower ? lower : x > upper ? upper : x;
}

static char* getPathTo(const char* path) {
    size_t len = strlen(RESOURCE_PATH) + strlen(path) + 1;

    char* result = malloc(len);
    if (!result)
        return NULL;

    snprintf(result, len, "%s%s", RESOURCE_PATH, path);
    return result;
}