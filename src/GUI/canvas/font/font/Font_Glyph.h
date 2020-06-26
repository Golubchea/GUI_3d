#ifndef FONT_GLYPH_H
#define FONT_GLYPH_H




#ifdef __cplusplus
extern "C" {
#endif

#ifdef FONTS_STATIC
#define FONTS_API static
#else
#define FONTS_API extern
#endif

#include <stdlib.h>

//#include "Font_main.h"

typedef struct Font_glyph
{
    unsigned int codepoint;
    int index;
    int next;
    short size, blur;
    short x0,y0,x1,y1;
    short xadv,xoff,yoff;
}Font_glyph;

//create destroy
//FONTS_API Font_atlas * font_atlas_create(int w, int h, int nnodes);
//FONTS_API void font_atlas_free(Font_atlas * f);




#ifdef __cplusplus
}
#endif


#endif
