#include <vec3.h>

#pragma once
#define INTERSECT_API extern


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


///
/// \brief intersect_line_plane solving system of plane and line in form:
/// {Ax+By+Cz+D=0
/// {(x-x1)/q1=(y-y1)/q2=(z-z1)/q3
/// \param cam_position
/// \param ray_direction
/// \param plane_normal
/// \param plane_height
/// \return
///
INTERSECT_API vec3 intersect_line_plane(vec3 cam_position, vec3 ray_direction, vec3 plane_normal,double plane_height);


typedef struct Plane
{
    float A;
    float B;
    float C;
    float D;
}Plane;

INTERSECT_API vec3 getNormal(Plane* p);

INTERSECT_API Plane getPlane(float* p);
INTERSECT_API vec3 IntersectionPoint(Plane *a,Plane * b, Plane * c);
INTERSECT_API vec3 IntersectionPoint2(Plane *a,Plane * b, Plane * c);

#ifdef __cplusplus
}
#endif // __cplusplus

