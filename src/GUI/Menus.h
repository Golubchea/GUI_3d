#ifndef MENUS_H
#define MENUS_H




#ifdef __cplusplus
extern "C" {
#endif



#ifdef MENUS_STATIC
#define MENUS_API static
#else
#define MENUS_API extern
#endif
#include <stdbool.h>
#include "Main_defs.h"


typedef struct Menu
{
    bool isWindowMoved;
    int dx;
    int dy;
    int maximize_state;

}Menu;


#include "Button_controller.h"
#include "canvas.h"

MENUS_API Menu* Menu_create(Button_controller *button_controller,float button_size,Canvas* c,Mouse *m );
MENUS_API void Menu_free(Menu *m);

#ifdef __cplusplus
}
#endif


#endif // ifndef
