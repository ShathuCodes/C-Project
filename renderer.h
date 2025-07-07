#ifndef RENDERER_H
#define RENDERER_H

#include "canvas.h"
#include "math3d.h"
#include <stdbool.h>

// Projects a 3D point to 2D screen space
vec3_t project_vertex(vec3_t v, mat4_t mvp, int width, int height);

// Checks if a 2D point lies inside the circular viewport
bool clip_to_circular_viewport(canvas_t* canvas, float x, float y);

// Draws a 3D wireframe mesh from vertex/edge list
void render_wireframe(canvas_t* canvas, vec3_t* vertices, int* edges, int edge_count, mat4_t mvp);

#endif


