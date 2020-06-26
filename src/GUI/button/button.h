#ifndef BUTTON_H
#define BUTTON_H




#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#ifdef BUTTON_STATIC
#define BUTTON_API static
#else
#define BUTTON_API extern
#endif

enum ROUNDED
{
    ROUNDED,
    ROUNDED_UP,
    ROUNDED_DOWN
};

typedef struct Button
{
    float x;
    float y;
    float w;
    float h;
    float text_size;
    unsigned int color;
    const char* name;

    char icon[128];
    int icon_size;
    int icon_size_max;
    //use data pointer with actions
    void (*action)(void* user_data,void* user_data2,void* user_data3,void* user_data4);
    void (*action_released)(void* user_data,void* user_data2,void* user_data3,void* user_data4);
    void (*action_everytime)(void* user_data,void* user_data2,void* user_data3,void* user_data4);
    void* data;
    void* data2;
    void* data3;
    void* data4;
    bool iconOnly;
    bool drawBorder;
    bool isMovable;
    bool isMove;
    bool textIsIcon;

    unsigned int color_hovered;
    unsigned int color_released;
    unsigned int color_background;
    bool backround;
    bool isRounded;
    enum ROUNDED rounded_type;
    bool isTouched;
}Button;



BUTTON_API Button *Button_create( const char* name,float text_size,float x,float y,float w,float h,unsigned int color,
                                  void (*action)(void* user_data,void* user_data2,void* user_data3,void* user_data4)   );
void Button_setIcon(Button *b,const char *name);



void Button_move(Button  * b,int mx,int my);




BUTTON_API void Button_free(Button * c);



#ifdef __cplusplus
}
#endif


#endif // ifndef
