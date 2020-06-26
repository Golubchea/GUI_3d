


#pragma once
#include <polynominals/polynominals.h>
#include <polynominals/numerical_root_finder.h>
#include <camera_solver/intersection_solver.h>
#include <containers/vector.h>
#include <vec3.h>
#define INTR_CAM_API extern
#define NOT_FOUND -999


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct Rect3d_t
{
vec3 ra;
vec3 rb;
vec3 rc;
vec3 rd;
double badness;
} Rect3d;





INTR_CAM_API vec3 vertex_apply_transformation(vec3 point ,vec3 scale,vec3 rotation,vec3 translation);

INTR_CAM_API Vector get_lambda_d_poly_a (double qab,double qac,double qad,double qbc,double qbd,double qcd);
INTR_CAM_API Vector get_lambda_d_poly_b (double qab,double qac,double qad,double qbc,double qbd,double qcd);

INTR_CAM_API Rect3d get_lambda_d(InPoint  pa,InPoint pb,InPoint pc,InPoint pd,double scale,double focal_length);
INTR_CAM_API void Rect3d_print( Rect3d *r);

//Calculate the transformation of the rectangle
INTR_CAM_API Rect3d get_transformation(Rect3d *rect);
// Reconstruct the rotation angles of the transformation
INTR_CAM_API vec3 get_rot_angles(Rect3d *rect);


INTR_CAM_API vec3 get_real_obj_pose(vec3 object_pos, vec3 cam_pos, double camera_real_height);
INTR_CAM_API double get_real_focal(vec3 object_pos ,vec3  real_object_pos,vec3  camera_pos,vec3 real_camera_pos, double oldfocal);
#ifdef __cplusplus
}
#endif // __cplusplus


