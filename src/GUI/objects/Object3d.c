#include "Object3d.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <SDL.h>
#include <matrix/matrix.h>

Object3d *Object3d_create(vec3 position,Color color)
{
    Object3d * obj=(Object3d *)malloc(sizeof (Object3d));
    //position
    matrix_identity_float(&obj->model[0],4);
    float translate[16];

    matrix_identity_float(&translate[0],4);
    translate[3]=position.x;
    translate[7]=position.y;
    translate[11]=position.z;

    float res[16];
    matrix_mul_matrix_float(obj->model,translate,res,4,4,4,4);
    matrix_copy_float(res,obj->model,4);
    matrix_transpose_float(obj->model,4,4);
    obj->color[0]= color.r;
    obj->color[1]= color.g;
    obj->color[2]= color.b;
    obj->color[3]= color.a;
    obj->vertices=NULL;
    obj->elements=NULL;
    return obj;
}

void Object3d_Cube(Object3d * obj,vec3 volume)
{


    GLfloat cube_vertices [] = {
        // front
        -1.0*volume.x, -1.0*volume.y, +1.0*volume.z,
        +1.0*volume.x, -1.0*volume.y, +1.0*volume.z,
        +1.0*volume.x, +1.0*volume.y, +1.0*volume.z,
        -1.0*volume.x, +1.0*volume.y, +1.0*volume.z,
        // top
        -1.0*volume.x, +1.0*volume.y, +1.0*volume.z,
        +1.0*volume.x, +1.0*volume.y, +1.0*volume.z,
        +1.0*volume.x, +1.0*volume.y, -1.0*volume.z,
        -1.0*volume.x, +1.0*volume.y, -1.0*volume.z,
        // back
        +1.0*volume.x, -1.0*volume.y, -1.0*volume.z,
        -1.0*volume.x, -1.0*volume.y, -1.0*volume.z,
        -1.0*volume.x, +1.0*volume.y, -1.0*volume.z,
        +1.0*volume.x, +1.0*volume.y, -1.0*volume.z,
        // bottom
        -1.0*volume.x, -1.0*volume.y, -1.0*volume.z,
        +1.0*volume.x, -1.0*volume.y, -1.0*volume.z,
        +1.0*volume.x, -1.0*volume.y, +1.0*volume.z,
        -1.0*volume.x, -1.0*volume.y, +1.0*volume.z,
        // left
        -1.0*volume.x, -1.0*volume.y, -1.0*volume.z,
        -1.0*volume.x, -1.0*volume.y, +1.0*volume.z,
        -1.0*volume.x, +1.0*volume.y, +1.0*volume.z,
        -1.0*volume.x, +1.0*volume.y, -1.0*volume.z,
        // right
        +1.0*volume.x, -1.0*volume.y, +1.0*volume.z,
        +1.0*volume.x, -1.0*volume.y, -1.0*volume.z,
        +1.0*volume.x, +1.0*volume.y, -1.0*volume.z,
        +1.0*volume.x, +1.0*volume.y, +1.0*volume.z,
    };

    SDL_Log(" obj size:%d bytes  %d",sizeof (cube_vertices),sizeof (cube_vertices)/sizeof (cube_vertices[0]) );

    static const GLushort cube_elements[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // top
        4, 5, 6,
        6, 7, 4,
        // back
        8, 9, 10,
        10, 11, 8,
        // bottom
        12, 13, 14,
        14, 15, 12,
        // left
        16, 17, 18,
        18, 19, 16,
        // right
        20, 21, 22,
        22, 23, 20,
    };
    obj->vertices=malloc(sizeof (GLfloat)*72);
    obj->elements=malloc(sizeof (GLushort)*36);

    memcpy(&obj->vertices[0],&cube_vertices[0],sizeof (GLfloat)*72);
    obj->vertices_size=sizeof (GLfloat)*72;

    memcpy(&obj->elements[0],&cube_elements[0],sizeof (GLushort)*36);
    obj->elements_size=sizeof (GLushort)*36;


}


void Object3d_axis(Object3d * obj,float size)
{

    GLfloat vertices[]=
    {
        1.0f*size,          0.0f,          0.0f,
        0,                  0.0f,          0.0f,
        0,                  1.0f*size,     0.0f,
        0,                  0.0f,          0.0f,
        0,                  0.0f,          1.0f*size,
        0,                  0.0f,          0.0f,
    };
    GLushort  elements[]=
    {
        0,1,2,3,4,5
    };



    obj->vertices=malloc(sizeof (GLfloat)*18);
    obj->elements=malloc(sizeof (GLushort)*6);

    memcpy(&obj->vertices[0],&vertices[0],sizeof (GLfloat)*18);
    obj->vertices_size=sizeof (GLfloat)*18;

    memcpy(&obj->elements[0],&elements[0],sizeof (GLushort)*6);
    obj->elements_size=sizeof (GLushort)*6;


    obj->style=LINE;


}


