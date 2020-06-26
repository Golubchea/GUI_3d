#include <polynominals/polynominals.h>
#include <polynominals/numerical_root_finder.h>
#include <camera_solver/intersection_solver.h>
#include <containers/vector.h>
#include <math.h>



INTER_API InPoint intersect_2d(InPoint *pa, InPoint *pb, InPoint *pc, InPoint *pd)
{
    InPoint ad;
    ad.x=pd->x  -   pa->x;
    ad.y=pd->y  -   pa->y;

    InPoint ab;
    ab.x=pb->x  -   pa->x;
    ab.y=pb->y  -   pa->y;

    InPoint cd;
    cd.x=pd->x  -   pc->x;
    cd.y=pd->y  -   pc->y;

    InPoint result;


    Answer_xy tmp= solve_linear_system_2d(ab.x,cd.x,ad.x,ab.y,cd.y,ad.y);
    if (tmp.x==ROOT_NOT_FOUND || tmp.x==ROOT_NOT_FOUND)
    {
        result.x=NOT_FOUND;
        result.y=NOT_FOUND;
        return result;
    }

    result.x=pa->x+ tmp.x*ab.x;
    result.y=pa->y+ tmp.x*ab.y;
    return result;
}

InPoint get_vanishing_point(InPoint *pa, InPoint *pb, InPoint *pc, InPoint *pd)
{
    InPoint p= intersect_2d(pa, pb, pc, pd);
    return p;
}


VanishingPoints get_vanishing_poins(InPoint *pa, InPoint *pb, InPoint *pc, InPoint *pd)
{
    VanishingPoints vp;
    InPoint p= intersect_2d(pa, pb, pc, pd);
    InPoint p2= intersect_2d(pa, pd, pb, pc);
    vp.one.x=p.x;
    vp.one.y=p.y;
    vp.two.x=p2.x;
    vp.two.y=p2.y;
    return vp;
}


vec3 get_camera_plane_Vec3(InPoint *p, double scale, double focal_length)
{
    //Convert a 2d point in the camera plane into a 3d vector from the camera onto the camera plane"""
    //field_of_view = 2 * atan(sensor_size / 2 * focal_length), assume sensor_size = 32
    double s = (16.0 / focal_length) / (scale / 2.0);
    vec3 v;
    v.x=p->x*s;
    v.y=p->y*s;
    v.z=-1.0;
    return v;
}



InPoint InPoint_create(double x, double y)
{
    InPoint v;
    v.x=x;
    v.y=y;


    return v;
}


void VanishingPoints_print(VanishingPoints *v)
{
    InPoint_print(&v->one);
    InPoint_print(&v->two);
}

void InPoint_print(InPoint *s)
{
       printf(" InPoint= (%lf,%lf) \n",s->x,s->y);
}

VanishingPoints_focal solve_F_S(InPoint *pa, InPoint *pb, InPoint *pc, InPoint *pd, double scale)
{

    VanishingPoints v_p=get_vanishing_poins(pa,pb,pc,pd);
    VanishingPoints_print(&v_p);
    vec3 vm=get_camera_plane_Vec3(&v_p.one,scale,1.0);
    vec3 vn=get_camera_plane_Vec3(&v_p.two,scale,1.0);
    printf("solve F\n");
    vec3_print(vm);
    vec3_print(vn);

    VanishingPoints_focal ans;
    ans.focal_length=sqrt(fabs(vec3_dot(&vm,&vn))  );
    ans.vps=v_p;
    return ans;
}
