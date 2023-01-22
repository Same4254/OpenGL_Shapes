#include "Utils/Utils.h"

float Utils_Magnitude(const float *v, size_t length) {
    float sum = 0;
    for (size_t i = 0; i < length; i++)
        sum += v[i] * v[i];
    return sqrtf(sum);
}

// Immutable

float* Utils_Scale(const float *m, float scalar, float *output, size_t length) {
    for (size_t i = 0; i < length; i++)
        output[i] = scalar * m[i];

    return output;
}

float* Utils_Add(const float *m1, const float *m2, float *output, size_t length) {
    for (size_t i = 0; i < length; i++)
        output[i] = m1[i] + m2[i];

    return output;
}

float* Utils_Subtract(const float *m1, const float *m2, float *output, size_t length) {
    for (size_t i = 0; i < length; i++)
        output[i] = m1[i] - m2[i];

    return output;
}

float  Utils_Dot(const float *m1, const float *m2, size_t length) {
    float sum = 0;

    for (size_t i = 0; i < length; i++)
        sum += m1[i] * m2[i];

    return sum;
}

float* Utils_Normalize(const float *v, float *output, size_t length) {
    float magnitude = Utils_Magnitude(v, length);

    return Utils_Scale(v, 1.0f / magnitude, output, length);
}

// Mutable

float* Utils_Scale_Mutable(float *m, const float scalar, size_t length) {
    return Utils_Scale(m, scalar, m, length);
}

float* Utils_Add_Mutable(float *m1, const float *m2, size_t length) {
    return Utils_Add(m1, m2, m1, length);
}

float* Utils_Subtract_Mutable(float *m1, const float *m2, size_t length) {
    return Utils_Subtract(m1, m2, m1, length);
}

float* Utils_Normalize_Mutable(float *v, size_t length) {
    return Utils_Normalize(v, v, length);
}

float* Utils_Copy_Mutable(float *dest, const float *src, size_t length) {
    memcpy(dest, src, sizeof(float) * length);

    return dest;
}

float* Utils_SetAll(float *m, float value, size_t length) {
    for (size_t i = 0; i < length; i++)
        m[i] = value;

    return m;
}

//******* Vector 3 *******//

float  Utils_Vector3_Dot(const Vec3f *v1, const Vec3f *v2) {
    return Utils_Dot(v1->data, v2->data, VEC3_LENGTH);
}

float  Utils_Vector3_Magnitude(const Vec3f *v) {
    return Utils_Magnitude(v->data, VEC3_LENGTH);
}

// Immutable

Vec3f* Utils_Vector3_Scale(const Vec3f *v, float scalar, Vec3f *output) {
    Utils_Scale(v->data, scalar, output->data, VEC3_LENGTH);

    return output;
}

Vec3f* Utils_Vector3_Add(const Vec3f *v1, const Vec3f *v2, Vec3f *output) {
    Utils_Add(v1->data, v2->data, output->data, VEC3_LENGTH);

    return output;
}

Vec3f* Utils_Vector3_Subtract(const Vec3f *v1, const Vec3f *v2, Vec3f *output) {
    Utils_Subtract(v1->data, v2->data, output->data, VEC3_LENGTH);

    return output;
}

Vec3f* Utils_Vector3_Normalize(const Vec3f *v, Vec3f *output) {
    Utils_Normalize(v->data, output->data, VEC3_LENGTH);

    return output;
}

Vec3f* Utils_Vector3_Cross(const Vec3f *v1, const Vec3f *v2, Vec3f *output) {
    output->x = (v1->y * v2->z) - (v1->z * v2->y);
    output->y = (v1->z * v2->x) - (v1->x * v2->z);
    output->z = (v1->x * v2->y) - (v1->y * v2->x);

    return output;
}

// Mutable

Vec3f* Utils_Vector3_Scale_Mutable(Vec3f *v, float scalar) {
    Utils_Scale(v->data, scalar, v->data, VEC3_LENGTH);
    return v;
}

Vec3f* Utils_Vector3_Add_Mutable(Vec3f *v1, const Vec3f *v2) {
    Utils_Add(v1->data, v2->data, v1->data, VEC3_LENGTH);
    return v1;
}

