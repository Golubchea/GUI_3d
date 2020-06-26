#include "Button_controller.h"
#include "canvas.h"
#include <stdbool.h>

Button_controller *Button_controller_create( )
{
    Button_controller* b=(Button_controller*)malloc(sizeof (Button_controller));
    if(b==NULL)
    {
        return NULL;
    }
    map_init(&b->buttons);

    return b;
}


void Button_controller_add(Button_controller *bc, Button *b)
{
    map_set(&bc->buttons, b->name, b);
}

void Button_controller_free(Button_controller * c)
{
    map_deinit(&c->buttons);
    free(c);
}
;
static inline bool inRectangle(int x,int y, int rx,int ry, int w,int h)
{
    if (  (x >= rx && x <= rx + w && y >= ry && y <= ry + h) )
    {
        return true;
    }
    else
    {
        return false;
    }
}


void Button_controller_ButtonChoose(Button_controller * bc,int mx,int my)
{
    const char *key;
    map_iter_t iter = map_iter(&bc->buttons );
    while ((key = map_next(&bc->buttons, &iter)))
    {
        Button* b= (Button* )*map_get(&bc->buttons, key);

        //SDL_Log("button: %s",b->name);

        if(inRectangle(mx,my,b->x,b->y,b->w,b->h))
        {
            b->isMovable=!b->isMovable;
        }

    }
}

void Button_controller_ButtonMove(Button_controller * bc,int mx,int my)
{
    const char *key;
    map_iter_t iter = map_iter(&bc->buttons );
    while ((key = map_next(&bc->buttons, &iter)))
    {
        Button* b= (Button* )*map_get(&bc->buttons, key);

        //SDL_Log("button: %s",b->name);

        if(b->isMovable==true)
        {
            b->x=mx;
            b->y=my;
        }

    }
}


Button* Button_controller_GetButtonByKey(Button_controller * bc,const char *key)
{
   void* val= *map_get(&bc->buttons, key);



    if(val==NULL)
    {
        return NULL;
    }
    else
    {
        Button* b= ( Button* )val;
        return b;
    }

}



Button* Button_controller_pressed(Button_controller * c,int x,int y)
{
    const char *key;

    map_iter_t iter = map_iter(&c->buttons );

    while ((key = map_next(&c->buttons, &iter)))
    {
        Button* b= (Button* )*map_get(&c->buttons, key);
        if(b->action!=NULL)
        {
            // SDL_Log("button: %s",b->name);

            if(inRectangle(x,y,b->x,b->y,b->w,b->h))
            {
                b->color=b->color_hovered;
                b->action(b->data,b->data2,b->data3,b->data4);
                b->isMove=1;
                return b;
            }
        }
    }
    return NULL;
}


Button* Button_controller_released(Button_controller * c,int x,int y)
{
    const char *key;

    map_iter_t iter = map_iter(&c->buttons );

    while ((key = map_next(&c->buttons, &iter)))
    {
        Button* b= (Button* )*map_get(&c->buttons, key);
        if(b->action!=NULL )
        {
            //SDL_Log("RELEASED button: %s",b->name);
            //SDL_Log("rect: %lf %lf %lf %lf ",b->x,b->y,b->w,b->h);
            if(inRectangle(x,y,b->x,b->y,b->w,b->h))
            {

                b->color=b->color_released;
                if(b->action_released!=NULL)
                {
                    b->action_released(b->data,b->data2,b->data3,b->data4);

                }

                b->isMove=0;
                return b;
            }
        }
    }
    return NULL;
}


void Button_controller_action_everytime(Button_controller * c)
{
    const char *key;

    map_iter_t iter = map_iter(&c->buttons );

    while ((key = map_next(&c->buttons, &iter)))
    {
        Button* b= (Button* )*map_get(&c->buttons, key);
        if(b->action_everytime!=NULL )
        {
            b->action_everytime(b->data,b->data2,b->data3,b->data3);
        }
    }
}



