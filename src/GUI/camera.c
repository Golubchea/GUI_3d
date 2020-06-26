#include "camera.h"
#include <vec3.h>
#include <vec2.h>
#include <matrix/matrix.h>
#define _USE_MATH_DEFINES
#include <math.h>





void Camera_lookAt(vec3 *eye, vec3 *target, vec3 *upDir, float *matrix)
{
    // compute the forward vector from target to eye
    vec3 forward = vec3_substract(  eye ,  target);
    //norm_vector(forward);                 // make unit length

    vec3_normalize(&forward);

    // compute the left vector
    vec3 left = vec3_cross( upDir,&forward); // cross product
    vec3_normalize(&left);
    // recompute the orthonormal up vector
    vec3 up = vec3_cross(&forward,&left) ;    // cross product
    //    vec3_print(forward);
    //    vec3_print(left);
    //    vec3_print(up);
    // init 4x4 matrix

    matrix_identity_float(matrix,4);

    // set rotation part, inverse rotation matrix: M^-1 = M^T for Euclidean transform
    matrix[0] = left.x;
    matrix[4] = left.y;
    matrix[8] = left.z;
    matrix[1] = up.x;
    matrix[5] = up.y;
    matrix[9] = up.z;
    matrix[2] = forward.x;
    matrix[6] = forward.y;
    matrix[10]= forward.z;

    // set translation part
    matrix[12]= -left.x * eye->x - left.y * eye->y - left.z * eye->z;
    matrix[13]= -up.x * eye->x - up.y * eye->y - up.z * eye->z;
    matrix[14]= -forward.x * eye->x - forward.y * eye->y - forward.z * eye->z;

}




void updateCameraVectors(Camera *c)
{
    // Calculate the new Front vector
    vec3 front;

    front.x = cos(rads(c->Yaw)) * cos(rads(c->Pitch));
    front.y = sin(rads(c->Pitch));
    front.z = sin(rads(c->Yaw)) * cos(rads(c->Pitch));
    vec3_normalize(&front);

    c->Front = front;
    // Also re-calculate the Right and Up vector
    vec3 t =vec3_cross(&c->Front,& c->WorldUp);
    c->Right = vec3_normalize(&t); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    vec3 t2 =vec3_cross(&c->Right,& c->Front);
    c->Up    = vec3_normalize(&t2   );

}

void NormalizePlane(float frustum[4])
{
    // Вычисляем величину нормали плоскости (точку A B C)
    // Помните, что (A, B, C) плоскости - то же самое, что (X, Y, Z) для нормали.
    // Чтобы вычислить величину, используем формулу: sqrt(x^2 + y^2 + z^2)
    float magnitude = (float)sqrt( frustum[0] * frustum[0] +
            frustum[1] * frustum[1] +
            frustum[2] * frustum[2] );

    // Затем делим значения плоскости на её величину.
    // После этого с плоскостью будет легче работать.
    frustum [0] /= magnitude;
    frustum [1] /= magnitude;
    frustum [2] /= magnitude;
    frustum [3] /= magnitude;
}


void Camera_extract_planes_from_projmat( Camera*  c,        float* proj     ,const float *view     )
{
    float clip[16];


    float proj_mat[16];
    float view_mat[16];
    matrix_copy_float(view ,view_mat,4);
    matrix_copy_float(proj ,proj_mat,4);

    matrix_transpose_float(proj_mat,4,4);
    //matrix_transpose_float(view_mat,4,4);

    matrix_mul_matrix_float(view_mat,proj_mat,clip,4,4,4,4);

    c->right[0]      = clip[12] - clip[ 0];
    c->right[1]      = clip[13] - clip[ 1];
    c->right[2]      = clip[14] - clip[ 2];
    c->right[3]      = clip[15] - clip[3 ];


    c->left[0]      = clip[ 12] + clip[ 0];
    c->left[1]      = clip[ 13] + clip[ 1];
    c->left[2]      = clip[14] + clip[ 2];
    c->left[3]      = clip[15] + clip[ 3];

    //
    c->top[0]      = clip[12] - clip[4];
    c->top[1]      = clip[13] - clip[5];
    c->top[2]      = clip[14] - clip[6];
    c->top[3]      = clip[15] - clip[7];


    c->bottom[0]      = clip[12] + clip[4];
    c->bottom[1]      = clip[13] + clip[5];
    c->bottom[2]      = clip[14] + clip[6];
    c->bottom[3]      = clip[15] + clip[7];




    c->near[0]      = clip[12] - clip[8];
    c->near[1]      = clip[13] - clip[9];
    c->near[2]      = clip[14] - clip[10];
    c->near[3]      = clip[15] - clip[11];


    c->far[0]      = clip[12] + clip[8];
    c->far[1]      = clip[13] + clip[9];
    c->far[2]      = clip[14] + clip[10];
    c->far[3]      = clip[15] + clip[11];
    NormalizePlane(c->right);
    NormalizePlane(c->left);
    NormalizePlane(c->top);
    NormalizePlane(c->bottom);
    NormalizePlane(c->near);
    NormalizePlane(c->far);

}


