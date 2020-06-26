#pragma once

#include "GUI/Button_controller.h"




typedef struct Context_menu
{
    int x;
    int y;
    Button *b1;
    Button *b2;
    Button *b3;
    Button *b4;
}Context_menu;

void load1(void* user_data,void* user_data2,void* user_data3,void* user_data4)
{

}

Context_menu *Context_menu_create(Button_controller *bc,int x,int y)
{
    Context_menu * r =(Context_menu *)malloc(sizeof (Context_menu));
    r->b4=Button_create("context_box",16,x,y,90,20*3,RGBA(0,0,0,50),NULL);
    r->b4->textIsIcon=1;
    r->b4->isRounded=1;
    r->b4->drawBorder=1;
    r->b4->rounded_type=ROUNDED;
    r->b4->color_background=RGBA(0,0,0,50);
    r->b4->backround=1;


    r->b1=Button_create("ctext1",18,x,y+20*0,90,15,RGBA(100,100,100,100),load1);
    r->b2=Button_create("ctext2",18,x,y+20*1,90,15,RGBA(100,100,100,100),NULL);
    r->b3=Button_create("ctext3",18,x,y+20*2,90,15,RGBA(100,100,100,100),NULL);



    Button_controller_add(bc,r->b1);
    Button_controller_add(bc,r->b2);
    Button_controller_add(bc,r->b3);
    Button_controller_add(bc,r->b4);
    return r;
}

void Context_menu_free(Context_menu * c,Button_controller *bc)
{
    Button_controller_EraseByKey(bc,"ctext1");
    Button_controller_EraseByKey(bc,"ctext2");
    Button_controller_EraseByKey(bc,"ctext3");
    Button_controller_EraseByKey(bc,"context_box");
    free(c);
}
