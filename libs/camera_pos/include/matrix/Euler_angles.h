#pragma once
#define MAT_API extern
#include <vec3.h>
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus



typedef struct quat
{
    double x;
    double y;
    double z;
    double w;
} quat;
enum RotSeq{zyx, zyz, zxy, zxz, yxz, yxy, yzx, yzy, xyz, xyx, xzy,xzx};

MAT_API void Rodrigues(vec3  r , float *Matrix);
MAT_API void quaternion2Euler (const quat q, double * res, enum RotSeq rotSeq);
///
/// \brief rotationMatrixToEulerAngles
/// \param R[3*3] - matrix 3x3
/// \param angles[3] -Euler angles in degrees
///
MAT_API void rotationMatrixToEulerAngles_float(float *Yaw2,float *Pitch2,float *Roll2,float * R  );// (float*)R[3][3] or R[3*3]
// solution_number =1 by default
MAT_API void euler_to_mat3_double(double M[3][3], vec3 e );


MAT_API void getEulerYPR(float * yaw,float*pitch, float*roll,float *mat, unsigned int solution_number);
MAT_API void eulO_to_mat3(float M[3][3], vec3 e );

MAT_API void matrix_mul3x3_vec3_float(vec3 *resultvector, const float *matrix, vec3 * pvector);
MAT_API void matrix_mul3x3_vec3(vec3 *resultvector, const double *matrix, vec3 * pvector);
MAT_API void matrix_mul3x3_vec3_in( const double *matrix, vec3   * p );
#ifdef __cplusplus
}
#endif // __cplusplus
