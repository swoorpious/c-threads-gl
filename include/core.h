#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"

static inline int clamp(const int x, const int lower, const int upper)
{
    return x < lower ? lower : x > upper ? upper : x;
}
