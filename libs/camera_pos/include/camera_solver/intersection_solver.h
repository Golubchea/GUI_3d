


#pragma once
#include <polynominals/polynominals.h>
#include <polynominals/numerical_root_finder.h>
#include <camera_solver/intersection_solver.h>
#include <containers/vector.h>
#include <vec3.h>
#define INTER_API extern
#define NOT_FOUND -999

typedef struct InPoint_t
{
    double x;
    double y;
}InPoint;

typedef struct VanishingPoints_t
{
    InPoint one;
    InPoint two;
}VanishingPoints;

typedef struct VanishingPoints_focal_t
{
    VanishingPoints vps;
    double focal_length;
}VanishingPoints_focal;


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

INTER_API InPoint InPoint_create(double x,double y);

INTER_API void InPoint_print(InPoint *v);
INTER_API void VanishingPoints_print(VanishingPoints *v);


INTER_API InPoint intersect_2d(InPoint *pa,InPoint *pb,InPoint *pc,InPoint *pd);

///Get the vanishing point of the lines AB and DC.
INTER_API InPoint get_vanishing_point(InPoint *pa,InPoint *pb,InPoint *pc,InPoint *pd);

/// Get the two vanishing points of the rectangle defined by the corners pa pb pc pd
INTER_API VanishingPoints get_vanishing_points(InPoint *pa,InPoint *pb,InPoint *pc,InPoint *pd);
///
/// \brief Convert a 2d point in the camera plane into a 3d vector from the camera onto the camera plane
///  field_of_view = 2 * atan(sensor_size / 2 * focal_length), assume sensor_size = 32
/// \param p
/// \param scale
/// \param focal_lenght
/// \return
///
INTER_API vec3 get_camera_plane_Vec3(InPoint *p,double scale,double focal_length);



//main function
/// - solve_F_S(): solves for the focal length using a single rectangle as input
INTER_API VanishingPoints_focal solve_F_S(InPoint *pa,InPoint *pb,InPoint *pc,InPoint *pd,double scale);


#ifdef __cplusplus
}
#endif // __cplusplus


