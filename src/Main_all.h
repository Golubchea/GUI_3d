#pragma once


extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "SDL.h"
#include "GUI/canvas.h"
#include "GUI/canvas3d.h"

#include "GUI/Button_controller.h"
#include "GUI/button/button.h"
#include "GUI/camera.h"


#include <geometry/projectUnprojectPoint.h>
#include <matrix/matrix.h>

#include "GUI/canvas/mathGL.h"
#include "GUI/Menus.h"


#include "Context_menu.h"
}
#include <memory>


typedef struct mctx
{
    Canvas              *canvas;
    Button_controller   *button_controller;
    Menu                *menu;
    Mouse               *mouse;
    Canvas3d            *canvas3d;
    Camera              *camera;
    Context_menu        *c_menu=NULL;
    Button *selected=NULL;

    bool drawing_lnes=true;
    Sint64 t;
    bool isText=true;
}mctx;
