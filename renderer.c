#include "../include/renderer.h"
#include "../include/lighting.h"
#include <math.h>

// Projects a 3D vector to screen space using MVP matrix
vec3_t project_vertex(vec3_t v, mat4_t mvp, int width, int height) {
    vec3_t transformed = mat4_transform(mvp, v);
    transformed.x /= transformed.z;
    transformed.y /= transformed.z;

    // Map to viewport
    transformed.x = (transformed.x + 1.0f) * 0.5f * width;
    transformed.y = (transformed.y + 1.0f) * 0.5f * height;
    return transformed;
}

// Clipping to circular viewport
bool clip_to_circular_viewport(canvas_t* canvas, float x, float y) {
    float cx = canvas->width / 2.0f;
    float cy = canvas->height / 2.0f;
    float r = (canvas->width < canvas->height ? canvas->width : canvas->height) / 2.0f;

    float dx = x - cx;
    float dy = y - cy;

    return dx * dx + dy * dy <= r * r;
}

// Renders a wireframe using edge list and lighting
void render_wireframe(canvas_t* canvas, vec3_t* vertices, int* edges, int edge_count, mat4_t mvp) {
    for (int i = 0; i < edge_count; i += 2) {
        vec3_t v0_world = vertices[edges[i]];
        vec3_t v1_world = vertices[edges[i + 1]];

        vec3_t v0_proj = project_vertex(v0_world, mvp, canvas->width, canvas->height);
        vec3_t v1_proj = project_vertex(v1_world, mvp, canvas->width, canvas->height);

        if (!clip_to_circular_viewport(canvas, v0_proj.x, v0_proj.y) &&
            !clip_to_circular_viewport(canvas, v1_proj.x, v1_proj.y)) {
            continue;
        }

        vec3_t edge_dir = {
            v1_world.x - v0_world.x,
            v1_world.y - v0_world.y,
            v1_world.z - v0_world.z
        };

        vec3_t light_dir = {0.5f, 1.0f, 0.8f}; // can be customized
        float intensity = compute_lambert(edge_dir, light_dir);

        draw_line_f(canvas, v0_proj.x, v0_proj.y, v1_proj.x, v1_proj.y, intensity);
    }
}

