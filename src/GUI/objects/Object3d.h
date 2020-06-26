#ifndef OBJECT3D_H
#define OBJECT3D_H




#ifdef __cplusplus
extern "C" {
#endif
#include <vec2.h>
#include <vec3.h>

#if OKPLATFORM_EMSCRIPTEN
#define GL_GLEXT_PROTOTYPES 1
//#include <SDL_opengles2.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "../canvas/glad/glad.h"
#include "../canvas/glad/khrplatform.h"
#endif


#ifdef CANVAS3D_STATIC
#define CANVAS3D_API static
#else
#define CANVAS3D_API extern
#endif

enum STYLE
{
    LINE,POLY,LINE_LOOP
};


typedef struct Color
{
    float r;
    float g;
    float b;
    float a;

}Color;





typedef struct Object3d
{
enum STYLE style;
GLfloat model[16];
GLfloat color[4];

GLfloat * vertices;
GLuint64  vertices_size;

GLushort * elements;
GLuint64  elements_size;
const char *name;
}Object3d;



//public functions

CANVAS3D_API Color Color_create(float r,float g,float b,float a);


CANVAS3D_API Object3d *Object3d_create(vec3 position,Color color);
CANVAS3D_API void Object3d_free(Object3d *c);

CANVAS3D_API vec3 Object3d_getPos(Object3d *c);

CANVAS3D_API void Object3d_Line(Object3d * obj,vec3 p1,vec3 p2);
CANVAS3D_API void Object3d_Grid(Object3d * obj,double step,vec3 size);
CANVAS3D_API void Object3d_axis(Object3d * obj,float size);
CANVAS3D_API void Object3d_Cube(Object3d * obj,vec3 volume);
CANVAS3D_API void Object3d_Rectangle(Object3d * obj, vec3 p1,vec3 p2,vec3 p3,vec3 p4);
CANVAS3D_API void Object3d_camera(Object3d * obj,float*projection,   Color x,vec3 rotation);

#ifdef __cplusplus
}
#endif


#endif // ifndef
