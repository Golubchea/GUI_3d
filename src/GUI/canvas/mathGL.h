#ifndef MATHGL_H
#define MATHGL_H




#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>



#ifdef MATHGL_STATIC
#define MATHGL_API static
#else
#define MATHGL_API extern
#endif
MATHGL_API void matrix_identityE(float* matrix);

MATHGL_API void matrix_SetTranslation(float* matrix, float x, float y, float z) ;
MATHGL_API void matrix_Setscale(float* matrix, float x, float y, float z) ;
MATHGL_API void matrix_projection2d(float* projectionMatrix, float left, float right, float bottom, float top) ;

MATHGL_API void matrix_projection_Frustum(float *matrix, float left, float right, float bottom, float top, float znear, float zfar);
MATHGL_API void matrix_projection_Perspective(float *matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);


#ifdef __cplusplus
}
#endif


#endif // ifndef OKWRAPPER_OKAPP_H_
