#ifndef FONT_MAIN_H
#define FONT_MAIN_H




#ifdef __cplusplus
extern "C" {
#endif

#include "SDL.h"
#include <stdbool.h>

#if OKPLATFORM_EMSCRIPTEN
#define GL_GLEXT_PROTOTYPES 1
//#include <SDL_opengles2.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "../../glad/glad.h"
#include "../../glad/khrplatform.h"
#endif

#include "stb_truetype.h"
//#include "fontstash/DataIO.h"
#include "Font_Atlas.h"
#include "Font_Glyph.h"
#include "Font_Settings.h"
#include "Font_font.h"


#ifdef FONTS_STATIC
#define FONTS_API static
#else
#define FONTS_API extern
#endif

enum Font_flags {
    FONT_ZERO_TOPLEFT = 1,
    FONT_ZERO_BOTTOMLEFT = 2,
};

enum Font_align {
    // Horizontal align
    FONT_ALIGN_LEFT 	= 1<<0,	// Default
    FONT_ALIGN_CENTER 	= 1<<1,
    FONT_ALIGN_RIGHT 	= 1<<2,
    // Vertical align
    FONT_ALIGN_TOP 		= 1<<3,
    FONT_ALIGN_MIDDLE	= 1<<4,
    FONT_ALIGN_BOTTOM	= 1<<5,
    FONT_ALIGN_BASELINE	= 1<<6, // Default
};

enum Font_errorCode {
    // Font atlas is full.
    FONT_ATLAS_FULL = 1,
    // Scratch memory used to render glyphs is full, requested size reported in 'val', you may need to bump up FONS_SCRATCH_BUF_SIZE.
    FONT_SCRATCH_FULL = 2,
    // Calls to fonsPushState has created too large stack, if you need deep state stack bump up FONS_MAX_STATES.
    FONT_STATES_OVERFLOW = 3,
    // Trying to pop too many states fonsPopState().
    FONT_STATES_UNDERFLOW = 4,
};



typedef struct GLcontext
{
    GLuint tex;
    int width, height;
    GLuint vertexBuffer;
    GLuint tcoordBuffer;
    GLuint colorBuffer;
    GLuint vertexArray; // Not used if GLFONTSTASH_IMPLEMENTATION_ES2 is defined
    int (*renderCreate)(void* uptr, int width, int height);

    void (*renderUpdate)(void* uptr, int* rect, const unsigned char* data);
    void (*renderDraw)(void* uptr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts);
    void (*renderDelete)(void* uptr);
}GLcontext;



typedef struct Font_parameters
{
    int width, height;
    unsigned char flags;
    GLcontext* userPtr;
}Font_parameters;


typedef struct Font_quad
{
    float x0,y0,s0,t0;
    float x1,y1,s1,t1;
}Font_quad;




typedef struct Font_iterator
{
    float x, y, nextx, nexty, scale, spacing;
    unsigned int codepoint;
    short isize, iblur;
    struct Font_font* font;
    int prevGlyphIndex;
    const char* str;
    const char* next;
    const char* end;
    unsigned int utf8state;
}Font_iterator;


typedef struct Font_state
{
    int font;
    int align;
    float size;
    unsigned int color;
    float blur;
    float spacing;
}Font_state;




#ifndef FONS_VERTEX_COUNT
#	define FONS_VERTEX_COUNT 1024
#endif
#ifndef FONS_MAX_STATES
#	define FONS_MAX_STATES 20
#endif

typedef struct Font_main_ctx
{
    Font_parameters params;
    float itw,ith;
    unsigned char* texData;
    int dirtyRect[4];
    Font_font** fonts;
    Font_atlas* atlas;
    int cfonts;
    int nfonts;
    float verts[FONS_VERTEX_COUNT*2];
    float tcoords[FONS_VERTEX_COUNT*2];
    unsigned int colors[FONS_VERTEX_COUNT];
    int nverts;
    unsigned char* scratch;
    int nscratch;
    Font_state states[FONS_MAX_STATES];
    int nstates;
    void (*handleError)(void* uptr, int error, int val);
    void* errorUptr;
}Font_main_ctx;



typedef struct Font_ctx
{
    Font_main_ctx* fs ;//= NULL;
    uint8_t* font_data_text;// = NULL;
    uint8_t* font_data_icons;
    int font_iconId ;//= FONS_INVALID;
    int font_textId ;//= FONS_INVALID;
    float lineHeight ;
}Font_ctx;


FONTS_API Font_main_ctx * font_main_create(Font_parameters * params);
FONTS_API void font_main_free(Font_main_ctx * f);





//public api functions

FONTS_API int Font_AddFontSdfMem(Font_main_ctx* stash, const char* name, unsigned char* data, int dataSize, int freeData, Font_Settings sdfSettings);

FONTS_API void Font_ClearState(Font_main_ctx* stash);
FONTS_API void Font_SetAlign(Font_main_ctx* stash, int align);
FONTS_API void Font_SetFont(Font_main_ctx* stash, int font);
FONTS_API void Font_SetSize(Font_main_ctx* stash, float size);
FONTS_API void Font_SetAlign(Font_main_ctx* stash, int align);
FONTS_API void Font_VertMetrics(Font_main_ctx*  stash,
                                float* ascender, float* descender, float* lineh);
FONTS_API void Font_SetColor(Font_main_ctx* stash, unsigned int color);

FONTS_API void Font_DrawText(Font_main_ctx* stash,
                             float x, float y,
                             const char* str, const char* end);



//private functions
FONTS_API Font_glyph* Font_getGlyph(Font_main_ctx* stash, Font_font* font, unsigned int codepoint,
                                    short isize, short iblur);

FONTS_API Font_glyph * Font_allocGlyph(Font_font* font);
FONTS_API int Font_ResetAtlas(Font_main_ctx* stash, int width, int height);
FONTS_API void Font_GetAtlasSize(Font_main_ctx* stash, int* width, int* height);
FONTS_API int Font_ExpandAtlas(Font_main_ctx* stash, int width, int height);
FONTS_API int  Font_allocFont(Font_main_ctx* stash);
#ifdef __cplusplus
}
#endif


#endif // ifndef OKWRAPPER_OKAPP_H_