Vec3f* Utils_Vector3_Subtract_Mutable(Vec3f *v1, const Vec3f *v2) {
    Utils_Subtract(v1->data, v2->data, v1->data, VEC3_LENGTH);
    return v1;
}

Vec3f* Utils_Vector3_Normalize_Mutable(Vec3f *v) {
    Utils_Normalize(v->data, v->data, VEC3_LENGTH);
    return v;
}

Vec3f* Utils_Vector3_CrossL(Vec3f *v1, const Vec3f *v2) {
    Vec3f temp;

    Utils_Vector3_Cross(v1, v2, &temp);
    Utils_Vector3_Copy(v1, &temp);

    return v1;
}

Vec3f* Utils_Vector3_CrossR(const Vec3f *v1, Vec3f *v2) {
    Vec3f temp;

    Utils_Vector3_Cross(v1, v2, &temp);
    Utils_Vector3_Copy(v2, &temp);

    return v2;
}

Vec3f* Utils_Vector3_Copy(Vec3f *dest, const Vec3f *src) {
    Utils_Copy_Mutable(dest->data, src->data, VEC3_LENGTH);

    return dest;
}

Vec3f* Utils_Vector3_SetAll(Vec3f *m, float value) {
    Utils_SetAll(m->data, value, VEC3_LENGTH);

    return m;
}

//******* Mat 4 *******//

// Immutable

Mat4f* Utils_Matrix4_Multiply(const Mat4f *m1, const Mat4f *m2, Mat4f *output) {
    size_t resultIndex = 0;
    for (size_t colStartIndex = 0; colStartIndex < MAT4_LENGTH; colStartIndex += MAT4_ROWS) {
        for (size_t rowStartIndex = 0; rowStartIndex < MAT4_ROWS; rowStartIndex++) {
            float sum = 0;
            size_t rightIndex = colStartIndex;
            for (size_t leftIndex = 0; leftIndex < MAT4_LENGTH; leftIndex += MAT4_ROWS) {
                sum += m1->data[leftIndex + rowStartIndex] * m2->data[rightIndex];
                rightIndex ++;
            }

            output->data[resultIndex] = sum;
            resultIndex++;
        }
    }

    return output;
}

Mat4f* Utils_Matrix4_Transpose(const Mat4f *m, Mat4f *output) {
    size_t resultIndex = 0;
    for (size_t colStartIndex = 0; colStartIndex < MAT4_COLS; colStartIndex++) {
        for (size_t index = colStartIndex; index < MAT4_LENGTH; index += MAT4_COLS) {
            output->data[resultIndex] = m->data[index];
            resultIndex++;
        }
    }

    return output;
}

// Mutable

Mat4f* Utils_Matrix4_Identity_Mutable(Mat4f *m) {
    Utils_Matrix4_SetAll(m, 0.0f);

    m->data[0] = 1;
    m->data[5] = 1;
    m->data[10] = 1;
    m->data[15] = 1;

    return m;
}

Mat4f* Utils_Matrix4_MultiplyL(Mat4f *m1, const Mat4f *m2) {
    Mat4f temp;

    Utils_Matrix4_Multiply(m1, m2, &temp);
    Utils_Matrix4_Copy(m1, &temp);

    return m1;
}

Mat4f* Utils_Matrix4_MultiplyR(const Mat4f *m1, Mat4f *m2) {
    Mat4f temp;

    Utils_Matrix4_Multiply(m1, m2, &temp);
    Utils_Matrix4_Copy(m2, &temp);

    return m2;
}

Mat4f* Utils_Matrix4_Transpose_Mutable(Mat4f *m) {
    Mat4f temp;

    Utils_Matrix4_Transpose(m, &temp);
    Utils_Matrix4_Copy(m, &temp);

    return m;
}

Mat4f* Utils_Matrix4_Copy(Mat4f *dest, const Mat4f *src) {
    memcpy(dest->data, src->data, sizeof(float) * 16);

    return dest;
}

Mat4f* Utils_Matrix4_SetAll(Mat4f *m, float value) {
    Utils_SetAll(m->data, value, MAT4_LENGTH);

    return m;
}

