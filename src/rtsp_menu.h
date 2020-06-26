#pragma once

#include "Main_all.h"

#include "json.hpp"
#include <fstream>
using jsonf = nlohmann::json;
void save(void* user_data,void* user_data2,void* user_data3,void* user_data4)
{
    mctx *ctx=( mctx *)user_data;


    Camera *camera=ctx->camera;


    jsonf jsonfile;








    jsonfile["OFFSET"]={camera->offset.x,camera->offset.y};
    jsonfile["OFFSET_ANGLE"]={0};

    //if(vector_is_empty(&reconstructor->answers)!=1)
    //{
    //    Reconstructor_answer * r =(Reconstructor_answer *)vector_back(&reconstructor->answers);
    //    jsonfile["CAMERA_ROTATION_DEGREES"] =  { r->rot_d.x,r->rot_d.y,r->rot_d.z};
    //    jsonfile["CAMERA_POSITION"] =  { r->pos.x,r->pos.y,r->pos.z};
    //    jsonfile["DESCRIPTION"] =   "Углы Эйлера, координатная ось opengl x-> y| z(.)";

    //}

    vec3 cam_pos= camera->Position;
    vec3 cam_rot=vec3_create( camera->Pitch, camera->Yaw,0)  ;
    jsonfile["CAM_POS_LAST"]={ cam_pos.x,cam_pos.y,cam_pos.z};
    jsonfile["CAM_ROT_LAST"]={ cam_rot.x,cam_rot.y,cam_rot.z};

    std::ofstream file("key.json");
    file << jsonfile;
    file.close();

}
void load(void* user_data,void* user_data2,void* user_data3,void* user_data4)
{
    mctx *ctx=( mctx *)user_data;
    // Menu_reconstructor * rmenu=ctx->menu_reconstructor ;
    Camera *camera=ctx->camera;
    std::ifstream ifs("key.json");
    jsonf jsonfile = jsonf::parse(ifs);






    camera->Position=vec3_create(jsonfile["CAM_POS_LAST"][0],jsonfile["CAM_POS_LAST"][1],jsonfile["CAM_POS_LAST"][2]);
    camera->Pitch=jsonfile["CAM_ROT_LAST"][0];
    camera->Yaw=(double)jsonfile["CAM_ROT_LAST"][1];
    updateCameraVectors(camera);
    Camera_SetViewMatrix(camera,ctx->canvas3d->projection,ctx->canvas3d->view);

    ifs.close();
}





extern void textInput(void* u,void* user_data2,void* user_data3,void* user_data4)
{
    mctx *ctx=( mctx *)u;
    SDL_Log("text input");
    SDL_StartTextInput();
    ctx->isText=false;
}

extern void textInputStop(void*u,void* user_data2,void* user_data3,void* user_data4)
{
    mctx *ctx=( mctx *)u;
    SDL_Log("text input");
    SDL_StopTextInput();
    ctx->isText=true;
}


extern void rtsp(void* s,void* n,void* u,void* user_data4)
{

}

extern void streamVC(void* u,void* user_data2,void* user_data3,void* user_data4)
{
    mctx *ctx=( mctx *)u;
    static bool t=false;
    if(t==false)
    {
        Button *text1=Button_create("text1",20,85+45,40,555,25,RGBA(0,255,0,255),textInput );
        text1->drawBorder=true;
        text1->textIsIcon=1;
        memset(text1->icon,'\0',128);
         strcat(text1->icon," utf-8 текст ");


        text1->data=ctx;
        Button_controller_add(ctx->button_controller,text1);

        Button *text2=Button_create("OK",25,85+45+555,40,80,25,RGBA(0,255,0,255),rtsp );
        text2->drawBorder=0;
        text2->data=&text1->icon[0];
        text2->data2=&text1->icon_size;
        text2->data3=ctx;
        Button_controller_add(ctx->button_controller,text2);
        t=!t;
    }
    else
    {



    }
    if(t==0)
    {
        Button_controller_EraseByKey(ctx->button_controller,"text1");
        Button_controller_EraseByKey(ctx->button_controller,"OK");
    }


}



void camera_getCurrentPosition(void* user_data,void* user_data2,void* user_data3,void* user_data4)
{
    mctx *r=(mctx *)user_data;
    SDL_Log("camera_getCurrentPosition");

    Button*b0= Button_controller_GetButtonByKey(r->button_controller,"console");
    Button*b1= Button_controller_GetButtonByKey(r->button_controller,"console2");


    vec3 cam_pos=r->camera->Position;
    vec3 cam_rot=vec3_create(r->camera->Pitch,r->camera->Yaw+90,0)  ;


    char p[256];
    sprintf(p, "Позиция: (%lf,%lf,%lf) метров",cam_pos.x,cam_pos.z,cam_pos.y );
    Button_setIcon(b0,p);

    sprintf(p, "Поворот:(%lf,%lf,%lf) градусов",cam_rot.x,cam_rot.y,cam_rot.z );
    Button_setIcon(b1,p);




}
