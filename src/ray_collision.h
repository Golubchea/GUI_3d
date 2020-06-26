#pragma once

#include "gizmo/tiny-gizmo.hpp"
extern "C"
{
#include "Main_all.h"

};

using namespace std;
using namespace tinygizmo;

typedef struct gizmo_t{
    gizmo_t(Canvas3d *c)
    {
        Object3d *grid= Object3d_create(vec3_create(0,0,0),Color_create(1,0,0,0.3));


        gizmo3d=grid;
        map_set(&c->objects, "gizmo11", grid);

        Object3d_Cube(grid,vec3_create(4,4,4));
        grid->style=STYLE::LINE_LOOP;
        free(grid->elements);
        free(grid->vertices);
        grid->elements=NULL;
        grid->vertices=NULL;
        r.position.x=0;
    }
    ~gizmo_t()
    {
        gizmo3d->elements=NULL;
        gizmo3d->vertices=NULL;
    }
    Object3d * gizmo3d;
    gizmo_application_state  state;
    gizmo_context  ctx;

    std::vector<float>data_v;
    std::vector<unsigned short>data_i;
    rigid_transform r;
}gizmo_t;

extern void convert_matrix(minalg::float4x4 *src,float *dst)
{
    dst[0]=src->x.x;
    dst[1]=src->x.y;
    dst[2]=src->x.z;
    dst[3]=src->x.w;

    dst[4]=src->y.x;
    dst[5]=src->y.y;
    dst[6]=src->y.z;
    dst[7]=src->y.w;

    dst[8]=src-> z.x;
    dst[9]=src-> z.y;
    dst[10]=src->z.z;
    dst[11]=src->z.w;

    dst[12]=src->w.x;
    dst[13]=src->w.y;
    dst[14]=src->w.z;
    dst[15]=src->w.w;
}




struct ray { vec3 origin; vec3 direction; };


typedef struct RayHitInfo {
    bool hit;               // Did the ray hit something?
    float distance;         // Distance to nearest hit
   vec3 position;       // Position of nearest hit
    vec3 normal;         // Surface normal of hit
} RayHitInfo;


typedef struct vec3f
{
     GLfloat x;
     GLfloat y;
     GLfloat z;

}vec3f;










extern void matrix_print_float1(float* c,unsigned int M, unsigned int N, char* name)
{
    SDL_Log("%s matrix%ux%u :\n",name,M,N);
    for(unsigned int i = 0; i <M; i++)
    {
        SDL_Log("(%g %g %g %g )",c[i*N+0],c[i*N+1],c[i*N+2],c[i*N+3]);
    }
}
extern void vec3_print1(vec3 s,std::string ss)
{
    SDL_Log(" %s= (%lf,%lf,%lf) \n",ss.c_str(),s.x,s.y,s.z);
}
extern void vec4_print1(vec4 s,std::string ss)
{
    SDL_Log("\n %s= (%lf,%lf,%lf,%lf) \n",ss.c_str(),s.vals[0],s.vals[1],s.vals[2],s.vals[3]);
}

extern ray get_ray_from_pixel (  vec2   pixel, vec2   viewport, vec3 camera_pos,float *proj,float *view)
{

    const float x = 2 * (pixel.x - 0) / viewport.x - 1;
    const float y = 1 - 2 * (pixel.y - 0) / viewport.y;

    float viewproj[16];


    matrix_mul_matrix_float(&view[0],&proj[0],&viewproj[0],4,4,4,4);//may need to transpose

    matrix_inverse4x4_float( &viewproj[0]);

    vec4 pp1=vec4_create(x, y, -1, 1);
    vec4 pp1res;


    matrix4x4_vec4(&viewproj[0],&pp1, &pp1res);
    vec4 pp2res;
    vec4 pp2=vec4_create(x, y, 1, 1);



    matrix4x4_vec4(&viewproj[0],&pp2, &pp2res);


    vec3 p1=vec3_create( pp1res.vals[0]*pp2res.vals[3],
            pp1res.vals[1]*pp2res.vals[3],
            pp1res.vals[2]*pp2res.vals[3]);

    vec3 p2=vec3_create( pp2res.vals[0]*pp1res.vals[3],
            pp2res.vals[1]*pp1res.vals[3],
            pp2res.vals[2]*pp1res.vals[3]);

    vec3 p_res =vec3_substract(&p2,&p1);

    return{ camera_pos, p_res };
}



