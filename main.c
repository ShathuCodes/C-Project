#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"

// Function prototypes
void run_task1_clock();
void run_task2_cube();
void run_task3_soccer();

// ==== Cube for Task 2 ====
static vec3_t cube_vertices[] = {
    {-1, -1, -1}, {1, -1, -1},
    {1, 1, -1}, {-1, 1, -1},
    {-1, -1, 1}, {1, -1, 1},
    {1, 1, 1}, {-1, 1, 1}
};

static int cube_edges[] = {
    0,1, 1,2, 2,3, 3,0,
    4,5, 5,6, 6,7, 7,4,
    0,4, 1,5, 2,6, 3,7
};

// ==== Soccer ball for Task 3 ====
static vec3_t soccer_vertices[] = {
    {0.500000f, 0.000000f, 2.427051f}, {0.500000f, 0.000000f, -2.427051f},
    {-0.500000f, 0.000000f, 2.427051f}, {-0.500000f, 0.000000f, -2.427051f},
    {2.427051f, 0.500000f, 0.000000f}, {2.427051f, -0.500000f, 0.000000f},
    {-2.427051f, 0.500000f, 0.000000f}, {-2.427051f, -0.500000f, 0.000000f},
    {0.000000f, 2.427051f, 0.500000f}, {0.000000f, 2.427051f, -0.500000f},
    {0.000000f, -2.427051f, 0.500000f}, {0.000000f, -2.427051f, -0.500000f},
    // ... (remaining soccer vertices omitted for brevity)
};

static int soccer_edges[][2] = {
    {0, 2}, {0, 12}, {0, 14},
    {1, 3}, {1, 13}, {1, 15},
    // ... (remaining soccer edges omitted for brevity)
};

// Save to PPM
void save_ppm(canvas_t* canvas, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) { perror("Failed to open file"); return; }
    fprintf(f, "P3\n%d %d\n255\n", canvas->width, canvas->height);
    for (int y=0; y<canvas->height; y++) {
        for (int x=0; x<canvas->width; x++) {
            int gray = (int)(canvas->pixels[y][x]*255);
            gray = gray > 255 ? 255 : (gray < 0 ? 0 : gray);
            fprintf(f, "%d %d %d ", gray, gray, gray);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void run_task2_cube() {
    int width = 800, height = 600;
    canvas_t* canvas = create_canvas(width, height);
    mat4_t view = mat4_translate(0, 0, -5);
    mat4_t proj = mat4_perspective(60.0f * (float)M_PI / 180.0f, (float)width/height, 0.1f, 100.0f);

    for (int frame = 0; frame < 60; frame++) {
        clear_canvas(canvas, 0.0f);
        float angle = 2.0f;  // fixed angle
        mat4_t model = mat4_rotate_xyz(0.0f, angle, 0.0f);
        mat4_t mvp = mat4_multiply(proj, mat4_multiply(view, model));

        for (int i = 0; i < 24; i += 2) {
            vec3_t v1 = mat4_transform(mvp, cube_vertices[cube_edges[i]]);
            vec3_t v2 = mat4_transform(mvp, cube_vertices[cube_edges[i+1]]);
            v1.x /= v1.z; v1.y /= v1.z;
            v2.x /= v2.z; v2.y /= v2.z;
            float x1 = (v1.x + 1) * width / 2;
            float y1 = (v1.y + 1) * height / 2;
            float x2 = (v2.x + 1) * width / 2;
            float y2 = (v2.y + 1) * height / 2;
            draw_line_f(canvas, x1, y1, x2, y2, 1.0f);
        }
        char filename[64];
        sprintf(filename, "build/frame_%03d.ppm", frame);
        save_ppm(canvas, filename);
    }
    destroy_canvas(canvas);
    printf("Rendered cube to build/\n");
}

void run_task3_soccer() {
    int width = 800, height = 600;
    canvas_t* canvas = create_canvas(width, height);
    mat4_t view = mat4_translate(0, 0, -8);
    mat4_t proj = mat4_perspective(45.0f * (float)M_PI / 180.0f, (float)width/height, 0.1f, 100.0f);

    for (int frame = 0; frame < 60; frame++) {
        clear_canvas(canvas, 0.0f);
        float angle = frame * 0.05f;
        mat4_t scale = mat4_scale(0.5f, 0.5f, 0.5f);
        mat4_t rotate = mat4_rotate_xyz(0.0f, angle, 0.0f);
        mat4_t model = mat4_multiply(rotate, scale);
        mat4_t mvp = mat4_multiply(proj, mat4_multiply(view, model));

        for (int i = 0; i < sizeof(soccer_edges)/sizeof(soccer_edges[0]); i++) {
            vec3_t v1 = soccer_vertices[soccer_edges[i][0]];
            vec3_t v2 = soccer_vertices[soccer_edges[i][1]];
            v1 = mat4_transform(mvp, v1);
            v2 = mat4_transform(mvp, v2);
            v1.x /= v1.z; v1.y /= v1.z;
            v2.x /= v2.z; v2.y /= v2.z;
            float x1 = (v1.x + 1) * width / 2;
            float y1 = (v1.y + 1) * height / 2;
            float x2 = (v2.x + 1) * width / 2;
            float y2 = (v2.y + 1) * height / 2;
            draw_line_f(canvas, x1, y1, x2, y2, 1.0f);
        }
        char filename[64];
        sprintf(filename, "build/soccer_%03d.ppm", frame);
        save_ppm(canvas, filename);
    }
    destroy_canvas(canvas);
    printf("Rendered soccer ball to build/\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("  %s task1\n", argv[0]);
        printf("  %s task2\n", argv[0]);
        printf("  %s task3\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "task1") == 0) {
        run_task1_clock();
    } else if (strcmp(argv[1], "task2") == 0) {
        run_task2_cube();
    } else if (strcmp(argv[1], "task3") == 0) {
        run_task3_soccer();
    } else {
        printf("Unknown task: %s\n", argv[1]);
        printf("Usage:\n");
        printf("  %s task1\n", argv[0]);
        printf("  %s task2\n", argv[0]);
        printf("  %s task3\n", argv[0]);
        return 1;
    }
    return 0;
}

void run_task1_clock() {
    int width = 800, height = 600;
    canvas_t* canvas = create_canvas(width, height);
    clear_canvas(canvas, 0.0f); // Clear to black

    float center_x = width / 2.0f;
    float center_y = height / 2.0f;
    float line_length = fminf(width, height) / 2.5f; // Line length relative to canvas size

    int num_lines = 24; // 360 / 15 = 24
    float angle_step = 15.0f * (float)M_PI / 180.0f; // 15 degrees in radians

    for (int i = 0; i < num_lines; i++) {
        float angle = i * angle_step;
        float end_x = center_x + line_length * cosf(angle);
        float end_y = center_y + line_length * sinf(angle);
        draw_line_f(canvas, center_x, center_y, end_x, end_y, 1.0f); // Use 1.0f for thickness
    }

    save_ppm(canvas, "build/task1_clock.ppm");
    destroy_canvas(canvas);
    printf("Rendered clock face to build/task1_clock.ppm\n");
}
