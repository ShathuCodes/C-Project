#include <stdlib.h>
#include <math.h>
#include "../include/canvas.h"

canvas_t* create_canvas(int width, int height) {
    canvas_t* canvas = malloc(sizeof(canvas_t));
    canvas->width = width;
    canvas->height = height;

    canvas->pixels = malloc(height * sizeof(float*));
    for (int y = 0; y < height; y++) {
        canvas->pixels[y] = calloc(width, sizeof(float));
    }

    return canvas;
}
void set_pixel_f(canvas_t* canvas, float x, float y, float intensity) {
    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    float dx = x - x0;
    float dy = y - y0;

    for (int i = 0; i <= 1; i++) {
        for (int j = 0; j <= 1; j++) {
            int xi = x0 + i;
            int yj = y0 + j;

            if (xi >= 0 && xi < canvas->width && yj >= 0 && yj < canvas->height) {
                float weight = (1.0f - fabsf(dx - i)) * (1.0f - fabsf(dy - j));
                canvas->pixels[yj][xi] += intensity * weight;

                if (canvas->pixels[yj][xi] > 1.0f)
                    canvas->pixels[yj][xi] = 1.0f;
            }
        }
    }
}

void draw_line_f(canvas_t* canvas, float x0, float y0, float x1, float y1, float thickness) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float steps = fmaxf(fabsf(dx), fabsf(dy))*2.0f;
    float step_x = dx / steps;    float step_y = dy / steps;

    for (int i = 0; i <= (int)steps; i++) {
        float x = x0 + i * step_x;
        float y = y0 + i * step_y;

        int r = (int)(thickness / 2);
        for (int tx = -r; tx <= r; tx++) {
            for (int ty = -r; ty <= r; ty++) {
                set_pixel_f(canvas, x + tx, y + ty, 1.0f);
            }
        }
    }
}


void destroy_canvas(canvas_t* canvas) {
    for (int y = 0; y < canvas->height; y++) {
        free(canvas->pixels[y]);
    }
    free(canvas->pixels);
    free(canvas);
}

void clear_canvas(canvas_t* canvas, float value) {
    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            canvas->pixels[y][x] = value;
        }
    }
}

