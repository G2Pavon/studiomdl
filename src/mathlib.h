#ifndef __MATHLIB__
#define __MATHLIB__

// mathlib.h

#include <math.h>

typedef float vec3_t[3]; // x,y,z
typedef float vec4_t[4]; // x,y,z,w

#define SIDE_FRONT 0
#define SIDE_ON 2
#define SIDE_BACK 1
#define SIDE_CROSS -2

#define Q_PI 3.14159265358979323846

extern vec3_t vec3_origin;

// Use this definition globally
#define ON_EPSILON 0.01
#define EQUAL_EPSILON 0.001

int VectorCompare(const vec3_t v1, const vec3_t v2);
void VectorCopy(const vec3_t v1, vec3_t v2);
void VectorScale(const vec3_t v1, const float scalar, vec3_t v2);

void VectorAdd(const vec3_t v1, const vec3_t v2, vec3_t v);
void VectorSubstract(const vec3_t v1, const vec3_t v2, vec3_t v);
float DotProduct(const vec3_t v1, const vec3_t v2);
void CrossProduct(const vec3_t v1, const vec3_t v2, vec3_t cross);
float VectorNormalize(vec3_t v);

void AngleMatrix(const vec3_t angles, float matrix[3][4]);
void AngleIMatrix(const vec3_t angles, float matrix[3][4]);
void R_ConcatTransforms(const float in1[3][4], const float in2[3][4], float out[3][4]);

void VectorTransform(const vec3_t in1, const float in2[3][4], vec3_t out);

#endif