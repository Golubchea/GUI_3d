#include "mathGL.h"
#include <stdio.h>
MATHGL_API void matrix_identityE(float *matrix)
{
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;

    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}

MATHGL_API void matrix_SetTranslation(float *matrix, float x, float y, float z)
{
    matrix[12] = x;
    matrix[13] = y;
    matrix[14] = z;
}

MATHGL_API void matrix_scale(float *matrix, float x, float y, float z)
{
    matrix[0] = x;
    matrix[5] = y;
    matrix[10] = z;
}



MATHGL_API void matrix_projection2d(float *projectionMatrix, float left, float right, float bottom, float top)
{
    float zRange = (left - right) * 2.0f;
    float zFar = zRange;
    float zNear = -zRange;

    projectionMatrix[0] = 2.0f / (right - left);
    projectionMatrix[1] = 0.0f;
    projectionMatrix[2] = 0.0f;
    projectionMatrix[3] = 0.0f;

    projectionMatrix[4] = 0.0f;
    projectionMatrix[5] = 2.0f / (top - bottom);
    projectionMatrix[6] = 0.0f;
    projectionMatrix[7] = 0.0f;

    projectionMatrix[8] = 0.0f;
    projectionMatrix[9] = 0.0f;
    projectionMatrix[10] = -2.0f / (zFar - zNear);
    projectionMatrix[11] = 0.0f;

    projectionMatrix[12] = -(right + left) / (right - left);
    projectionMatrix[13] = -(top + bottom) / (top - bottom);
    projectionMatrix[14] = (zFar + zNear) / (zFar - zNear);
    projectionMatrix[15] = 1.0f;
}

//perspective proj mat equations

MATHGL_API void matrix_projection_Frustum(float *matrix, float left, float right, float bottom, float top, float znear, float zfar)
{
    float temp, temp2, temp3, temp4;
    temp = 2.0 * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    matrix[0] = temp / temp2;
    matrix[1] = 0.0;
    matrix[2] = 0.0;
    matrix[3] = 0.0;
    matrix[4] = 0.0;
    matrix[5] = temp / temp3;
    matrix[6] = 0.0;
    matrix[7] = 0.0;
    matrix[8] = (right + left) / temp2;
    matrix[9] = (top + bottom) / temp3;
    matrix[10] = (-zfar - znear) / temp4;
    matrix[11] = -1.0;
    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = (-temp * zfar) / temp4;
    matrix[15] = 0.0;
}
#define VAR(Variable) (#Variable)

MATHGL_API void matrix_projection_Perspective(float *matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar)
{
    float ymax, xmax;
    // float temp, temp2, temp3, temp4;
    ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
    // ymin = -ymax;
    // xmin = -ymax * aspectRatio;
    xmax = ymax * aspectRatio;




    matrix_projection_Frustum(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);

//matrix_print_float(matrix,4,4,"projection:");

}