extern RayHitInfo GetCollisionRayTriangle(ray ray, vec3 p1, vec3 p2, vec3 p3)
{
    #define EPSILON 0.000001        // A small number

    vec3 edge1, edge2;
    vec3 p, q, tv;
    float det, invDet, u, v, t;
    RayHitInfo result = {0};

    // Find vectors for two edges sharing V1
    edge1 = vec3_substract(&p2, &p1);
    edge2 = vec3_substract(&p3, &p1);

    // Begin calculating determinant - also used to calculate u parameter
    p = vec3_cross(&ray.direction, &edge2);

    // If determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
    det = vec3_dot(&edge1, &p);

    // Avoid culling!
    if ((det > -EPSILON) && (det < EPSILON)) return result;

    invDet = 1.0f/det;

    // Calculate distance from V1 to ray origin
    tv = vec3_substract(&ray.origin, &p1);

    // Calculate u parameter and test bound
    u = vec3_dot(&tv, &p)*invDet;

    // The intersection lies outside of the triangle
    if ((u < 0.0f) || (u > 1.0f)) return result;

    // Prepare to test v parameter
    q = vec3_cross(&tv, &edge1);

    // Calculate V parameter and test bound
    v = vec3_dot(&ray.direction, &q)*invDet;

    // The intersection lies outside of the triangle
    if ((v < 0.0f) || ((u + v) > 1.0f)) return result;

    t = vec3_dot(&edge2, &q)*invDet;

    if (t > EPSILON)
    {
        // Ray hit, get hit point and normal
        result.hit = true;
        result.distance = t;
        result.hit = true;
        vec3 tt=vec3_cross(&edge1, &edge2);
        result.normal = vec3_normalize(&tt);
        vec3_scale(&ray.direction, t);

        result.position = vec3_add(&ray.origin, &ray.direction);
    }

    return result;
}



extern vec3f vec3_Transform(vec3f v, float* mat)
{
    vec3f result = { 0 };
    float x = v.x;
    float y = v.y;
    float z = v.z;

    result.x = mat[0]*x + mat[4]*y + mat[8]*z + mat[12];
    result.y = mat[1]*x + mat[5]*y + mat[9]*z + mat[13];
    result.z = mat[2]*x + mat[6]*y + mat[10]*z + mat[14];

    return result;
}


extern RayHitInfo GetCollisionRayModel(ray ray, Object3d* model)
{
    RayHitInfo result = { 0 };


            // model->mesh.triangleCount may not be set, vertexCount is more reliable
            int triangleCount =  model->vertices_size/3;

            // Test against all triangles in mesh
            for (int i = 0; i < triangleCount; i++)
            {
                vec3f a, b, c;
                vec3f *vertdata =(vec3f *) model->vertices;

                if (model->elements_size>0)
                {
                    a = vertdata[model->elements[i*3 + 0]];
                    b = vertdata[model->elements[i*3 + 1]];
                    c = vertdata[model->elements[i*3 + 2]];
                }
                else
                {
                    a = vertdata[i*3 + 0];
                    b = vertdata[i*3 + 1];
                    c = vertdata[i*3 + 2];
                }

                a = vec3_Transform(a, model->model);
                b = vec3_Transform(b, model->model);
                c = vec3_Transform(c, model->model);

                vec3 aa;
                aa.x=a.x;
                aa.y=a.y;
                aa.z=a.z;

                vec3 bb;
                bb.x=b.x;
                bb.y=b.y;
                bb.z=b.z;

                vec3 cc;
                cc.x=c.x;
                cc.y=c.y;
                cc.z=c.z;

                RayHitInfo triHitInfo = GetCollisionRayTriangle(ray, aa, bb, cc);

                if (triHitInfo.hit)
                {
                    // Save the closest hit triangle
                    if ((!result.hit) || (result.distance > triHitInfo.distance)) result = triHitInfo;
                }
            }



    return result;
}



vec3 closestPointOnLine( vec3 *point,ray *r)
{
    vec3 s=vec3_substract(&r->origin,&r->direction);
    double LineLength= vec3_distance(&s);

    vec3 Line_direction=vec3_substract(&r->direction,&r->origin);
    vec3_scale(&Line_direction,1.0/LineLength);

    vec3 Vector=vec3_substract(point,&r->origin);


    double distance=vec3_dot(&Vector,&Line_direction);

    if(distance<=0)
    {
        return r->origin;
    }
    if(distance>=LineLength)
    {
        return r->direction;
    }


    vec3_scale( &Line_direction, distance);
    vec3 a=vec3_add(&r->origin,&Line_direction);

    return a;

}

minalg::float4 get_orientation(Camera * c)
{ return qmul(rotation_quat(minalg::float3(0, 1, 0), c->Yaw)
              , rotation_quat(minalg::float3(1, 0, 0), c->Pitch));
}


Object3d* mouseButtonDown(mctx *ctx)
{
    const char *key;
    map_iter_t iter = map_iter(&ctx->canvas3d->objects);



    ray rayDir = get_ray_from_pixel(vec2_create(ctx->mouse->x,ctx->mouse->y),
                                    vec2_create(ctx->canvas->width,ctx->canvas->height),
                                    ctx->camera->Position,
                                    ctx->canvas3d->projection,
                                    ctx->canvas3d->view) ;




    const char * closest=NULL;
    float closest_distance = 1.0 / 0.0; // infinity
    while ((key = map_next(&ctx->canvas3d->objects, &iter)))
    {

        Object3d *b =Canvas3d_GetObject( ctx->canvas3d,key);
        vec3 pos= Object3d_getPos(b);


        vec3 cpol=closestPointOnLine(&pos,&rayDir);
        float  dtol= vec3_distance2(&pos,&cpol);
        float distance =vec3_distance2(&pos,&rayDir.origin);



        if(dtol < 0.2 * sqrtf(3.0) && distance < closest_distance)
        {
            closest= key;

            closest_distance = distance;
        }
    }

    SDL_Log( "selected:  %s\n", closest) ;
    if(closest!=NULL)
    {
    Object3d *rr =Canvas3d_GetObject( ctx->canvas3d,closest);
    return rr;
    }
    else
    {
        return NULL;
    }
}
