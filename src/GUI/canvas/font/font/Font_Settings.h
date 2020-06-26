#ifndef FONT_SETTINGS_H
#define FONT_SETTINGS_H




#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include "Font_Glyph.h"
#include "stb_truetype.h"
#ifdef FONTS_STATIC
#define FONTS_API static
#else
#define FONTS_API extern
#endif


// See also: stb_truetype documentation for stbtt_GetGlyphSDF. These parameters are copied from there
typedef struct Font_Settings
{
    unsigned char sdfEnabled;
    unsigned char onedgeValue; // value 0-255 to test the SDF against to reconstruct the character (i.e. the isocontour of the character)

    int padding;               // extra "pixels" around the character which are filled with the distance to the character (not 0),
    // which allows effects like bit outlines

    float pixelDistScale;      // what value the SDF should increase by when moving one SDF "pixel" away from the edge (on the 0..255 scale)
    // if positive, > onedge_value is inside; if negative, < onedge_value is inside
}Font_Settings;

FONTS_API int min_i(int a, int b);
FONTS_API int max_i(int a, int b);
FONTS_API  void *memSet(void *s, int c, size_t n);


#ifdef __cplusplus
}
#endif


#endif // ifndef