void Object3d_Rectangle(Object3d * obj,vec3 p1,vec3 p2,vec3 p3,vec3 p4)
{


    vec3 size=vec3_create(1.0,1.0,1.0);
    //uncomment
    // vec3 pos=vec3_substract(&p3,&p1);
    //vec3_scale(&pos,0.5);
    vec3 pos=vec3_create(0,0,0);





    GLfloat vertices[]=
    {
        (float)p1.x*(float)size.x-(float)pos.x  ,          (float)p1.y*(float)size.y-(float)pos.y  ,          (float)p1.z*(float)size.z-(float)pos.z  ,
        (float)p2.x*(float)size.x-(float)pos.x  ,          (float)p2.y*(float)size.y-(float)pos.y  ,          (float)p2.z*(float)size.z-(float)pos.z  ,
        (float)p3.x*(float)size.x-(float)pos.x  ,          (float)p3.y*(float)size.y-(float)pos.y  ,          (float)p3.z*(float)size.z-(float)pos.z  ,
        (float)p4.x*(float)size.x-(float)pos.x  ,          (float)p4.y*(float)size.y-(float)pos.y  ,          (float)p4.z*(float)size.z-(float)pos.z
    };
    GLushort  elements[]=
    {
        0,1,2,
        2,3,0
    };



    obj->vertices=malloc(sizeof (GLfloat)*12);
    obj->elements=malloc(sizeof (GLushort)*6);

    memcpy(&obj->vertices[0],&vertices[0],sizeof (GLfloat)*12);
    obj->vertices_size=sizeof (GLfloat)*12;

    memcpy(&obj->elements[0],&elements[0],sizeof (GLushort)*6);
    obj->elements_size=sizeof (GLushort)*6;


    obj->style=LINE_LOOP;

}



void Object3d_camera(Object3d * obj,float*projection,   Color x,vec3 rotation)
{

    float p_t[16];
    matrix_copy_float(projection,p_t,4);
    matrix_transpose_float(p_t,4,4);


    float v_t[16];
    matrix_identity_float(v_t,4);


    float pv_inv[16];
    matrix_mul_matrix_float(v_t,p_t,pv_inv,4,4,4,4);


    matrix_inverse4x4_float(pv_inv);

    double nearZ=0.001f;
    double farZ=10.0f;


    obj->color[0]=x.r;
    obj->color[1]=x.g;
    obj->color[2]=x.b;
    obj->color[3]=x.a;

    vec4 v[8];
    v[0]=(vec4_create(-1,-1,nearZ,1));
    v[1]=(vec4_create(-1,-1,farZ,1));
    v[2]=(vec4_create(-1,+1,nearZ,1));
    v[3]=(vec4_create(-1,+1,farZ,1));
    v[4]=(vec4_create(+1,+1,nearZ,1));

    v[5]=(vec4_create(+1,+1,farZ,1));
    v[6]=(vec4_create(+1,-1,nearZ,1));
    v[7]=(vec4_create(+1,-1,farZ,1));

    vec3 v_out[8];
    for (unsigned int i = 0; i < 8; ++i)
    {
        vec4 v_0=vec4_create(0,0,0,0);

        matrix4x4_mul_vec4_float(pv_inv ,&v[i],&v_0);
        vec3 out=vec3_create(v_0.vals[0]/v_0.vals[3],v_0.vals[1]/v_0.vals[3],v_0.vals[2]/v_0.vals[3]);
        v_out[i]=out;
    }

    for (unsigned int i = 0; i < 8; ++i)
    {
        vec3_scale(&v_out[i],1000);
    }



    //vec3 size=vec3_create(1,1,1);
    //auto axis= make_shared<Object>(pos,size,x,"axis") ;



    matrix_rotate4x4_z_float(obj->model, rotation.z);
    matrix_rotate4x4_y_float(obj->model, rotation.y);
    matrix_rotate4x4_x_float(obj->model, rotation.x);


    obj->style=LINE;
    GLfloat vertices[]=
    {
        (float)v_out[0].x, (float)v_out[0].y, (float)v_out[0].z,
        (float)v_out[2].x, (float)v_out[2].y, (float)v_out[2].z,
        (float)v_out[2].x, (float)v_out[2].y, (float)v_out[2].z,
        (float)v_out[4].x, (float)v_out[4].y, (float)v_out[4].z,
        (float)v_out[6].x, (float)v_out[6].y, (float)v_out[6].z,
        (float)v_out[4].x, (float)v_out[4].y, (float)v_out[4].z,

        (float)v_out[0].x, (float)v_out[0].y, (float)v_out[0].z,
        (float)v_out[6].x, (float)v_out[6].y, (float)v_out[6].z,
        (float)v_out[3].x, (float)v_out[3].y, (float)v_out[3].z,
        (float)v_out[3].x, (float)v_out[3].y, (float)v_out[3].z,

        (float)v_out[5].x, (float)v_out[5].y, (float)v_out[5].z,
        (float)v_out[7].x, (float)v_out[7].y, (float)v_out[7].z,
        (float)v_out[5].x, (float)v_out[5].y, (float)v_out[5].z,
        (float)v_out[0].x, (float)v_out[0].y, (float)v_out[0].z,
        (float)v_out[7].x, (float)v_out[7].y, (float)v_out[7].z,

        (float)v_out[1].x, (float)v_out[1].y, (float)v_out[1].z,
        (float)v_out[7].x, (float)v_out[7].y, (float)v_out[7].z,
        (float)v_out[2].x, (float)v_out[2].y, (float)v_out[2].z,
        (float)v_out[4].x, (float)v_out[4].y, (float)v_out[4].z,
        (float)v_out[1].x, (float)v_out[1].y, (float)v_out[1].z,

        (float)v_out[3].x, (float)v_out[3].y, (float)v_out[3].z,
        (float)v_out[6].x, (float)v_out[6].y, (float)v_out[6].z,
        (float)v_out[3].x, (float)v_out[3].y, (float)v_out[3].z,
        (float)v_out[5].x, (float)v_out[5].y, (float)v_out[5].z,
        (float)v_out[3].x, (float)v_out[3].y, (float)v_out[3].z,
        (float)v_out[1].x, (float)v_out[1].y, (float)v_out[1].z
    };
    GLushort  elements[]=
    {
        0,1,2,3 ,4,5 ,6,7,8,9,10,11 ,12,13,14,15,16,17,18,19,20,21,22,23,24,25
    };


    obj->vertices=malloc(sizeof (GLfloat)*84);
    obj->elements=malloc(sizeof (GLushort)*26);

    memcpy(&obj->vertices[0],&vertices[0],sizeof (GLfloat)*84);
    obj->vertices_size=sizeof (GLfloat)*84;

    memcpy(&obj->elements[0],&elements[0],sizeof (GLushort)*26);
    obj->elements_size=sizeof (GLushort)*26;


    obj->style=LINE ;

}


