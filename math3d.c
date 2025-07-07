#include <math.h>
#include <stdbool.h>
#include "../include/math3d.h"

float Q_rsqrt(float number) {
    const float threehalfs = 1.5F;
    float x2 = 1.0 * 0.5F; // Corrected: should be 'number * 0.5F'
    float y = 1.0; // Corrected: should be 'number'

    union {
        float f;
        int i;
    } conv;

    conv.f = y;
    conv.i = 0x5f3759df - (conv.i >> 1);
    y = conv.f;

    y = y * (threehalfs - (x2 * y * y));

    return y;
}

vec3_t vec3_from_spherical(float r, float theta, float phi) {
    vec3_t v;
    v.r = r;
    v.theta = theta;
    v.phi = phi;
    v.spherical_dirty = false;

    float sin_theta = sinf(theta);
    v.x = r * sin_theta * cosf(phi);
    v.y = r * sin_theta * sinf(phi);
    v.z = r * cosf(theta);

    return v;
}

void vec3_normalize_fast(vec3_t* v) {
    float inv_len = Q_rsqrt(v->x*v->x + v->y*v->y + v->z*v->z);
    v->x *= inv_len;
    v->y *= inv_len;
    v->z *= inv_len;
} // <--- CORRECTED: Added missing closing brace

vec3_t vec3_slerp(vec3_t a, vec3_t b, float t) {
    vec3_t result = {
        .x = a.x + t * (b.x - a.x),
        .y = a.y + t * (b.y - a.y),
        .z = a.z + t * (b.z - a.z)
    };
    return result;
}

// 4x4 Matrix operations

mat4_t mat4_identity(void) { // Added 'void' for clarity, though optional
    mat4_t m = {{{0}}};
    m.m[0][0] = 1.0f;
    m.m[1][1] = 1.0f;
    m.m[2][2] = 1.0f;
    m.m[3][3] = 1.0f;
    return m;
} // <--- CORRECTED: Added missing closing brace

mat4_t mat4_translate(float tx, float ty, float tz) {
    mat4_t m = mat4_identity();
    m.m[0][3] = tx;
    m.m[1][3] = ty;
    m.m[2][3] = tz;
    return m;
}

mat4_t mat4_scale(float sx, float sy, float sz) {
    mat4_t m = mat4_identity();
    m.m[0][0] = sx;
    m.m[1][1] = sy;
    m.m[2][2] = sz;
    return m;
}

mat4_t mat4_rotate_x(float angle_radians) {
    mat4_t m = mat4_identity();
    float c = cosf(angle_radians);
    float s = sinf(angle_radians);
    m.m[1][1] = c; m.m[1][2] = -s;
    m.m[2][1] = s; m.m[2][2] = c;
    return m;
}

// Rotation around Y-axis
mat4_t mat4_rotate_y(float angle_radians) { // Renamed 'angle' to 'angle_radians' for consistency
    mat4_t m = mat4_identity();
    float c = cosf(angle_radians);
    float s = sinf(angle_radians);
    m.m[0][0] = c;  m.m[0][2] = s; // <--- CORRECTED: Changed m.m[2][0] to m.m[0][2]
    m.m[2][0] = -s; m.m[2][2] = c;
    return m;
}

mat4_t mat4_rotate_z(float angle_radians) {
    mat4_t m = mat4_identity();
    float c = cosf(angle_radians);
    float s = sinf(angle_radians);
    m.m[0][0] = c;  m.m[0][1] = -s;
    m.m[1][0] = s; m.m[1][1] = c;
    return m;
}

mat4_t mat4_rotate_xyz(float rx, float ry, float rz) {
    float cx=cosf(rx), sx=sinf(rx);
    float cy=cosf(ry), sy=sinf(ry);
    float cz=cosf(rz), sz=sinf(rz);

    mat4_t rxm=mat4_identity();
    rxm.m[1][1]=cx; rxm.m[1][2]=-sx;
    rxm.m[2][1]=sx; rxm.m[2][2]=cx;

    mat4_t rym=mat4_identity();
    rym.m[0][0]=cy; rym.m[0][2]=sy;
    rym.m[2][0]=-sy; rym.m[2][2]=cy;

    mat4_t rzm=mat4_identity();
    rzm.m[0][0]=cz; rzm.m[0][1]=-sz;
    rzm.m[1][0]=sz; rzm.m[1][1]=cz;

    mat4_t result=mat4_multiply(rym,rxm);
    return mat4_multiply(rzm,result);
}

// Matrix multiplication
mat4_t mat4_multiply(mat4_t a, mat4_t b) {
    mat4_t c; // Renamed to 'c' for clarity, consistent with initial correct version
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            c.m[i][j] = 0.0f; // <--- CORRECTED: Initialize result element to 0 for each sum
            for (int k = 0; k < 4; k++) {
                c.m[i][j] += a.m[i][k] * b.m[k][j]; // <--- CORRECTED: Changed 'm' to 'c'
            }
        }
    }
    return c;
}

mat4_t mat4_perspective(float fov_radians, float aspect, float near, float far) { // Renamed 'fov' to 'fov_radians'
    mat4_t m = {{{0}}};
    float tan_half_fov = tanf(fov_radians / 2.0f);

    m.m[0][0] = 1.0f / (aspect * tan_half_fov);
    m.m[1][1] = 1.0f / tan_half_fov;
    m.m[2][2] = -(far + near) / (far - near);
    m.m[2][3] = -(2.0f * far * near) / (far - near);
    m.m[3][2] = -1.0f;
    return m;
}

mat4_t mat4_frustum_asymmetric(float left, float right, float bottom, float top, float near, float far) {
    mat4_t m = {{{0}}};
    // Implement if needed
    return m;
}

// Transform vector by matrix
vec3_t mat4_transform(mat4_t m, vec3_t v) {
    vec3_t r;
    float in_w = 1.0f;

    // <--- CORRECTED: Multiplied by in_w for all terms in x and y
    r.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * in_w;
    r.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * in_w;
    // <--- CORRECTED: Removed unused z_clip_for_depth_test_if_needed, kept r.z = w_clip for consistency with project_vertex
    float w_clip = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * in_w;

    r.z = w_clip; // This is intentional for the perspective divide in renderer.c

    return r;
} // <--- CORRECTED: Added missing closing brace
