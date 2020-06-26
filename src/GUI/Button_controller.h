#ifndef BUTTON_CONTROLLER_H
#define BUTTON_CONTROLLER_H




#ifdef __cplusplus
extern "C" {
#endif



#ifdef BUTTON_STATIC
#define BUTTON_API static
#else
#define BUTTON_API extern
#endif

#include "data_structures/map.h"
#include "button/button.h"
#include "canvas.h"
typedef struct Button_controller
{
map_void_t buttons;
}Button_controller;





BUTTON_API Button_controller *Button_controller_create( );
BUTTON_API void Button_controller_free(Button_controller * c);

BUTTON_API void Button_controller_add(Button_controller *bc, Button *b);

//actions
BUTTON_API Button * Button_controller_pressed(Button_controller * c,int x,int y);
BUTTON_API Button * Button_controller_released(Button_controller * c,int x,int y);
BUTTON_API void Button_controller_action_everytime(Button_controller * c);
BUTTON_API void Button_controller_action_resizeWindow(Button_controller * c,Canvas *canv,int oldX,int oldY);


BUTTON_API void Button_controller_drawShapes(Button_controller * bc,Canvas* c);
BUTTON_API void Button_controller_drawTexts(Button_controller * bc,Canvas* canvas);
BUTTON_API void Button_controller_drawIcons(Button_controller * bc,Canvas* canvas);
BUTTON_API Button* Button_controller_GetButtonByKey(Button_controller * bc,const char *key);
BUTTON_API void  Button_controller_EraseByKey(Button_controller * bc,const char *key);
BUTTON_API void Button_controller_ButtonChoose(Button_controller * bc,int mx,int my);
BUTTON_API void Button_controller_ButtonMove(Button_controller * bc,int mx,int my);
#ifdef __cplusplus
}
#endif


#endif // ifndef
