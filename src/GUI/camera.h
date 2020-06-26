#ifndef CAMERA_H
#define CAMERA_H




#ifdef __cplusplus
extern "C" {
#endif
#include <vec2.h>
#include <vec3.h>
#include "SDL.h"

#if OKPLATFORM_EMSCRIPTEN
#define GL_GLEXT_PROTOTYPES 1
//#include <SDL_opengles2.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "canvas/glad/glad.h"
#include "canvas/glad/khrplatform.h"
#endif


#ifdef CANVAS3D_STATIC
#define CANVAS3D_API static
#else
#define CANVAS3D_API extern
#endif
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};



typedef struct Camera
{
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;

    vec2 offset;

    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float speed     ;
    float sphere_radius ;

    float lastX ;
    float lastY ;
    bool firstMouse  ;

    float xoffset;
    float yoffset;

    float left[4];
    float right[4];
    float top[4];
    float bottom[4];

    float near[4];
    float far[4];

    vec3 cam_degrees ;
}Camera;



CANVAS3D_API void Camera_lookAt(vec3 *eye, vec3 *target, vec3 *upDir, float *matrix);

CANVAS3D_API void getViewOfObject(Camera *c,float * Object_model_matrix);
CANVAS3D_API void getViewOfObjectRoll(Camera *c,float * Object_model_matrix,float roll_rads);
CANVAS3D_API float getFov(float *projection);
CANVAS3D_API float getAspect(float *projection);


//public functions
CANVAS3D_API Camera *Camera_create(vec3 position,vec2 window_size, vec3 up,float yaw,float pitch);
CANVAS3D_API void Camera_SetViewMatrix(Camera *c,float *proj,float *view);
//processing
CANVAS3D_API void Camera_listen(Camera *c,SDL_Event *ev);
CANVAS3D_API void Camera_ProcessMouseMovement(Camera*c, float xoffset, float yoffset, bool constrainPitch);
CANVAS3D_API void Camera_processCameraMov(Camera* c,SDL_Event *ev);
CANVAS3D_API void Camera_ProcessMouseScroll(Camera *c,float yoffset);
CANVAS3D_API void Camera_free(Camera *c);
CANVAS3D_API void updateCameraVectors(Camera *c);



#ifdef __cplusplus
}
#endif


#endif // ifndef












//if(vector_is_empty(&ctx.reconstructor->answers)!=1)
//{
//    Reconstructor_answer * t =(Reconstructor_answer *)vector_back(&ctx.reconstructor->answers);
//    // matrix_print_float1(t->camera->model,4,4,"CAMERA BEFORE:");
//    matrix_rotate4x4_around_axis_float( t->rectangle->model ,vec3_create(0,1,0),90 );




//    matrix_identityE(t->camera->model);



//    float pivot[3];



//    pivot[0]= - (float)t->pos.x *1.0f;
//    pivot[1]= - (float)t->pos.y*1.0f;
//    pivot[2]= - (float)t->pos.z*1.0f ;


//    matrix_translate4x4_float(t->camera->model,&pivot[0]);


//    //            //
//    //           //

//    float r=sqrt(t->pos.x*t->pos.x+t->pos.z*t->pos.z);
//    float x=r*cos(rads(t->rot_d.y)+M_PI);
//    float z=r*sin(rads(t->rot_d.y)+M_PI);

//    vec2 vnn= vec2_create(x,z);
//    vec2 v1= vec2_create(1,0);
//    vec2 v2=vec2_normalize(&vnn);



//    //x1*y2-y1*x2,x1*x2+y1*y2

//    double angle=atan2(v1.x*v2.x+v1.y*v2.y,v1.x*v2.y-v1.y*v2.x);
//    angle=degrees(angle);
//    //SDL_Log("angle: %lf",angle);
//    matrix_rotate4x4_around_axis_float(t->camera->model ,vec3_create(0,0,1),t->rot_d.z);
//    matrix_rotate4x4_around_axis_float(t->camera->model ,vec3_create(0,1,0),angle );

//    matrix_rotate4x4_around_axis_float(t->camera->model ,vec3_create(1,0,0),t->rot_d.x);





//    pivot[0]=  x;
//    pivot[1]=  0;
//    pivot[2]= z;


//    matrix_SetTranslation(t->camera->model,x,t->pos.y,z);



//    getViewOfObjectRoll(ctx.camera,t->camera->model, M_PI/2.0 );


//    ctx.canvas3d->kx=  compute_unproject_koefficients_x(ctx.canvas3d->projection,ctx.canvas3d->view);
//    ctx.canvas3d->ky=  compute_unproject_koefficients_y(ctx.canvas3d->projection,ctx.canvas3d->view);

//    //matrix_print_float1(t->camera->model,4,4,"CAMERA:");

//}
