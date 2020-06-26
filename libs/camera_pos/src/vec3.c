#include <vec3.h>
#include <math.h>
#include <matrix/matrix.h>
VEC_API double rads(double angle)
{
    return (angle/180.0)*M_PI;
}


VEC_API double degrees(double radians)
{
    return (radians*180.0)/M_PI;
}


VEC_API vec3 vec3_create(double x, double y, double z)
{
    vec3 v;
    v.x=x;
    v.y=y;
    v.z=z;

    return v;
}

VEC_API vec3 vec3_copy(vec3 *src)
{
    vec3 dst;
    dst.x=src->x;
    dst.y=src->y;
    dst.z=src->z;
    return  dst;
}

VEC_API double vec3_dot(vec3 *a, vec3 *b)
{
    return a->x*b->x  +  a->y*b->y  +  a->z*b->z;
}
//https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
vec3 vec3_rotate_around_axis(vec3 v,vec3 k,float angle)
{

    angle=rads(angle);
    vec3 v1=vec3_scale2(v,cos(angle));


    vec3 v2=vec3_cross(&k,&v);
    v2=vec3_scale2(v2,sin(angle));


    double scalar= vec3_dot(&k,&v)*(1.0-cos(angle));
    vec3 v3=vec3_scale2(k,scalar);


    vec3 temp1 =vec3_add(&v1,&v2);
    vec3 a =vec3_add(&temp1,&v3);


    return a;
}



VEC_API void vec3_print(vec3 s)
{
    printf(" vec3= (%lf,%lf,%lf) \n",s.x,s.y,s.z);
}

VEC_API vec3 vec3_normalize(vec3 *a)
{
    vec3 v;

    double dist=  sqrt (a->x*a->x+ a->y*a->y+ a->z*a->z);

    v.x=a->x/dist;
    v.y=a->y/dist;
    v.z=a->z/dist;
    return v;
}







VEC_API vec3 vec3_add(vec3 *a, vec3 *b)
{
    vec3 v;
    v.x=a->x   +   b->x;
    v.y=a->y   +   b->y;
    v.z=a->z   +   b->z;
    return v;
}

VEC_API void vec3_add_ptr(vec3 *a, vec3 *b)
{

    a->x=a->x   +   b->x;
    a->y=a->y   +   b->y;
    a->z=a->z   +   b->z;

}


VEC_API void vec3_scale(vec3 *a,double scale)
{
    a->x=a->x   *scale;
    a->y=a->y   *scale;
    a->z=a->z   *scale;
}


VEC_API vec3 vec3_scale2(vec3  a,double scale)
{
    vec3 b;
    b.x=a.x   *scale;
    b.y=a.y   *scale;
    b.z=a.z   *scale;
    return  b;
}



VEC_API vec3 vec3_substract(vec3 *a, vec3 *b)
{
    vec3 v;
    v.x=a->x-b->x;
    v.y=a->y-b->y;
    v.z=a->z-b->z;
    return v;
}

VEC_API vec3 vec3_cross(vec3 *a, vec3 *b)
{
    double X=a->y*b->z -  a->z*b->y;
    double Y=a->z*b->x -  a->x*b->z;
    double Z=a->x*b->y -  a->y*b->x;
    return vec3_create(X,Y,Z);
}

VEC_API double vec3_distance(vec3 *t)
{
    return sqrt(t->x*t->x+t->y*t->y+t->z*t->z);
}

VEC_API double vec3_angle(vec3 *a, vec3 *b)
{
    double A=vec3_dot(a,b);
    double d1=vec3_distance(a);
    double d2=vec3_distance(b);

    return acos(A/(d1*d2)   );
}

VEC_API void vec4_print(vec4 s)
{
    printf("\n vec4= (%lf,%lf,%lf,%lf) \n",s.vals[0],s.vals[1],s.vals[2],s.vals[3]);
}

VEC_API vec4 vec4_create(double x, double y, double z, double w)
{
    vec4 v;
    v.vals[0]=x;
    v.vals[1]=y;
    v.vals[2]=z;
    v.vals[3]=w;

    return v;
}

float vec3_distance2(vec3 *a, vec3 *b)
{
    vec3 v=vec3_substract(b,a);

    return vec3_distance(&v);
}
