
#include "Menus.h"
#include "button/button.h"

void translateWinPressed(void* user_data,void* user_data2,void* user_data3,void* user_data4)
{
    Menu *m=(Menu *)user_data3;
    Canvas *c=(Canvas *)user_data;
    Mouse *mm=(Mouse  *)user_data2;
    m->dx=mm->x;
    m->dy=mm->y;

    m->isWindowMoved=true;



}

void translateWinEveryTime(void* user_data,void* user_data2,void* user_data3,void* user_data4)
{
    Menu *m=(Menu *)user_data3;
    if (m->isWindowMoved==true)
    {
        //Mouse *mm=(Mouse *)user_data2;
        Canvas *c=(Canvas *)user_data;
        int x;
        int y;
        SDL_GetGlobalMouseState(&x, &y);


        x= x-m->dx;
        y= y-m->dy;
        SDL_SetWindowPosition(c->window->sdlWindow,
                              x,
                              y);
    }

}


void translateWinReleased(void* user_data,void* user_data2,void* user_data3,void* user_data4)
{
    Menu *m=(Menu *)user_data3;
    m->isWindowMoved=false;
}


void closeWin(void* user_data,void* user_data2,void* user_data3,void* user_data4)
{
    SDL_Event sdlevent = {};
    sdlevent.type = SDL_KEYDOWN;
    sdlevent.key.keysym.sym = SDLK_ESCAPE;
    SDL_PushEvent(&sdlevent);
    //      c->event.key.keysym.sym = SDLK_ESCAPE;
}

void maximize(void* user_data,void* user_data2,void* user_data3,void* user_data4)
{
    Menu *m=(Menu *)user_data2;
    Canvas *c=(Canvas *)user_data;


    if(m->maximize_state==1)
    {
        SDL_RestoreWindow(c->window->sdlWindow);
        m->maximize_state=0;
    }
    else if(m->maximize_state==0)
    {
        SDL_MaximizeWindow(c->window->sdlWindow);
        m->maximize_state=1;
    }


}

void minimize(void* user_data,void* user_data2,void* user_data3,void* user_data4)
{
    Canvas *c=(Canvas *)user_data;
    SDL_MinimizeWindow(c->window->sdlWindow);
}


Menu* Menu_create(Button_controller *button_controller, float button_size, Canvas *c, Mouse *mou)
{

    float w=c->width;
    float h=c->height;
    Button* m=Button_create("Menubar",button_size,0,0,w,34,RGBA(130,206,219,155),
                            NULL );
    m->iconOnly=true;
    m->backround=true;
    m->isRounded=true;
    m->drawBorder=false;
    m->rounded_type=ROUNDED_DOWN;
    m->color_background =RGBA(255,255,255,155);
    Button_controller_add(button_controller,m);



    Menu* me=(Menu*)malloc(sizeof (Menu));

    me->isWindowMoved=0;
    me->maximize_state=0;
    Button* mtr=Button_create("Translate_win",button_size,290,-2,w-390,28,RGBA(0,255,47,255),
                              translateWinPressed);
    mtr->iconOnly=true;
    mtr->backround=false;
    mtr->isRounded=false;
    mtr->drawBorder=0;
    mtr->rounded_type=ROUNDED_DOWN;
    mtr->data=c;
    mtr->data2=mou;
    mtr->data3=(void*)me;
    mtr->action_everytime=translateWinEveryTime;
    mtr->action_released=translateWinReleased;

    Button_controller_add(button_controller,mtr);





    Button* m_close=Button_create("Close",button_size+5,w-30,0,26,26,RGBA(0,255,47,255),
                                  closeWin );
    m_close->iconOnly=true;
    Button_setIcon(m_close,"\ue80e");
    m_close->backround=false;
    m_close->isRounded=false;
    m_close->drawBorder=false;
    Button_controller_add(button_controller,m_close);

    Button* m_max=Button_create("Maximize",button_size,w-60,3,26,26,RGBA(0,255,47,255),
                                maximize );
    m_max->iconOnly=true;
    Button_setIcon(m_max,"\ue801");
    m_max->backround=false;
    m_max->isRounded=false;
    m_max->drawBorder=false;
    m_max->rounded_type=ROUNDED_UP;
    m_max->color_hovered=RGBA(0,255,47,255);
    m_max->data=c;
    m_max->data2=me;
    Button_controller_add(button_controller,m_max);

    Button* m_min=Button_create("Minimize",button_size,w-90,3,26,26,RGBA(0,255,47,255),
                                minimize );
    m_min->iconOnly=true;
    Button_setIcon(m_min,"\ue80a");
    m_min->backround=false;
    m_min->isRounded=false;
    m_min->drawBorder=false;
    m_min->color_hovered=RGBA(0,255,47,255);
    m_min->data=c;
    m_min->rounded_type=ROUNDED_UP;
    Button_controller_add(button_controller,m_min);
    return me;
}


void Menu_free(Menu *m)
{
    free (m);

}