void Camera_SetViewMatrix(Camera *c,float *proj,float *view)
{
    vec3 pf=vec3_add( &c->Position , &c->Front);
    Camera_lookAt(&c->Position,&pf, &c->Up,view);
    // matrix_print_float(proj,4,4,"proj\n");
    // matrix_print_float(proj,4,4,"view\n");

    if(c->firstMouse==true)
    {
        Camera_extract_planes_from_projmat( c,  proj, view);
        // extract_planes_from_projmat3(   proj);
    }
}






void Camera_ProcessKeyboard(Camera *c,enum Camera_Movement direction, float deltaTime)
{
    c->firstMouse = true;

    float velocity = c->MovementSpeed * deltaTime;
    if (direction == FORWARD)
    {
        vec3 f_copy=vec3_copy(&c->Front);
        vec3_scale(&f_copy,velocity);
        c->Position =vec3_add(&c->Position,&f_copy);
    }
    if (direction == BACKWARD)
    {
        vec3 f_copy=vec3_copy(&c->Front);
        vec3_scale(&f_copy,velocity);
        c->Position =vec3_substract(&c->Position,&f_copy);
    }
    if (direction == LEFT)
    {
        vec3 f_copy=vec3_copy(&c->Right);
        vec3_scale(&f_copy,velocity);
        c->Position =vec3_substract(&c->Position,&f_copy);
    }

    if (direction == RIGHT)
    {
        vec3 f_copy=vec3_copy(&c->Right);
        vec3_scale(&f_copy,velocity);
        c->Position =vec3_add(&c->Position,&f_copy);
    }
}






void Camera_processCameraMov(Camera* c,SDL_Event *ev)
{

    if (ev ->type == SDL_KEYDOWN && ev ->key.keysym.sym == SDLK_w)
    {
        //SDL_Log("moved w");
        Camera_ProcessKeyboard(c, FORWARD,c->speed);
    }
    if (ev ->type == SDL_KEYDOWN && ev ->key.keysym.sym == SDLK_s)
    {
        //SDL_Log("moved s");
        Camera_ProcessKeyboard( c,BACKWARD,c->speed);
    }
    if (ev ->type == SDL_KEYDOWN && ev ->key.keysym.sym == SDLK_a)
    {
        //SDL_Log("moved a");
        Camera_ProcessKeyboard(c, LEFT,c->speed);
    }
    if (ev->type == SDL_KEYDOWN && ev ->key.keysym.sym == SDLK_d)
    {
        //SDL_Log("moved d");
        Camera_ProcessKeyboard(c, RIGHT,c->speed);
    }

    if (ev->type == SDL_KEYDOWN && ev ->key.keysym.sym == SDLK_k)
    {
        c->offset.x=c->offset.x-0.5;
        vec3 f_copy=vec3_create(c->offset.x,0,0);

        c->Position =vec3_add(&c->Position,&f_copy);
    }

    if (ev->type == SDL_KEYDOWN && ev ->key.keysym.sym == SDLK_i)
    {
        c->offset.x=c->offset.x+0.5;
        vec3 f_copy=vec3_create(c->offset.x,0,0);

        c->Position =vec3_add(&c->Position,&f_copy);
    }

    if (ev->type == SDL_KEYDOWN && ev ->key.keysym.sym == SDLK_j)
    {
        c->offset.y=c->offset.y-0.5;
        vec3 f_copy=vec3_create(0,0,c->offset.y);

        c->Position =vec3_add(&c->Position,&f_copy);
    }

    if (ev->type == SDL_KEYDOWN && ev ->key.keysym.sym == SDLK_l)
    {
        c->offset.y=c->offset.y+0.5;
        vec3 f_copy=vec3_create(0,0,c->offset.y);

        c->Position =vec3_add(&c->Position,&f_copy);
    }



}


void Camera_ProcessMouseMovement(Camera*c, float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= c->MouseSensitivity;
     yoffset *= c->MouseSensitivity;

     c->lastX= xoffset;
     c->lastY= yoffset;

    c->Yaw   += xoffset;
    c->Pitch -= yoffset;

    //SDL_Log("process mouse move");

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
//    if (constrainPitch)
//    {
//        if (c->Pitch > 89.9f)
//            c->Pitch = 89.9f;
//        if (c->Pitch < -89.9f)
//            c->Pitch = -89.9f;
//    }




    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors(c);
}


void Camera_ProcessMouseScroll(Camera *c,float yoffset)
{
    if (c->Zoom >= 1.0f && c->Zoom <= 45.0f)
        c->Zoom -= yoffset;
    if (c->Zoom <= 1.0f)
        c->Zoom = 1.0f;
    if (c->Zoom >= 45.0f)
        c->Zoom = 45.0f;
}


