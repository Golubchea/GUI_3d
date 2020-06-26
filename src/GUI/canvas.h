#ifndef CANVAS_H
#define CANVAS_H




#ifdef __cplusplus
extern "C" {
#endif


#include "canvas/okplatform.h"
#include "SDL.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "canvas/window.h"
#include "canvas/shader.h"
#include "canvas/mathGL.h"
#include "canvas/font/Font.h"
#include "Main_defs.h"
#if OKPLATFORM_EMSCRIPTEN
#define GL_GLEXT_PROTOTYPES 1
//#include <SDL_opengles2.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "canvas/glad/glad.h"
#include "canvas/glad/khrplatform.h"
#endif

#define CANVAS_VERTEX_COUNT 512

#ifndef GLFONS_VERTEX_ATTRIB
#	define GLFONS_VERTEX_ATTRIB 0
#endif

#ifndef GLFONS_TCOORD_ATTRIB
#	define GLFONS_TCOORD_ATTRIB 1
#endif

#ifndef GLFONS_COLOR_ATTRIB
#	define GLFONS_COLOR_ATTRIB 2
#endif




#ifdef CANVAS_STATIC
#define CANVAS_API static
#else
#define CANVAS_API extern
#endif


typedef struct Canvas_data
{

    float verts[CANVAS_VERTEX_COUNT*2];
    float tcoords[CANVAS_VERTEX_COUNT*2];
    unsigned int colors[CANVAS_VERTEX_COUNT];
    unsigned int nverts;


    int texW;
    int texH;

}Canvas_data;

typedef struct Quad
{
    float x0,y0,s0,t0;
    float x1,y1,s1,t1;
}Quad;

typedef struct Canvas
{
    GLuint tex;
    int width, height;

    int width_image, height_image;

    GLuint vertexBuffer;
    GLuint tcoordBuffer;
    GLuint colorBuffer;
    GLuint vertexArray;

    // use to switch in shader
    GLint shaderInSwitcherLoc;
    GLint shader_state;


    GLfloat projection[16];
    //GLfloat modelView[16];
    GLfloat modelViewGUI[16];
    //id's for shader communication
    GLint projectionMatrixLoc;
    GLint modelViewMatrixLoc;
    GLint projectionMatrixSDFLoc ;
    GLint modelViewMatrixSDFLoc ;

    Canvas_data data;
    Font *font;
    Window *window;
    Shader_sdf* shaders;
    SDL_Event event;

    bool needToRedraw;
}Canvas;



///
/// \brief Canvas_create
/// конструктор
/// \return
///
CANVAS_API Canvas *Canvas_create( const char * background);
///
/// \brief canvas_DrawRectangleLine
/// \param stash
/// \param x
/// \param y
/// \param w
/// \param h
/// \param color
///
CANVAS_API void canvas_DrawRectangleLine(Canvas* stash,
                                         float x, float y,
                                         float w,float h, unsigned int color);
///
/// \brief canvas_DrawRectangleLine2
/// \param stash
/// \param x0
/// \param y0
/// \param x1
/// \param y1
/// \param x2
/// \param y2
/// \param x3
/// \param y3
/// \param color
///
CANVAS_API void canvas_DrawRectangleLine2(Canvas* stash,
                                          float x0, float y0,
                                          float x1, float y1,
                                          float x2, float y2,
                                          float x3, float y3,
                                          unsigned int color);


CANVAS_API void canvas_DrawLine(Canvas* stash,
                                float x, float y,
                                float x2,float y2, unsigned int color);


CANVAS_API void canvas_DrawRectangleRounded(Canvas* stash,
                                         float x, float y,
                                         float w,float h,float radius, int n,unsigned int color);


CANVAS_API void canvas_DrawRectangleRoundedUp(Canvas* stash,
                                            float x, float y,
                                            float w,float h,float r,int n, unsigned int color);


CANVAS_API void canvas_DrawRectangleRoundedDown(Canvas* stash,
                                            float x, float y,
                                            float w,float h,float r,int n, unsigned int color);


/// \brief canvas_DrawRectangle
/// \param stash
/// \param x
/// \param y
/// \param w
/// \param h
/// \param color
CANVAS_API void canvas_DrawRectangle(Canvas* stash,
                                     float x, float y,
                                     float w,float h, unsigned int color);
///
/// \brief Canvas_free
/// \param c
///
CANVAS_API void Canvas_free(Canvas * c);



//inner functions
CANVAS_API __inline void canvas_add_vertex(Canvas *canvas, float x, float y, float s, float t, unsigned int c);

CANVAS_API void canvas_start_paint2dUI(Canvas * c);
CANVAS_API void canvas_end_paint2dUI(Canvas * c);
CANVAS_API void canvas_size_changed(Canvas * c);
CANVAS_API void canvas_start_paintText(Canvas *canvas);



extern size_t strLen2(const char *str);

extern char* strCat(char* destination, const char* source);
int strCat2(char* destination, const char* source);

extern void RemoveChars(char *s, char c);



#include "data_structures/map.h"
#include "canvas/window.h"

#include "objects/Object3d.h"

#ifdef CANVAS3D_STATIC
#define CANVAS3D_API static
#else
#define CANVAS3D_API extern
#endif




typedef struct Canvas3d
{
    map_void_t objects;

    GLfloat projection[16];
    GLfloat view[16];// camera matrix



    //main shader id
    GLuint shader;



    // use to switch in shader
    GLint isImage;
    GLint isImageLoc;

    // GLint uniform_tex;
    GLint uniform_color ;

    GLint uniform_model_matrix ;
    GLint uniform_view_matrix ;
    GLint uniform_projection_matrix ;
    GLint attrib_pos;


    GLuint VBO, EBO;

    vec4 kx;
    vec4 ky;
}Canvas3d;




CANVAS3D_API Canvas3d * Canvas3d_create(Window *win);
CANVAS3D_API void Canvas3d_add_object(Canvas3d *c,Object3d *e,const char* name);
///
/// \brief Canvas3d_GetObject get from map 3d object
/// \param c
/// \param key
/// \return
///
CANVAS3D_API Object3d* Canvas3d_GetObject(Canvas3d *c,const char *key);
CANVAS3D_API int Canvas3d_delete_object(Canvas3d *c,  const char *name);
CANVAS3D_API void Canvas3d_render(Canvas3d *c);
CANVAS3D_API void Canvas3d_size_changed(Canvas3d *c,int w,int h);
CANVAS3D_API void Canvas3d_free(Canvas3d *c);




#ifdef __cplusplus
}
#endif


#endif // ifndef
