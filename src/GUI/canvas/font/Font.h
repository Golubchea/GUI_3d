#ifndef FONT_H
#define FONT_H




#ifdef __cplusplus
extern "C" {
#endif

#include "SDL.h"
#include <stdbool.h>



//#if OKPLATFORM_EMSCRIPTEN
//#define GL_GLEXT_PROTOTYPES 1
//#include <SDL_opengles2.h>

//#else
//#include "glad/glad.h"
//#include "glad/khrplatform.h"
//#endif

#include "font/Font_main.h"

#ifdef FONTS_STATIC
#define FONTS_API static
#else
#define FONTS_API extern
#endif

typedef struct Font
{
    Font_main_ctx* fs ;//= NULL;
    uint8_t* fontDataDroidSans;// = NULL;
    uint8_t* fontDataDroidIcons;
    int fontIcon ;//= FONS_INVALID;
    int fontSdf ;//= FONS_INVALID;
     float lineHeight ;
}Font;


FONTS_API Font *Font_create(const char* fontPath_TTF,
                                      const char* fontPathICONS_TTF,
                                      int (*renderCreate)(void* uptr, int width, int height) ,
                                      void (*renderUpdate)(void* uptr, int* rect, const unsigned char* data),
                                      void (*renderDraw)(void* uptr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts),
                                      void (*renderDelete)(void* uptr) );
FONTS_API void Font_free(Font * f);



#ifdef __cplusplus
}
#endif


#endif // ifndef OKWRAPPER_OKAPP_H_
