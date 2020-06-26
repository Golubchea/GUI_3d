
#pragma once
#define MAT_API extern
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <vec3.h>
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
//all matMxN[i][j] in form matMxN[i*M+j]

// check this function
//+
MAT_API void matrix_Setscale( float *matrix, float x,  float y,  float z);

MAT_API  void matrix_add_matrix(double * A ,double * B,double *res,unsigned int Arows,unsigned int Acols);
//-
MAT_API  void matrix_substract_matrix(double * A ,double * B,double *res,unsigned int Arows,unsigned int Acols);

MAT_API  void vec4_mul_matrix4x4_float(vec4 *B,float *A,  vec4  *res);
MAT_API  void matrix4x4_mul_vec4_float(float * A ,vec4 * B,vec4  *res);


// can parallelize this functions
MAT_API  void matrix_mul_matrix(double * A ,double * B,double *res,unsigned int Arows,unsigned int Acols,unsigned int Brows,unsigned int Bcols);
MAT_API  void matrix_mul_matrix_float(float * A ,float * B,float *res,unsigned int Arows,unsigned int Acols,unsigned int Brows,unsigned int Bcols);

MAT_API  void matrix4x4_vec4(float *A,vec4 *B,  vec4  *res);

MAT_API  void matrix_mul_scalar(double * src ,double scalar,unsigned int M,unsigned int N);

MAT_API  void matrix_transpose(double  *array, int m, int n);
MAT_API  void matrix_transpose_float(float  *array, int m, int n);
MAT_API  void matrix_transpose2(double  *array,double  *out , int m, int n);
//use malloc res[N+1]
MAT_API void Atranspose_mul_A(double *src,double *res,unsigned  M,unsigned   N);

//debug print
MAT_API void matrix_print(double* c,unsigned int M, unsigned int N, char* name);
MAT_API void matrix_print_float(float* c,unsigned int M, unsigned int N, char* name);
MAT_API void matrix_load(double * mat, int M);
MAT_API void matrix_zero(double * mat, int M);
MAT_API void matrix_zero_float(float * mat, int M);


MAT_API void matrix_copy(double * src,double * dst, int rang);
MAT_API void matrix_copy_float(const float * src,float * dst, int rang);

MAT_API void matrix_identity(double * mat, int M);
MAT_API void matrix_identity_float(float * mat, int M);

MAT_API void matrix_inverse3x3(double* m,double * res);
MAT_API bool matrix_inverse4x4(const double *m , double *invOut);

MAT_API  void matrix_add_matrix_float(float * A ,float * B,float *res,unsigned int Arows,unsigned int Acols);

MAT_API void matrix_inverse3x3_float2(float* m);

//cam pos
MAT_API  void matrix_mul_scalar_float(float * src ,float scalar,unsigned int M,unsigned int N);
MAT_API void matrix_multiply3x3_vector_float(float *resultvector, const float *matrix, const float *pvector);
MAT_API void matrix_inverse3x3_float(float* m,float * res);

//build mat
MAT_API  void matrix_getVec3_float_t(const float *m, float *vec3);
MAT_API  void matrix_getVec3_float(const float *m, float *vec3);
MAT_API  void matrix_build_model_float(const float *rot, const float *translate, float *out2);


//maybe wrong I tested it
MAT_API void matrix_inverse4x4_float( float *m);


//rotation
MAT_API  void matrix_rotate4x4_x_float(float * srcmat4x4 ,float  angle);
MAT_API  void matrix_rotate4x4_y_float(float * srcmat4x4 ,float  angle);
MAT_API  void matrix_rotate4x4_z_float(float * srcmat4x4 ,float  angle);
MAT_API  void matrix_rotate4x4_around_axis_float(float * srcmat4x4 ,vec3 u,float  angle);
//translate object
MAT_API  void matrix_translate4x4_float(float * srcmat4x4 ,float * translate_vector3);
MAT_API  void vec3_copy_float(const float *src,float *dst  );



MAT_API  void FPSViewRH( vec3 eye, float pitch, float yaw, float *out);
MAT_API  void matrix_build_model_float_from_blender(const float *rot, const float *translate, float *out );

#ifdef __cplusplus
}
#endif // __cplusplus