void Object3d_Grid(Object3d * obj,double step,vec3 size)
{
    double n=(size.z/step)*4*3;


    obj->vertices_size=sizeof (GLfloat)*n*2 ;
    obj->elements_size=(obj->vertices_size/12)+12 ;

    SDL_Log("n=%lf %lu",n ,obj->elements_size/sizeof (GLushort));
    obj->vertices=malloc(obj->vertices_size);
    obj->elements=malloc(obj->elements_size);
    obj->style=LINE;


    float p=0;
    for (  int i=0;i<=  n    ;i=i+12)
    {

        obj->vertices[i]= size.x/2.0 ;
        obj->vertices[i+1]= 0 ;
        obj->vertices[i+2]= size.z/2.0 - p* step  ;


        obj->vertices[i+3]= -size.x/2.0;
        obj->vertices[i+4]= 0;
        obj->vertices[i+5]= size.z/2.0 - p* step ;
        //

        obj->vertices[i+6]=size.x/2.0- p*step ;
        obj->vertices[i+7]= 0 ;
        obj->vertices[i+8]= size.z/2.0   ;

        obj->vertices[i+9]=size.x/2.0- p*step  ;
        obj->vertices[i+10]= 0 ;
        obj->vertices[i+11]= -size.z/2.0  ;
        p++;
    }


    for (unsigned int i=0;i<obj->elements_size/sizeof (GLushort);++i)
    {
        obj->elements[i]=i;
    }


}


void Object3d_Line(Object3d * obj,vec3 p1,vec3 p2)
{
    vec3 size=vec3_create(1.0,1.0,1.0);
    //uncomment
    // vec3 pos=vec3_substract(&p3,&p1);
    //vec3_scale(&pos,0.5);
    vec3 pos=vec3_create(0,0,0);


    GLfloat vertices[]=
    {
        (float)p1.x*(float)size.x-(float)pos.x  ,          (float)p1.y*(float)size.y-(float)pos.y  ,          (float)p1.z*(float)size.z-(float)pos.z  ,
        (float)p2.x*(float)size.x-(float)pos.x  ,          (float)p2.y*(float)size.y-(float)pos.y  ,          (float)p2.z*(float)size.z-(float)pos.z  ,

    };
    GLushort  elements[]=
    {
        0,1,2

    };



    obj->vertices=malloc(sizeof (GLfloat)*6);
    obj->elements=malloc(sizeof (GLushort)*3);

    memcpy(&obj->vertices[0],&vertices[0],sizeof (GLfloat)*6);
    obj->vertices_size=sizeof (GLfloat)*6;

    memcpy(&obj->elements[0],&elements[0],sizeof (GLushort)*3);
    obj->elements_size=sizeof (GLushort)*3;


    obj->style=LINE;

}


Color Color_create(float r, float g, float b, float a)
{
    Color c;
    c.r=r;
    c.g=g;
    c.b=b;
    c.a=a;
    return  c;
}

void Object3d_free(Object3d *c)
{

    if(c->vertices!=NULL)
    free(c->vertices);
    if(c->elements!=NULL)
    free(c->elements);
    free(c);
}

vec3 Object3d_getPos(Object3d *c)
{
vec3 v;


v.x=c->model[12];
v.y=c->model[13];
v.z=c->model[14];

return v;
}
