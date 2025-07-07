#ifndef MATH3D_H
#define MATH3D_H

#include <stdbool.h>

// 3D Vector structure (Cartesian + Spherical)
typedef struct {
    // Cartesian coordinates
    float x, y, z;

    // Spherical coordinates (automatically updated)
    float r, theta, phi;

    // Dirty flag for spherical coordinates
    bool spherical_dirty;
} vec3_t;

// 4x4 Matrix (column-major)
typedef struct {
    float m[4][4];
} mat4_t;

// Vector operations
vec3_t vec3_from_spherical(float r, float theta, float phi);
void vec3_normalize_fast(vec3_t* v);
vec3_t vec3_slerp(vec3_t a, vec3_t b, float t);

// Matrix operations
mat4_t mat4_identity(void);
mat4_t mat4_translate(float tx, float ty, float tz);
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_rotate_xyz(float rx, float ry, float rz);
mat4_t mat4_frustum_asymmetric(float left, float right, float bottom, float top, float near, float far);
mat4_t mat4_scale(float sx, float sy, float sz);

// Vector/matrix operations
vec3_t mat4_mult_vec3(mat4_t m, vec3_t v);


mat4_t mat4_perspective(float fov_radians, float aspect, float near, float far);
mat4_t mat4_rotate_y(float angle);
mat4_t mat4_multiply(mat4_t a, mat4_t b);
vec3_t mat4_transform(mat4_t m, vec3_t v);  // Renamed from mat4_mult_vec3

#endif
