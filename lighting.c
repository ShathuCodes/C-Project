#include "../include/lighting.h"
#include <math.h>

// Compute Lambert intensity based on dot product
float compute_lambert(vec3_t edge_dir, vec3_t light_dir) {
    vec3_normalize_fast(&edge_dir);
    vec3_normalize_fast(&light_dir);
    float dot = edge_dir.x * light_dir.x + edge_dir.y * light_dir.y + edge_dir.z * light_dir.z;
    return fmaxf(0.0f, dot);
}

