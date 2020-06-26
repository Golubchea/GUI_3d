#include "vec2.h"
#include <math.h>
#include <stdio.h>


VEC_API vec2 vec2_normalize(vec2 *a)
{
    vec2 v;

    double dist=  sqrt (a->x*a->x+ a->y*a->y);

    v.x=a->x/dist;
    v.y=a->y/dist;
    return v;
}

VEC_API double vec2_dot(vec2 *a, vec2 *b)
{
    return a->x*b->x  +  a->y*b->y  ;
}


VEC_API double vec2_distance(vec2 *t)
{
    return sqrt(t->x*t->x+t->y*t->y);
}

VEC_API double vec2_angle(vec2 *a, vec2 *b)
{
    double A=vec2_dot(a,b);
    double d1=vec2_distance(a);
    double d2=vec2_distance(b);

    return acos(A/(d1*d2)   );
}


VEC_API vec2 vec2_create(double x, double y)
{
    vec2 v;
    v.x=x;
    v.y=y;


    return v;
}

VEC_API void vec2_print(vec2 s)
{
    printf("\n vec2= (%lf,%lf) \n",s.x,s.y);
}


VEC_API vec2 vec2_rotatePointXY(vec2 point,vec2 center,double angle)
{
angle = (angle ) * (M_PI/180.0); // Convert to radians

double cs=cos(angle);
double sn=sin(angle);

double X = cs * (point.x - center.x) -  sn * (point.y - center.y) + center.x;
double Y = sn * (point.x - center.x) +  cs * (point.y - center.y) + center.y;

return vec2_create(X,Y);
}