void Camera_listen(Camera *c,SDL_Event *ev)
{



    if(ev->type==SDL_MOUSEBUTTONDOWN &&ev->button.button==SDL_BUTTON_MIDDLE)
    {
        c-> firstMouse = true;

    }


    if(ev->type==SDL_MOUSEMOTION&&ev->button.button==SDL_BUTTON_MIDDLE)
    {


//        float xpos=ev->motion.x;
//        float ypos=ev->motion.y;

//        if (c->firstMouse)
//        {
//            c->lastX = xpos;
//            c->lastY = ypos;
//            c->firstMouse = false;
//        }

//        c->xoffset = xpos - c->lastX;
//        c->yoffset = c->lastY - ypos; // reversed since y-coordinates go from bottom to top

//        c->lastX = xpos;
//        c->lastY = ypos;




        Camera_ProcessMouseMovement(c, ev->motion.xrel , ev->motion.yrel ,false);
    }
    if(ev->type==SDL_MOUSEWHEEL)
    {

    }

    //        if(ev ->type == SDL_KEYUP &&ev->button.button==SDL_BUTTON_MIDDLE)
    //        {
    //            float xpos=ev->motion.x;
    //            float ypos=ev->motion.y;
    //            lastX -= xpos;
    //            lastY -= ypos;
    //        }

}

float getFov(float *projection)
{
    return 2.0*atan( 1.0/projection[4+1] ) * 180.0 / M_PI;
}

float getAspect(float *projection)
{
    return projection[5]/projection[0];
}

void getViewOfObject(Camera *c,float * Object_model_matrix)
{



    // updateCameraVectors();
    //    Yaw=90;
    //    Pitch=0;
    float* t1=&Object_model_matrix[12];

    float t[16];
    matrix_copy_float(&Object_model_matrix[0],t,4);
    matrix_transpose_float(t,4,4);
    matrix_inverse4x4_float(t);






    c->Position=vec3_create(*t1, *(t1+1), *(t1+2)   );

    c->Front=vec3_create( -t[8],  -t[9], -t[10]);

    vec3 upVector=vec3_create( 0 ,1,0);

    //vec3 up1=vec3_create(0.0,1.0,0.0);
    vec3 right=vec3_cross(&c->Front,&upVector);

    c->Right=vec3_normalize(&right);

    vec3 up=vec3_cross(&right,&c->Front);
    c->Up=vec3_normalize(&up);




    vec3 eye1=vec3_create(Object_model_matrix[12],Object_model_matrix[13], Object_model_matrix[14]);
    vec3 front=vec3_create(Object_model_matrix[8], Object_model_matrix[9], Object_model_matrix[10]);
    Camera_lookAt( &eye1, &front, &upVector,&t[0]);

    c->firstMouse=true;
}

void getViewOfObjectRoll(Camera *c,float * Object_model_matrix,float roll_rads)
{



    // updateCameraVectors();
    //    Yaw=90;
    //    Pitch=0;
    float* t1=&Object_model_matrix[12];

    float t[16];
    matrix_copy_float(&Object_model_matrix[0],t,4);
    matrix_transpose_float(t,4,4);
    matrix_inverse4x4_float(t);






    c->Position=vec3_create(*t1, *(t1+1), *(t1+2)   );

    c->Front=vec3_create( -t[8],  -t[9], -t[10]);

    vec3 upVector=vec3_create(cos(roll_rads),sin(roll_rads),0);

    //vec3 up1=vec3_create(0.0,1.0,0.0);
    vec3 right=vec3_cross(&c->Front,&upVector);

    c->Right=vec3_normalize(&right);

    vec3 up=vec3_cross(&right,&c->Front);
    c->Up=vec3_normalize(&up);




    vec3 eye1=vec3_create(Object_model_matrix[12],Object_model_matrix[13], Object_model_matrix[14]);
    vec3 front=vec3_create(Object_model_matrix[8], Object_model_matrix[9], Object_model_matrix[10]);
    Camera_lookAt( &eye1, &front, &upVector,&t[0]);

    c->firstMouse=true;
}

Camera *Camera_create(vec3 position,vec2 window_size, vec3 up,float yaw,float pitch)
{
    Camera* c=(Camera*)malloc(sizeof(Camera));
    c->offset=vec2_create(0,0);
    c->Position=position;
    c->WorldUp=up;
    c->Yaw=yaw;
    c->Pitch=pitch;
    c->lastX=(float)window_size.x/2.0f;
    c->lastY=(float)window_size.y/2.0f;
    c->Front=vec3_create(0.0f, 0.0f, -1.0f);
    c->speed=0.2f;

    c->MovementSpeed=2.5f;
    c->MouseSensitivity=0.1f;
    c->Zoom=45.0f;

    updateCameraVectors(c);
    return c;
}

void Camera_free(Camera *c)
{
    free(c);
}
