#ifndef FONT_ATLAS_H
#define FONT_ATLAS_H




#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>

#ifdef FONTS_STATIC
#define FONTS_API static
#else
#define FONTS_API extern
#endif

typedef struct Font_atlasNode
{
    short x, y, width;
}Font_atlasNode;

typedef struct Font_atlas
{
    int width, height;
    Font_atlasNode* nodes;
    int nnodes;
    int cnodes;
}Font_atlas;
//create destroy
FONTS_API Font_atlas * font_atlas_create(int w, int h, int nnodes);
FONTS_API void font_atlas_free(Font_atlas * f);

//public functions
FONTS_API int  font_atlas_InsertNode(Font_atlas* atlas, int idx, int x, int y, int w);
FONTS_API void font_atlas_RemoveNode(Font_atlas* atlas, int idx);
FONTS_API void font_atlas_Expand(Font_atlas* atlas, int w, int h);
FONTS_API void font_atlas_Reset(Font_atlas * atlas, int w, int h);
FONTS_API int  font_atlas_AddSkylineLevel(Font_atlas* atlas, int idx, int x, int y, int w, int h);

FONTS_API int font_atlas_RectFits(Font_atlas* atlas, int i, int w, int h);
FONTS_API int font_atlas_AddRect(Font_atlas* atlas, int rw, int rh, int* rx, int* ry);

// obvious defs for c
FONTS_API int min_i(int a, int b);
FONTS_API int max_i(int a, int b);
FONTS_API void *memSet(void *s, int c, size_t n);


#ifdef __cplusplus
}
#endif


#endif // ifndef
