#pragma once
#include "GUI/objects/Object3d.h"
#include "GUI/camera.h"
#include "GUI/canvas3d.h"
#include "GUI/canvas.h"
#include <unproject/unproject.h>
#include <geometry/intersect_line_plane.h>

#include "GUI/Button_controller.h"
#include "GUI/button/button.h"


#include <camera_solver/intrisic_camera_solver.h>



#include <vec2.h>
#include <vec3.h>
vec3 unproj(vec2 mou,Canvas * canvas,Camera * camera, Canvas3d* canvas3d)
{


    vec2 win=vec2_create(canvas->width,canvas->height);



    vec3 pos=vec3_create(camera->Position.x,camera->Position.y,camera->Position.z);

    vec3  dir=unproject(mou.x,mou.y, win.x,win.y ,&canvas3d->projection[0],&canvas3d->view[0]);

    // Vec3_t dir=Vec3_t_create(cam->Front.x,cam->Front.y,cam->Front.z);
    vec3 plane_normal=vec3_create(0,1,0);
    vec3 pos_on_plane=intersect_line_plane(pos,dir,plane_normal,0);

    return pos_on_plane;
}


void unproj_display(void* user_data,void* user_data2,void* user_data3,void* user_data4)
{
    Canvas  *canvas=(Canvas *)user_data;

    Camera  *camera=( Camera  *)user_data2;
    Canvas3d  *canvas3d=(Canvas3d *)user_data3;

    Rect3d p_out;
    p_out.ra=unproj(vec2_create(0,0),canvas,camera, canvas3d);
    p_out.rb=unproj(vec2_create( canvas->width,0), canvas, camera, canvas3d);
    p_out.rc=unproj(vec2_create( canvas->width, canvas->height), canvas, camera, canvas3d);
    p_out.rd=unproj(vec2_create(0, canvas->height), canvas, camera, canvas3d);

    static int i=0;
    char  unproj[10];
    sprintf(unproj,"unproj%d",i);
    i++;



    Object3d *obj=Object3d_create(vec3_create(0.0,0.0,0.0),Color_create(1.0,0.59,0.12,0.7));
    Canvas3d_add_object( canvas3d,obj,unproj );

    Object3d_Rectangle(obj,p_out.ra,p_out.rb,p_out.rc,p_out.rd);
    //Object3d_Cube(obj,vec3_create(0.2,0.2,0.2));
    obj->style=LINE_LOOP;


}

void Menu_unproject_create(Canvas *r,Camera *cam ,Button_controller *b,Canvas3d *c3d)
{
    Button *b5=Button_create("Unproject",29,150,3,25,25,RGBA(0,255,0,255),unproj_display );
    Button_setIcon(b5,"\uf1ab");
    b5->data=(void*)r;
    b5->data2=(void*)cam;
    b5->data3=(void*)c3d;


    b5->drawBorder=false;
    b5->iconOnly=true;
    Button_controller_add(b,b5);
}