void Utils_Matrix4_CalculatePerspective(Mat4f *matrix, float fovRadians, float aspectRatio, float near, float far) {
    Utils_Matrix4_SetAll(matrix, 0.0f);

    matrix->data[0] = 1.0f / (aspectRatio * tanf(fovRadians / 2.0f));
    matrix->data[5] = 1.0f / tanf(fovRadians / 2.0f);
    matrix->data[10] = (-(far + near)) / (far - near);

    matrix->data[11] = -1.0f;
    matrix->data[14] = (-(2.0f * far * near) / (far - near));
}

Mat4f* Utils_Matrix4_Rotate(const Mat4f *matrix, const Vec3f *unormalizedAxis, float theta, Mat4f *output) {
    Vec3f axis;

    Utils_Vector3_Normalize(unormalizedAxis, &axis);

    Mat4f rotationMatrix;

    rotationMatrix.data[0] = cosf(theta) + axis.x * axis.x * (1.0f - cosf(theta));
    rotationMatrix.data[1] = axis.x * axis.y * (1.0f - cosf(theta)) + axis.z * sinf(theta);
    rotationMatrix.data[2] = axis.x * axis.z * (1.0f - cosf(theta)) - axis.y * sinf(theta);
    rotationMatrix.data[3] = 0;

    rotationMatrix.data[4] = axis.x * axis.y * (1.0f - cosf(theta)) - axis.z * sinf(theta);
    rotationMatrix.data[5] = cosf(theta) + axis.y * axis.y * (1.0f - cosf(theta));
    rotationMatrix.data[6] = axis.y * axis.z * (1.0f - cosf(theta)) + axis.x * sinf(theta);
    rotationMatrix.data[7] = 0;

    rotationMatrix.data[8] = axis.x * axis.z * (1.0f - cosf(theta)) + axis.y * sinf(theta);
    rotationMatrix.data[9] = axis.y * axis.z * (1.0f - cosf(theta)) - axis.x * sinf(theta);
    rotationMatrix.data[10] = cosf(theta) + axis.x * axis.x * (1.0f - cosf(theta));
    rotationMatrix.data[11] = 0;

    rotationMatrix.data[12] = 0;
    rotationMatrix.data[13] = 0;
    rotationMatrix.data[14] = 0;
    rotationMatrix.data[15] = 1;

    Utils_Matrix4_Multiply(matrix, &rotationMatrix, output);

    return output;
}

Mat4f* Utils_Matrix4_Translate(const Mat4f *matrix, const Vec3f *translation, Mat4f *output) {
    Mat4f translationMatrix;
    Utils_Matrix4_Identity_Mutable(&translationMatrix);

    Utils_Copy_Mutable(&translationMatrix.data[12], translation->data, VEC3_LENGTH);
    Utils_Matrix4_Multiply(&translationMatrix, matrix, output);

    return output;
}

Mat4f* Utils_Matrix4_Rotate_Mutable(Mat4f *matrix, const Vec3f *unormalizedAxis, float theta) {
    Mat4f temp;

    Utils_Matrix4_Rotate(matrix, unormalizedAxis, theta, &temp);
    Utils_Matrix4_Copy(matrix, &temp);

    return matrix;
}

Mat4f* Utils_Matrix4_Translate_Mutable(Mat4f *matrix, const Vec3f *translation) {
    Mat4f temp;

    Utils_Matrix4_Translate(matrix, translation, &temp);
    Utils_Matrix4_Copy(matrix, &temp);

    return matrix;
}

Vec3f* Utils_TransformPoint(const Mat4f *matrix, const Vec3f *point, Vec3f *output) {
    output->data[0] = matrix->data[0] * point->data[0] + matrix->data[4] * point->data[1] + matrix->data[8] * point->data[2];
    output->data[1] = matrix->data[1] * point->data[0] + matrix->data[5] * point->data[1] + matrix->data[9] * point->data[2];
    output->data[2] = matrix->data[2] * point->data[0] + matrix->data[6] * point->data[1] + matrix->data[10] * point->data[2];

    return output;
}

Vec3f* Utils_TransformPoint_Mutable(const Mat4f *matrix, Vec3f *point) {
    Vec3f temp;
    Utils_TransformPoint(matrix, point, &temp);
    Utils_Vector3_Copy(point, &temp);

    return point;
}
