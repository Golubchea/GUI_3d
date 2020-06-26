


#pragma once
#include <polynominals/polynominals.h>
#include <polynominals/numerical_root_finder.h>

#include <containers/vector.h>
#define VEC_API extern


typedef struct vec4
{
    double vals[4];
} vec4;

typedef struct vec3
{
     double x;
     double y;
     double z;

}vec3;




#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
VEC_API void   vec4_print(vec4 src);
//rodrigues shiat
vec3 vec3_rotate_around_axis(vec3 v,vec3 k,float angle);
VEC_API vec4 vec4_create(double x,double y,double z,double w);

VEC_API void vec3_scale(vec3 *a,double scale);
VEC_API vec3 vec3_scale2(vec3  a,double scale);

VEC_API void vec3_add_ptr(vec3 *a, vec3 *b);
VEC_API vec3 vec3_copy(vec3 *src);
VEC_API vec3 vec3_create(double x,double y,double z);
VEC_API void   vec3_print(vec3 src);
VEC_API double vec3_dot(vec3 * a,vec3 * b);
VEC_API vec3 vec3_normalize(vec3 * a);

VEC_API vec3 vec3_add(vec3 *a, vec3 *b);
VEC_API vec3 vec3_substract(vec3 * a,vec3 * b);

VEC_API float vec3_distance2(vec3 * a,vec3 * b);

VEC_API vec3 vec3_cross(vec3 * a,vec3 * b);
VEC_API double vec3_distance(vec3 *t);
VEC_API double vec3_angle(vec3 * a,vec3 * b);

VEC_API double rads(double angle);
VEC_API double degrees(double radians);




#ifdef __cplusplus
}
#endif // __cplusplus


