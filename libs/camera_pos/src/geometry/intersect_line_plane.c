#include <geometry/intersect_line_plane.h>



INTERSECT_API vec3 intersect_line_plane(vec3 cam_position, vec3 ray_direction, vec3 plane_normal,double plane_height)
{
    double A=plane_normal.x;
    double B=plane_normal.y;
    double C=plane_normal.z;

    double q1=ray_direction.x;
    double q2=ray_direction.y;
    double q3=ray_direction.z;

    double D=plane_height;

    double x1=cam_position.x;
    double y1=cam_position.y;
    double z1=cam_position.z;


    double x=(   (B*q2*x1/q1)-(y1*B)+(x1*C*q3/q1)-(C*z1) -D )/  (      (A)+(B*q2/q1)+(C*q3/q1)          );
    double y=(   (A*q1*y1/q2)-(x1*A)+(y1*C*q3/q2)-(C*z1) -D )/  (      (B)+(A*q1/q2)+(C*q3/q2)          );
    double z=(   (A*q1*z1/q3)-(x1*A)+(z1*B*q2/q3)-(B*y1) -D)/  (      (C)+(A*q1/q3)+(B*q2/q3)          );

    return vec3_create(x,y,z);
}


INTERSECT_API vec3 getNormal(Plane* p)
{
    return vec3_create(p->A,p->B,p->C);
}

INTERSECT_API vec3 IntersectionPoint(Plane *a,Plane * b, Plane * c)
{
    // Formula used
    //                d1 ( N2 * N3 ) + d2 ( N3 * N1 ) + d3 ( N1 * N2 )
    //P =   ---------------------------------------------------------------------
    //                             N1 . ( N2 * N3 )
    //
    // Note: N refers to the normal, d refers to the displacement. '.' means dot product. '*' means cross product


    vec3 v1;
    vec3 v2;
    vec3 v3;

    vec3 b_normal=getNormal(b);
    vec3 c_normal=getNormal(c);

    vec3 bc_cross=vec3_cross(&b_normal,&c_normal);
    vec3 a_normal=getNormal(a);

    double f2= vec3_dot(&a_normal,&bc_cross);



    vec3_scale(&bc_cross,a->D);
    v1= vec3_copy(&bc_cross);
    vec3 ca_cross=vec3_cross(&c_normal,&a_normal);
    vec3 ab_cross=vec3_cross(&a_normal,&b_normal);


    vec3_scale(&ca_cross,b->D);
    vec3_scale(&ab_cross,c->D);
    v2=vec3_copy(&ca_cross);
    v3=vec3_copy(&ab_cross);

    //v2 = (b.D * (Vector3.Cross(c.Normal, a.Normal)));
    //v3 = (c.D * (Vector3.Cross(a.Normal, b.Normal)));

    vec3 vec=vec3_create(v1.x+v2.x+v3.x,v1.y+v2.y+v3.y,v1.z+v2.z+v3.z);
    vec3_scale(&vec,1.0/f2);

    return vec;
    //Vector3 vec = new Vector3(v1.X + v2.X + v3.X, v1.Y + v2.Y + v3.Y, v1.Z + v2.Z + v3.Z);
    //return vec / f2;
}

INTERSECT_API vec3 IntersectionPoint2(Plane *a,Plane * b, Plane * c)
{
    float A1=a->A;
    float A2=b->A;
    float A3=c->A;


    float B1=a->B;
    float B2=b->B;
    float B3=c->B;


    float C1=a->C;
    float C2=b->C;
    float C3=c->C;


    float D1=a->D;
    float D2=b->D;
    float D3=c->D;


    float temp1= (  (C2*A3)/(C3*B2) - A2/B2 )*   (  (C1*D3)/(C3*A1)  ) -  (  (C2*A3)/(C3*B2) -A2/B2   )*(D1/A1) ;
    float temp2=(1-(C1*A3)/(C3*A1) );


    float temp3=(C2*D3)/(C3*B2) -(D2/B2);
    float eq1=temp1/temp2+temp3;

    float temp4=     (1-(C2*B3)/(B2*C3) ) ;
    float temp5=  temp4- ( (C2*A3)/(C3*B2)- (A2/B2)  ) *  ( (C1*B3)/(C3*A1)- (B1/A1)  )/temp2 ;

    float y1=eq1/temp5;

    float nom= temp4*y1-(C2*D3)/(C3*B2)+(D2/B2);
    float denom=(C2*A3)/(C3*B2)-(A2/B2);

    float x1=nom/denom;

    float z1=(-A3*x1-B3*y1-D3 )/C3;

    vec3 v=vec3_create((double)x1,(double)y1,(double)z1);
    return v;
}

INTERSECT_API vec3 IntersectionPoint3(Plane *a,Plane * b, Plane * c)
{
    float A1=a->A;
    float A2=b->A;
    float A3=c->A;


    float B1=a->B;
    float B2=b->B;
    float B3=c->B;


    float C1=a->C;
    float C2=b->C;
    float C3=c->C;


    float D1=a->D;
    float D2=b->D;
    float D3=c->D;





   // vec3 v=vec3_create((double)x1,(double)y1,(double)z1);
   // return v;
}


Plane getPlane(float *p)
{
    Plane pp;

    pp.A=p[0];
    pp.B=p[1];
    pp.C=p[2];
    pp.D=p[3];

    return pp;
}
