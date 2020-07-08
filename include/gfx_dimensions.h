#pragma once

#include <math.h>
#include "pc/gfx/fast64.h"

#define GFX_DIMENSIONS_FROM_LEFT_EDGE(v) (SCREEN_WIDTH / 2 - SCREEN_HEIGHT / 2 * gfx_current_dimensions.aspect_ratio + (v))
#define GFX_DIMENSIONS_FROM_RIGHT_EDGE(v) (SCREEN_WIDTH / 2 + SCREEN_HEIGHT / 2 * gfx_current_dimensions.aspect_ratio - (v))
#define GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(v) ((int)floorf(GFX_DIMENSIONS_FROM_LEFT_EDGE(v)))
#define GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(v) ((int)ceilf(GFX_DIMENSIONS_FROM_RIGHT_EDGE(v)))
#define GFX_DIMENSIONS_ASPECT_RATIO (gfx_current_dimensions.aspect_ratio)

