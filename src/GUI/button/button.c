#include "button.h"


#include "../Main_defs.h"
#include <string.h>
static void memCpy0(void *dest, void *src, size_t n)
{
    // Typecast src and dest addresses to (char *)
    char *csrc = (char *)src;
    char *cdest = (char *)dest;

    // Copy contents of src[] to dest[]
    for (int i=0; i<n; i++)
        cdest[i] = csrc[i];
}


static size_t strLen(const char *str)
{
    const char *s;

    for (s = str; *s; ++s)
        ;
    return (s - str);
}

//static char *strdup(const char *s)
//{
//    size_t len = strLen (s) + 1;         // длина строки включая нулевой символ
//    char *result = (char*) malloc (len);
//    if (result == (char*) 0)             // или if (!result)
//        return (char*) 0;
//    // мы уже знаем длину строки и хотим использовать RVO
//    memCpy0(&result[0], (void*)&s[0], len);
//    return result;
//}



void Button_setIcon(Button *b,const char *name)
{

    strcpy(b->icon,name);

}

void Button_move(Button  * b,int mx,int my)
{
    if(b!=NULL)
    {
        //SDL_Log("button: %s",b->name);

        if(b->isMovable==true &&b->isMove==1)
        {
            b->x=mx;
            b->y=my;
        }

    }
}



void Button_free(Button *c)
{

    //free( (void*)c->name);
    //    if(c->icon!=NULL)
    //    {
    //        free( (void*)c->icon);
    //    }
    c->action=NULL;
    free(c);
}

Button *Button_create(const char *name, float text_size, float x, float y, float w, float h, unsigned int color, void (*action)(void *, void *, void *, void *))
{

    Button* b=(Button*)malloc(sizeof (Button));
    if(b==NULL)
    {
        return NULL;
    }
    b->text_size=text_size;
    b->icon_size=0;
    b->x=x;
    b->y=y;
    b->name=strdup(name);
    b->w=w;
    b->h=h;
    b->color=color;
    b->action=action;
    memset(b->icon,'\0',128);

    b->iconOnly=false;
    b->drawBorder=true;
    b->isMovable=false;
    b->backround=false;
    b->isRounded=false;
    b->action_released=NULL;
    b->action_everytime=NULL;
    b->color_hovered=RGBA(0,0,0,255);
    b->color_background=RGBA(0,0,0,200);
    b->rounded_type=ROUNDED;
    b->color_released=color;
    b->textIsIcon=false;


    b->icon_size_max=(w/text_size) *2;


    return b;
}
