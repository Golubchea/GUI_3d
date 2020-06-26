#include <unproject/unproject.h>

#include <matrix/matrix.h>

UNPROJECT_API vec3 unproject(int mouse_x, int mouse_y, int width, int height ,float *projection,float *view)
{

    //http://antongerdelan.net/opengl/raycasting.html
    //range [0:width, height:0]
    float x = (2.0f * (float)mouse_x) / (float)width - 1.0f;


    float y = 1.0f - (2.0f * (float)mouse_y) / (float)height;
    float z =  1.0f;

    vec3 ray_nds = vec3_create((double) x,(double) y,(double) z);

   // printf(" range [-1:1, -1:1, -1:1] \n");
  //  vec3_print(ray_nds);

    vec4 ray_clip = vec4_create(ray_nds.x,ray_nds.y, -1.0, 1.0);

   // printf(" range [-1:1, -1:1, -1:1] \n");
  //  vec4_print(ray_clip);
    float proj_inv[16];
    matrix_copy_float(&projection[0],&proj_inv[0],4);

    //
    matrix_transpose_float( &proj_inv[0],4,4);
    //

    matrix_inverse4x4_float(&proj_inv[0]);

    vec4 ray_eye = vec4_create(0,0,0,0);

    matrix4x4_mul_vec4_float(&proj_inv[0],&ray_clip,&ray_eye);

    ray_eye.vals[2]=-1.0;
    ray_eye.vals[3]=0.0;

    float view_inv[16];
    matrix_copy_float(&view[0],&view_inv[0],4);
    //
    matrix_transpose_float( &view_inv[0],4,4);
    //
    matrix_inverse4x4_float(&view_inv[0]);


    vec4 ray_world ;
    matrix4x4_mul_vec4_float(view_inv,&ray_eye,&ray_world);
    vec3 ray_world3=vec3_create( ray_world.vals[0], ray_world.vals[1], ray_world.vals[2]);

    vec3 ray_world3_1 =vec3_normalize(&ray_world3);
    vec3_print(ray_world3_1);
    return ray_world3_1;
}


UNPROJECT_API vec3 unproject2(int mouse_x, int mouse_y, int width, int height ,float *projection,float *view)
{

    //http://antongerdelan.net/opengl/raycasting.html
    //range [0:width, height:0]
    float x = (2.0f * (float)mouse_x) / (float)width - 1.0f;


    float y = 1.0f - (2.0f * (float)mouse_y) / (float)height;
    float z =  1.0f;

    vec3 ray_nds = vec3_create((double) x,(double) y,(double) z);

    printf(" range [-1:1, -1:1, -1:1] \n");
    vec3_print(ray_nds);

    vec4 ray_clip = vec4_create(ray_nds.x,ray_nds.y, -1.0, 1.0);

    printf(" range [-1:1, -1:1, -1:1] \n");
    vec4_print(ray_clip);
    float proj_inv[16];
    matrix_copy_float(&projection[0],&proj_inv[0],4);
    matrix_inverse4x4_float(&proj_inv[0]);

    vec4 ray_eye = vec4_create(0,0,0,0);

    matrix4x4_mul_vec4_float(&proj_inv[0],&ray_clip,&ray_eye);

    ray_eye.vals[2]=-1.0;
    ray_eye.vals[3]=0.0;

    float view_inv[16];
    matrix_copy_float(&view[0],&view_inv[0],4);
    matrix_inverse4x4_float(&view_inv[0]);


    vec4 ray_world ;
    matrix4x4_mul_vec4_float(view_inv,&ray_eye,&ray_world);
    vec3 ray_world3=vec3_create( -ray_world.vals[0], ray_world.vals[1], -ray_world.vals[2]);

    vec3 ray_world3_1 =vec3_normalize(&ray_world3);
    vec3_print(ray_world3_1);
    return ray_world3_1;
}




