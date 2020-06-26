#include "Font_font.h"



Font_glyph* Font_font_allocGlyph(Font_font* font)
{
    if (font->nglyphs+1 > font->cglyphs) {
        font->cglyphs = font->cglyphs == 0 ? 8 : font->cglyphs * 2;
        font->glyphs = (Font_glyph*)realloc(font->glyphs, sizeof(Font_glyph) * font->cglyphs);
        if (font->glyphs == NULL) return NULL;
    }
    font->nglyphs++;
    return &font->glyphs[font->nglyphs-1];
}



void Font_font_free(Font_font *font)
{
    if (font == NULL) return;
    if (font->glyphs) free(font->glyphs);
    if (font->freeData && font->data) free(font->data);
    free(font);
}


