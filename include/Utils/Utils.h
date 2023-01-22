#pragma once

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define PI 3.14159265358979323846

#define VEC3_LENGTH 3

#define VEC4_LENGTH 4

#define MAT4_LENGTH 16
#define MAT4_ROWS 4
#define MAT4_COLS 4

#define MAT4_COL_1 0
#define MAT4_COL_2 4
#define MAT4_COL_3 8
#define MAT4_COL_4 12

typedef union {
    struct {
        float x;
        float y;
        float z;
    };

    float data[VEC3_LENGTH];
} Vec3f;

typedef struct {
    float data[MAT4_LENGTH];
} Mat4f;

//*************************** Math ***************************//

extern float Utils_RadiansToDegrees(float radians);
extern float Utils_DegreesToRadians(float degrees);
extern float Utils_CorrectATan(float x, float y);

//************ Matrix/Vector Operations ************//

extern float Utils_Magnitude(const float *v, size_t length);
extern float Utils_Dot(const float *m1, const float *m2, size_t length);

// Immutable

extern float* Utils_Scale(const float *m, float scalar, float *output, size_t length);
extern float* Utils_Add(const float *m1, const float *m2, float *output, size_t length);
extern float* Utils_Subtract(const float *m1, const float *m2, float *output, size_t length);

extern float* Utils_Normalize(const float *v, float *output, size_t length);

// Mutable

extern float* Utils_Scale_Mutable(float *m, const float scalar, size_t length);
extern float* Utils_Add_Mutable(float *m1, const float *m2, size_t length);
extern float* Utils_Subtract_Mutable(float *m1, const float *m2, size_t length);

extern float* Utils_Normalize_Mutable(float *v, size_t length);

extern float* Utils_Copy_Mutable(float *dest, const float *src, size_t length);
extern float* Utils_SetAll(float *m, float value, size_t length);

//******* Vector 3 *******//

extern float  Utils_Vector3_Dot(const Vec3f *v1, const Vec3f *v2);
extern float  Utils_Vector3_Magnitude(const Vec3f *v);

// Immutable

extern Vec3f* Utils_Vector3_Scale(const Vec3f *v, float scalar, Vec3f *output);
extern Vec3f* Utils_Vector3_Add(const Vec3f *v1, const Vec3f *v2, Vec3f *output);
extern Vec3f* Utils_Vector3_Subtract(const Vec3f *v1, const Vec3f *v2, Vec3f *output);

extern Vec3f* Utils_Vector3_Normalize(const Vec3f *v, Vec3f *output);

extern Vec3f* Utils_Vector3_Cross(const Vec3f *v1, const Vec3f *v2, Vec3f *output);

// Mutable

extern Vec3f* Utils_Vector3_Scale_Mutable(Vec3f *v, float scalar);
extern Vec3f* Utils_Vector3_Add_Mutable(Vec3f *v1, const Vec3f *v2);
extern Vec3f* Utils_Vector3_Subtract_Mutable(Vec3f *v1, const Vec3f *v2);

extern Vec3f* Utils_Vector3_Normalize_Mutable(Vec3f *v);

extern Vec3f* Utils_Vector3_CrossL(Vec3f *v1, const Vec3f *v2);
extern Vec3f* Utils_Vector3_CrossR(const Vec3f *v1, Vec3f *v2);

extern Vec3f* Utils_Vector3_Copy(Vec3f *dest, const Vec3f *src);
extern Vec3f* Utils_Vector3_SetAll(Vec3f *m, float value);

//******* Mat4 Operations *******//

// Immutable

extern Mat4f* Utils_Matrix4_Multiply(const Mat4f *m1, const Mat4f *m2, Mat4f *output);
extern Mat4f* Utils_Matrix4_Transpose(const Mat4f *m, Mat4f *output);

// Mutable

extern Mat4f* Utils_Matrix4_Identity_Mutable(Mat4f *m);

extern Mat4f* Utils_Matrix4_MultiplyL(Mat4f *m1, const Mat4f *m2);
extern Mat4f* Utils_Matrix4_MultiplyR(const Mat4f *m1, Mat4f *m2);
extern Mat4f* Utils_Matrix4_Transpose_Mutable(Mat4f *m);

extern Mat4f* Utils_Matrix4_Copy(Mat4f *dest, const Mat4f *src);
extern Mat4f* Utils_Matrix4_SetAll(Mat4f *m, float value);

//******* Mat4 Rendering Operations *******//

extern void Utils_Matrix4_CalculatePerspective(Mat4f *matrix, float fovRadians, float aspectRatio, float near, float far);

extern Mat4f* Utils_Matrix4_Rotate(const Mat4f *matrix, const Vec3f *unormalizedAxis, float theta, Mat4f *output);
extern Mat4f* Utils_Matrix4_Translate(const Mat4f *matrix, const Vec3f *translation, Mat4f *output);

extern Mat4f* Utils_Matrix4_Rotate_Mutable(Mat4f *matrix, const Vec3f *unormalizedAxis, float theta);
extern Mat4f* Utils_Matrix4_Translate_Mutable(Mat4f *matrix, const Vec3f *translation);

extern Vec3f* Utils_TransformPoint(const Mat4f *matrix, const Vec3f *point, Vec3f *output);
extern Vec3f* Utils_TransformPoint_Mutable(const Mat4f *matrix, Vec3f *point);

//*************************** File I/O ***************************//

extern uint64_t Utils_FileIO_ReadFile(const char *filePath, char **buffer);
extern uint64_t Utils_FileIO_ReadFileTerminated(const char *filePath, char **buffer);

extern uint32_t Utils_FileIO_CRC32(const char *data);
extern float*   Utils_FileIO_ReadPNG_RGB(const char *filePath);
