//#ifndef CANVAS3D_H
//#define CANVAS3D_H




//#ifdef __cplusplus
//extern "C" {
//#endif


//#include "SDL.h"

//#if OKPLATFORM_EMSCRIPTEN
//#define GL_GLEXT_PROTOTYPES 1
////#include <SDL_opengles2.h>
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
//#else
//#include "canvas/glad/glad.h"
//#include "canvas/glad/khrplatform.h"
//#endif

//#include "data_structures/map.h"
//#include "canvas/window.h"

//#include "objects/Object3d.h"



//#ifdef CANVAS3D_STATIC
//#define CANVAS3D_API static
//#else
//#define CANVAS3D_API extern
//#endif




//typedef struct Canvas3d
//{
//    map_void_t objects;

//    GLfloat projection[16];
//    GLfloat view[16];// camera matrix



//    //main shader id
//    GLuint shader;



//    // use to switch in shader
//    GLint isImage;
//    GLint isImageLoc;

//    // GLint uniform_tex;
//    GLint uniform_color ;

//    GLint uniform_model_matrix ;
//    GLint uniform_view_matrix ;
//    GLint uniform_projection_matrix ;
//    GLint attrib_pos;


//    GLuint VBO, EBO;


//}Canvas3d;




//CANVAS3D_API Canvas3d * Canvas3d_create(Window *win);
//CANVAS3D_API void Canvas3d_add_object(Canvas3d *c,Object3d *e,const char* name);
/////
///// \brief Canvas3d_GetObject get view of camera
///// \param c
///// \param key
///// \return
/////
//CANVAS3D_API Object3d* Canvas3d_GetObject(Canvas3d *c,const char *key);

//CANVAS3D_API void Canvas3d_render(Canvas3d *c);
//CANVAS3D_API void Canvas3d_size_changed(Canvas3d *c,int w,int h);
//CANVAS3D_API void Canvas3d_free(Canvas3d *c);

//#ifdef __cplusplus
//}
//#endif


//#endif // ifndef