void Button_controller_drawShapes(Button_controller * bc,Canvas* c)
{
    const char *key;

    map_iter_t iter = map_iter(&bc->buttons );

    while ((key = map_next(&bc->buttons, &iter)))
    {

        Button* b= (Button* )*map_get(&bc->buttons, key);
        if(b->backround==true && b->isRounded==false)
        {
            canvas_DrawRectangle(c,b->x,b->y,b->w,b->h, b->color_background );
        }
        else if (b->backround==true && b->isRounded==true)
        {


            if(b->rounded_type==ROUNDED)
            {
                canvas_DrawRectangleRounded(c,b->x,b->y,b->w,b->h,b->text_size,5, b->color_background );
            }
            else if (b->rounded_type==ROUNDED_UP)
            {
                canvas_DrawRectangleRoundedUp(c,b->x,b->y,b->w,b->h,b->text_size,6, b->color_background );
            }
            else if (b->rounded_type==ROUNDED_DOWN)
            {
                canvas_DrawRectangleRoundedDown(c,b->x,b->y,b->w,b->h,b->text_size,5, b->color_background );
            }


        }

        if(b->drawBorder==true)
        {
            canvas_DrawRectangleLine(c,b->x,b->y,b->w,b->h,  b->color);
        }

    }
}





void Button_controller_drawTexts(Button_controller * bc,Canvas* canvas)
{
    const char *key;

    map_iter_t iter = map_iter(&bc->buttons );
    Font_ClearState(canvas->font->fs);
    Font_SetFont(canvas->font->fs, canvas->font->fontSdf);

    Font_SetAlign(canvas->font->fs, FONT_ALIGN_LEFT | FONT_ALIGN_TOP);
    Font_VertMetrics(canvas->font->fs, NULL, NULL, &canvas->font->lineHeight);


    while ((key = map_next(&bc->buttons, &iter)))
    {
        Button* b= (Button* )*map_get(&bc->buttons, key);
        if(b->iconOnly!=true && b->textIsIcon==false)
        {
            Font_SetColor(canvas->font->fs, b->color);
            Font_SetSize(canvas->font->fs, b->text_size);
            Font_DrawText(canvas->font->fs, b->x+24, b->y, b->name , NULL);
        }
    }
}


void Button_controller_drawIcons(Button_controller * bc,Canvas* canvas)
{
    const char *key;

    map_iter_t iter = map_iter(&bc->buttons );
    Font_ClearState(canvas->font->fs);
    //Font_SetFont(canvas->font->fs, canvas->font->fontIcon);


    Font_SetAlign(canvas->font->fs, FONT_ALIGN_LEFT | FONT_ALIGN_TOP);
    Font_VertMetrics(canvas->font->fs, NULL, NULL, &canvas->font->lineHeight);


    while ((key = map_next(&bc->buttons, &iter)))
    {
        Button* b= (Button* )*map_get(&bc->buttons, key);
        if(b->icon!=NULL)
        {
            if(b->textIsIcon)
            {
                Font_SetFont(canvas->font->fs, canvas->font->fontSdf);
            }
            else
            {
                Font_SetFont(canvas->font->fs, canvas->font->fontIcon);
            }
            Font_SetSize(canvas->font->fs, b->text_size);
            Font_SetColor(canvas->font->fs, b->color);
            Font_DrawText(canvas->font->fs, b->x+2, b->y, b->icon , NULL);
        }

    }
}




void  Button_controller_EraseByKey(Button_controller * bc,const char *key)
{
    map_remove(&bc->buttons, key);
}










void Button_controller_action_resizeWindow(Button_controller *bc,Canvas *canv,int oldWidth,int oldHeihgt)
{
    //Button* menu=Button_controller_GetButtonByKey(bc,"Menubar");
    //menu->w=canv->width;
    //Button* console=Button_controller_GetButtonByKey(bc,"Consolebar");
    //console->w=canv->width/2.0-canv->width/70.0 +1;


    const char *key;
    map_iter_t iter = map_iter(&bc->buttons );
    while ((key = map_next(&bc->buttons, &iter)))
    {
        Button* b= (Button* )*map_get(&bc->buttons, key);
        b->x=canv->width*b->x/(double)oldWidth;
        b->y=canv->height*b->y/(double)oldHeihgt;

        b->w=canv->width*b->w/(double)oldWidth;
        //b->h=canv->height*b->h/(double)oldHeihgt;
        b->icon_size_max= (b->w/b->text_size)*1.2;
    }
}
