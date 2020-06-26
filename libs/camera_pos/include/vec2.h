


#pragma once

#include <containers/vector.h>
#define VEC_API extern


typedef struct vec2
{
     float x;
     float y;


}vec2;




#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
VEC_API void   vec2_print(vec2 src);


VEC_API vec2 vec2_create(double x,double y);


VEC_API double vec2_dot(vec2 *a, vec2 *b);
VEC_API double vec2_distance(vec2 *t);
VEC_API double vec2_angle(vec2 *a, vec2 *b);
VEC_API vec2 vec2_normalize(vec2 *a);


VEC_API vec2 vec2_rotatePointXY(vec2 point,vec2 center,double angle);

//VEC_API double rads(double angle);
//VEC_API double degrees(double radians);





#ifdef __cplusplus
}
#endif // __cplusplus


