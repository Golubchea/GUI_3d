#ifndef FONT_FONT_H
#define FONT_FONT_H




#ifdef __cplusplus
extern "C" {
#endif

#ifdef FONTS_STATIC
#define FONTS_API static
#else
#define FONTS_API extern
#endif
#include <stdlib.h>
#include "Font_Glyph.h"
#include "Font_Settings.h"
#include "stb_truetype.h"

#define FONS_HASH_LUT_SIZE 256
#define FONS_MAX_FALLBACKS 20

#ifndef FONS_INIT_GLYPHS
#	define FONS_INIT_GLYPHS 256
#endif

typedef struct Font_font
{
    stbtt_fontinfo font;
    char name[64];
    unsigned char* data;
    int dataSize;
    unsigned char freeData;
    float ascender;
    float descender;
    float lineh;
    Font_glyph* glyphs;
    int cglyphs;
    int nglyphs;
    int lut[FONS_HASH_LUT_SIZE];
    int fallbacks[FONS_MAX_FALLBACKS];
    int nfallbacks;
    Font_Settings sdfSettings;
}Font_font;

//create destroy
FONTS_API void Font_font_free(Font_font * f);


FONTS_API Font_glyph* Font_font_allocGlyph(Font_font* font);
//public functions



#ifdef __cplusplus
}
#endif


#endif // ifndef
