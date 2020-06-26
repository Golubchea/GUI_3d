#include "Font_main.h"

#ifndef FONS_SCRATCH_BUF_SIZE
#define FONS_SCRATCH_BUF_SIZE 64000
#endif
#define FONS_INIT_ATLAS_NODES 256
#define FONS_INIT_FONTS 4
#define GLFONTSTASH_IMPLEMENTATION_ES2


#ifndef GLFONS_VERTEX_ATTRIB
#	define GLFONS_VERTEX_ATTRIB 0
#endif

#ifndef GLFONS_TCOORD_ATTRIB
#	define GLFONS_TCOORD_ATTRIB 1
#endif

#ifndef GLFONS_COLOR_ATTRIB
#	define GLFONS_COLOR_ATTRIB 2
#endif


#if OKPLATFORM_EMSCRIPTEN
#define GL_GLEXT_PROTOTYPES 1
 //#include <SDL_opengles2.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "../../glad/glad.h"
#include "../../glad/khrplatform.h"
#endif
#define FONT_NOTUSED(v)  (void)sizeof(v)


static void *memSet2(void *s, int c, size_t n)
{

    unsigned char* p=(unsigned char*)s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}

static void memCpy(void *dest, void *src, size_t n)
{
    // Typecast src and dest addresses to (char *)
    char *csrc = (char *)src;
    char *cdest = (char *)dest;

    // Copy contents of src[] to dest[]
    for (int i=0; i<n; i++)
        cdest[i] = csrc[i];
}

void Font_renderUpdate(GLcontext* gl, int* rect, const unsigned char* data)
{
gl->renderUpdate((void*)gl,rect,data);

}


void Font_renderDraw(GLcontext* gl  , const float* verts, const float* tcoords, const unsigned int* colors, int nverts)
{
gl->renderDraw((void*)gl,verts,tcoords,colors,nverts);
}

int Font_main_renderCreate(GLcontext *gl, int width, int height)
{
return gl->renderCreate(gl,width,height);
}



void Font_main_renderDelete(GLcontext *gl)
{
gl->renderDelete((void*)gl);
}





void Font_flush(Font_main_ctx* stash)
{
    // Flush texture
    if (stash->dirtyRect[0] < stash->dirtyRect[2] && stash->dirtyRect[1] < stash->dirtyRect[3]) {

        Font_renderUpdate(stash->params.userPtr, stash->dirtyRect, stash->texData);
        // Reset dirty rect
        stash->dirtyRect[0] = stash->params.width;
        stash->dirtyRect[1] = stash->params.height;
        stash->dirtyRect[2] = 0;
        stash->dirtyRect[3] = 0;
    }

    // Flush triangles
    if (stash->nverts > 0)
    {

        Font_renderDraw(stash->params.userPtr, stash->verts, stash->tcoords, stash->colors, stash->nverts);
        stash->nverts = 0;
    }
}
static unsigned int fons__hashint(unsigned int a)
{
    a += ~(a<<15);
    a ^=  (a>>10);
    a +=  (a<<3);
    a ^=  (a>>6);
    a += ~(a<<11);
    a ^=  (a>>16);
    return a;
}


static int Font_buildGlyphBitmap(stbtt_fontinfo *font, int glyph, float size, float scale,
                                 int *advance, int *lsb, int *x0, int *y0, int *x1, int *y1, const Font_Settings* sdfSettings)
{
    FONT_NOTUSED(size);
    stbtt_GetGlyphHMetrics(font, glyph, advance, lsb);
    stbtt_GetGlyphBitmapBox(font, glyph, scale, scale, x0, y0, x1, y1);

    if (sdfSettings->sdfEnabled)
    {
        *x0 -= sdfSettings->padding;
        *y0 -= sdfSettings->padding;
        *x1 += sdfSettings->padding;
        *y1 += sdfSettings->padding;
    }

    return 1;
}

static void Fonts_renderGlyphBitmap(stbtt_fontinfo *font, unsigned char *output, int outWidth, int outHeight, int outStride,
                                    float scaleX, float scaleY, int glyph, const Font_Settings* sdfSettings)
{
    if (!sdfSettings->sdfEnabled)
    {
        stbtt_MakeGlyphBitmap(font, output, outWidth, outHeight, outStride, scaleX, scaleY, glyph);
    }
    else
    {
        int w = 0, h = 0, xoff = 0, yoff = 0;
        unsigned char* sdfData = stbtt_GetGlyphSDF(font, scaleX, glyph, sdfSettings->padding, sdfSettings->onedgeValue, sdfSettings->pixelDistScale, &w, &h, &xoff, &yoff);

        for (int y = 0; y < h; y++)
        {
            unsigned char* outRow = output + (outStride*y);
            unsigned char* inRow = sdfData + (w*y);

            memCpy(outRow, inRow, w);
        }

        if (sdfData)
            stbtt_FreeSDF(sdfData, NULL);
    }
}






Font_glyph* Font_getGlyph(Font_main_ctx* stash, Font_font* font, unsigned int codepoint,
                          short isize, short iblur)
{
    int i, g, advance, lsb, x0, y0, x1, y1, gw, gh, gx, gy, x, y;
    float scale;
    Font_glyph* glyph = NULL;
    unsigned int h;
    float size = isize/10.0f;
    int pad, added;
    unsigned char* bdst;
    unsigned char* dst;
    Font_font* renderFont = font;

    if (isize < 2) return NULL;
    if (iblur > 20) iblur = 20;
    pad = iblur+2;

    // Reset allocator.
    stash->nscratch = 0;

    // Find code point and size.
    h = fons__hashint(codepoint) & (FONS_HASH_LUT_SIZE-1);
    i = font->lut[h];


    while (i != -1)
    {
        if (font->glyphs[i].codepoint == codepoint && font->glyphs[i].size == isize && font->glyphs[i].blur == iblur)
            return &font->glyphs[i];
        i = font->glyphs[i].next;
    }

    // Could not find glyph, create it.
    g = stbtt_FindGlyphIndex(&font->font, codepoint);
    // Try to find the glyph in fallback fonts.
    if (g == 0) {
        for (i = 0; i < font->nfallbacks; ++i) {
            Font_font* fallbackFont = stash->fonts[font->fallbacks[i]];
            int fallbackIndex = stbtt_FindGlyphIndex(&fallbackFont->font, codepoint);
            if (fallbackIndex != 0) {
                g = fallbackIndex;
                renderFont = fallbackFont;
                break;
            }
        }
        // It is possible that we did not find a fallback glyph.
        // In that case the glyph index 'g' is 0, and we'll proceed below and cache empty glyph.
    }
    scale = stbtt_ScaleForPixelHeight(&renderFont->font, size);
    Font_buildGlyphBitmap(&renderFont->font, g, size, scale, &advance, &lsb, &x0, &y0, &x1, &y1, &renderFont->sdfSettings);
    gw = x1-x0 + pad*2;
    gh = y1-y0 + pad*2;

    // Find free spot for the rect in the atlas

    added = font_atlas_AddRect(stash->atlas, gw, gh, &gx, &gy);
    if (added == 0 && stash->handleError != NULL) {
        // Atlas is full, let the user to resize the atlas (or not), and try again.
        stash->handleError(stash->errorUptr, FONT_ATLAS_FULL, 0);
        added = font_atlas_AddRect(stash->atlas, gw, gh, &gx, &gy);
    }
    if (added == 0) return NULL;

    // Init glyph.
    glyph = Font_allocGlyph(font);
    glyph->codepoint = codepoint;
    glyph->size = isize;
    glyph->blur = iblur;
    glyph->index = g;
    glyph->x0 = (short)gx;
    glyph->y0 = (short)gy;
    glyph->x1 = (short)(glyph->x0+gw);
    glyph->y1 = (short)(glyph->y0+gh);
    glyph->xadv = (short)(scale * advance * 10.0f);
    glyph->xoff = (short)(x0 - pad);
    glyph->yoff = (short)(y0 - pad);
    glyph->next = 0;

    // Insert char to hash lookup.
    glyph->next = font->lut[h];
    font->lut[h] = font->nglyphs-1;

    // Rasterize
    dst = &stash->texData[(glyph->x0+pad) + (glyph->y0+pad) * stash->params.width];

    Fonts_renderGlyphBitmap(&renderFont->font, dst, gw-pad*2,gh-pad*2, stash->params.width, scale,scale, g, &renderFont->sdfSettings);

    // Make sure there is one pixel empty border.
    dst = &stash->texData[glyph->x0 + glyph->y0 * stash->params.width];
    for (y = 0; y < gh; y++) {
        dst[y*stash->params.width] = 0;
        dst[gw-1 + y*stash->params.width] = 0;
    }
    for (x = 0; x < gw; x++) {
        dst[x] = 0;
        dst[x + (gh-1)*stash->params.width] = 0;
    }

    // Debug code to color the glyph background
    /*	unsigned char* fdst = &stash->texData[glyph->x0 + glyph->y0 * stash->params.width];
    for (y = 0; y < gh; y++) {
        for (x = 0; x < gw; x++) {
            int a = (int)fdst[x+y*stash->params.width] + 20;
            if (a > 255) a = 255;
            fdst[x+y*stash->params.width] = a;
        }
    }*/

    // Blur
    //    if (iblur > 0) {
    //        stash->nscratch = 0;
    //        bdst = &stash->texData[glyph->x0 + glyph->y0 * stash->params.width];
    //        fons__blur(stash, bdst, gw,gh, stash->params.width, iblur);
    //    }

    stash->dirtyRect[0] = min_i(stash->dirtyRect[0], glyph->x0);
    stash->dirtyRect[1] = min_i(stash->dirtyRect[1], glyph->y0);
    stash->dirtyRect[2] = max_i(stash->dirtyRect[2], glyph->x1);
    stash->dirtyRect[3] = max_i(stash->dirtyRect[3], glyph->y1);

    return glyph;
}

int  Font_allocFont(Font_main_ctx* stash)
{
    Font_font* font = NULL;

    if (stash->nfonts+1 > stash->cfonts) {
        stash->cfonts = stash->cfonts == 0 ? 8 : stash->cfonts * 2;
        stash->fonts = (Font_font**)realloc(stash->fonts, sizeof(Font_font*) * stash->cfonts);
        if (stash->fonts == NULL)
            return -1;
    }



    font = (Font_font*)malloc(sizeof(Font_font));
    if (font == NULL) goto error;
    memSet2(font, 0, sizeof(Font_font));

    font->glyphs = (Font_glyph*)malloc(sizeof(Font_glyph) * FONS_INIT_GLYPHS);
    if (font->glyphs == NULL) goto error;
    font->cglyphs = FONS_INIT_GLYPHS;
    font->nglyphs = 0;

    stash->fonts[stash->nfonts++] = font;
    return stash->nfonts-1;

error:
    Font_font_free(font);

    return -1;

}


Font_glyph * Font_allocGlyph(Font_font* font)
{
    if (font->nglyphs+1 > font->cglyphs) {
        font->cglyphs = font->cglyphs == 0 ? 8 : font->cglyphs * 2;
        font->glyphs = (Font_glyph*)realloc(font->glyphs, sizeof(Font_glyph) * font->cglyphs);
        if (font->glyphs == NULL) return NULL;
    }
    font->nglyphs++;
    return &font->glyphs[font->nglyphs-1];
}






static void Font_addWhiteRect(Font_main_ctx* stash, int w, int h)
{
    int x, y, gx, gy;
    unsigned char* dst;
    if (font_atlas_AddRect(stash->atlas, w, h, &gx, &gy) == 0)
        return;

    // Rasterize
    dst = &stash->texData[gx + gy * stash->params.width];
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++)
            dst[x] = 0xff;
        dst += stash->params.width;
    }

    stash->dirtyRect[0] = min_i(stash->dirtyRect[0], gx);
    stash->dirtyRect[1] = min_i(stash->dirtyRect[1], gy);
    stash->dirtyRect[2] = max_i(stash->dirtyRect[2], gx+w);
    stash->dirtyRect[3] = max_i(stash->dirtyRect[3], gy+h);
}

void Font_PushState(Font_main_ctx* stash)
{

    SDL_Log("void fonsPushState(FONScontext* stash)");

    if (stash->nstates >= FONS_MAX_STATES) {
        if (stash->handleError)
        {
            stash->handleError(stash->errorUptr, FONT_STATES_OVERFLOW, 0);
            SDL_Log("stash->handleError");
        }
        return;
    }
    if (stash->nstates > 0)
        memCpy(&stash->states[stash->nstates], &stash->states[stash->nstates-1], sizeof(Font_state));
    stash->nstates++;
    SDL_Log("pushed");
}

static Font_state* Font_getState(Font_main_ctx* stash)
{
    return &stash->states[stash->nstates-1];
}

void Font_SetAlign(Font_main_ctx* stash, int align)
{

    Font_getState(stash)->align = align;
}

void Font_SetFont(Font_main_ctx* stash, int font)
{
    Font_getState(stash)->font = font;
}

void Font_SetSize(Font_main_ctx* stash, float size)
{
    Font_getState(stash)->size = size;
}

void Font_SetColor(Font_main_ctx* stash, unsigned int color)
{
    Font_getState(stash)->color = color;
}

void Font_VertMetrics(Font_main_ctx*  stash,
                      float* ascender, float* descender, float* lineh)
{
    Font_font* font;
    Font_state* state =Font_getState(stash);
    short isize;

    if (stash == NULL) return;
    if (state->font < 0 || state->font >= stash->nfonts) return;
    font = stash->fonts[state->font];
    isize = (short)(state->size*10.0f);
    if (font->data == NULL) return;

    if (ascender)
        *ascender = font->ascender*isize/10.0f;
    if (descender)
        *descender = font->descender*isize/10.0f;
    if (lineh)
        *lineh = font->lineh*isize/10.0f;
}



void Font_ClearState(Font_main_ctx* stash)
{
    Font_state* state = Font_getState(stash);
    state->size = 8.0f;
    state->color = 0xffffffff;
    state->font = 0;
    state->blur = 0;
    state->spacing = 0;
    state->align = FONT_ALIGN_LEFT | FONT_ALIGN_BASELINE;
}


void Font_PopState(Font_main_ctx* stash)
{
    if (stash->nstates <= 1)
    {
        if (stash->handleError)
            stash->handleError(stash->errorUptr, FONT_STATES_UNDERFLOW, 0);
        return;
    }
    stash->nstates--;
}



Font_main_ctx *font_main_create(Font_parameters *params)
{
    Font_main_ctx* stash = NULL;

    // Allocate memory for the font stash.
    stash = (Font_main_ctx*)malloc(sizeof(Font_main_ctx));
    if (stash == NULL) goto error;

    SDL_Log(" FONScontext* fonsCreateInternal(FONSparams* params) ");
    memSet2(stash, 0, sizeof(Font_main_ctx));
    SDL_Log("++fonsCreateInternal memset 1");
    stash->params = *params;

    // Allocate scratch buffer.
    stash->scratch = (unsigned char*)malloc(FONS_SCRATCH_BUF_SIZE);
    if (stash->scratch == NULL) goto error;
    SDL_Log("++2");
    // Initialize implementation library




    if (Font_main_renderCreate(stash->params.userPtr,  stash->params.width,  stash->params.height)==0  )
    {
        SDL_Log("++Font_main_renderCreate error;");
        goto error;
    }

    SDL_Log("++fonsCreateInternal alloc atlas ");

    stash->atlas = font_atlas_create(stash->params.width, stash->params.height, FONS_INIT_ATLAS_NODES);
    if (stash->atlas == NULL) goto error;

    // Allocate space for fonts.
    stash->fonts = (Font_font**)malloc(sizeof(Font_font*) * FONS_INIT_FONTS);
    if (stash->fonts == NULL) goto error;
    SDL_Log("++fonsCreateInternal memset 2 ");
    memSet2(stash->fonts, 0, sizeof(Font_font*) * FONS_INIT_FONTS);
    stash->cfonts = FONS_INIT_FONTS;
    stash->nfonts = 0;

    // Create texture for the cache.
    stash->itw = 1.0f/stash->params.width;
    stash->ith = 1.0f/stash->params.height;
    stash->texData = (unsigned char*)malloc(stash->params.width * stash->params.height);
    if (stash->texData == NULL) goto error;
    memSet2(stash->texData, 0, stash->params.width * stash->params.height);

    stash->dirtyRect[0] = stash->params.width;
    stash->dirtyRect[1] = stash->params.height;
    stash->dirtyRect[2] = 0;
    stash->dirtyRect[3] = 0;

    // Add white rect at 0,0 for debug drawing.
    Font_addWhiteRect(stash, 2,2);

    Font_PushState(stash);
    Font_ClearState(stash);

    return stash;

error:
    font_main_free(stash);
    return NULL;

}


int Font_ResetAtlas(Font_main_ctx* stash, int width, int height)
{
    int i, j;
    if (stash == NULL) return 0;

    // Flush pending glyphs.
    Font_flush(stash);

    // Create new texture
    {
        if (Font_main_renderCreate(stash->params.userPtr, width, height) == 0)
            return 0;
    }

    // Reset atlas
    font_atlas_Reset(stash->atlas, width, height);

    // Clear texture data.
    stash->texData = (unsigned char*)realloc(stash->texData, width * height);
    if (stash->texData == NULL) return 0;
    memSet2(stash->texData, 0, width * height);

    // Reset dirty rect
    stash->dirtyRect[0] = width;
    stash->dirtyRect[1] = height;
    stash->dirtyRect[2] = 0;
    stash->dirtyRect[3] = 0;

    // Reset cached glyphs
    for (i = 0; i < stash->nfonts; i++) {
        Font_font* font = stash->fonts[i];
        font->nglyphs = 0;
        for (j = 0; j < FONS_HASH_LUT_SIZE; j++)
            font->lut[j] = -1;
    }

    stash->params.width = width;
    stash->params.height = height;
    stash->itw = 1.0f/stash->params.width;
    stash->ith = 1.0f/stash->params.height;

    // Add white rect at 0,0 for debug drawing.
    Font_addWhiteRect(stash, 2,2);

    return 1;
}


void font_main_free(Font_main_ctx *stash)
{

    int i;
    if (stash == NULL) return;

    Font_main_renderDelete(stash->params.userPtr);

    for (i = 0; i < stash->nfonts; ++i)
        Font_font_free(stash->fonts[i]);

    if (stash->atlas) font_atlas_free(stash->atlas);
    if (stash->fonts) free(stash->fonts);
    if (stash->texData) free(stash->texData);
    if (stash->scratch) free(stash->scratch);
    free(stash);

}

static size_t strLen(const char *str)
{
    const char *s;

    for (s = str; *s; ++s)
        ;
    return (s - str);
}



static float Font_getVertAlign(Font_main_ctx* stash, Font_font* font, int align, short isize)
{
    if (stash->params.flags & FONT_ZERO_TOPLEFT) {
        if (align & FONT_ALIGN_TOP) {
            return font->ascender * (float)isize/10.0f;
        } else if (align & FONT_ALIGN_MIDDLE) {
            return (font->ascender + font->descender) / 2.0f * (float)isize/10.0f;
        } else if (align & FONT_ALIGN_BASELINE) {
            return 0.0f;
        } else if (align & FONT_ALIGN_BOTTOM) {
            return font->descender * (float)isize/10.0f;
        }
    } else {
        if (align & FONT_ALIGN_TOP) {
            return -font->ascender * (float)isize/10.0f;
        } else if (align & FONT_ALIGN_MIDDLE) {
            return -(font->ascender + font->descender) / 2.0f * (float)isize/10.0f;
        } else if (align & FONT_ALIGN_BASELINE) {
            return 0.0f;
        } else if (align & FONT_ALIGN_BOTTOM) {
            return -font->descender * (float)isize/10.0f;
        }
    }
    return 0.0;
}



#define FONS_UTF8_ACCEPT 0
#define FONS_UTF8_REJECT 12

static unsigned int Font_decutf8(unsigned int* state, unsigned int* codep, unsigned int byte)
{
    static const unsigned char utf8d[] = {
        // The first part of the table maps bytes to character classes that
        // to reduce the size of the transition table and create bitmasks.
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,

        // The second part is a transition table that maps a combination
        // of a state of the automaton and a character class to a state.
        0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
        12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
        12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
        12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
        12,36,12,12,12,12,12,12,12,12,12,12,
    };

    unsigned int type = utf8d[byte];

    *codep = (*state != FONS_UTF8_ACCEPT) ?
                (byte & 0x3fu) | (*codep << 6) :
                (0xff >> type) & (byte);

    *state = utf8d[256 + *state + type];
    return *state;
}

static void Font_getQuad(Font_main_ctx* stash,Font_font* font,
                         int prevGlyphIndex, Font_glyph* glyph,
                         float scale, float spacing, float* x, float* y, Font_quad* q)
{
    float rx,ry,xoff,yoff,x0,y0,x1,y1;

    if (prevGlyphIndex != -1) {
        float adv = stbtt_GetGlyphKernAdvance(&font->font, prevGlyphIndex, glyph->index) * scale;
        *x += (int)(adv + spacing + 0.5f);
    }

    // Each glyph has 2px border to allow good interpolation,
    // one pixel to prevent leaking, and one to allow good interpolation for rendering.
    // Inset the texture region by one pixel for correct interpolation.
    xoff = (short)(glyph->xoff+1);
    yoff = (short)(glyph->yoff+1);
    x0 = (float)(glyph->x0+1);
    y0 = (float)(glyph->y0+1);
    x1 = (float)(glyph->x1-1);
    y1 = (float)(glyph->y1-1);

    if (stash->params.flags & FONT_ZERO_TOPLEFT) {
        rx = (float)(int)(*x + xoff);
        ry = (float)(int)(*y + yoff);

        q->x0 = rx;
        q->y0 = ry;
        q->x1 = rx + x1 - x0;
        q->y1 = ry + y1 - y0;

        q->s0 = x0 * stash->itw;
        q->t0 = y0 * stash->ith;
        q->s1 = x1 * stash->itw;
        q->t1 = y1 * stash->ith;
    } else {
        rx = (float)(int)(*x + xoff);
        ry = (float)(int)(*y - yoff);

        q->x0 = rx;
        q->y0 = ry;
        q->x1 = rx + x1 - x0;
        q->y1 = ry - y1 + y0;

        q->s0 = x0 * stash->itw;
        q->t0 = y0 * stash->ith;
        q->s1 = x1 * stash->itw;
        q->t1 = y1 * stash->ith;
    }

    *x += (int)(glyph->xadv / 10.0f + 0.5f);
}


void Font_GetAtlasSize(Font_main_ctx* stash, int* width, int* height)
{
    if (stash == NULL) return;
    *width = stash->params.width;
    *height = stash->params.height;
}




int Font_ExpandAtlas(Font_main_ctx* stash, int width, int height)
{
    int i, maxy = 0;
    unsigned char* data = NULL;
    if (stash == NULL) return 0;

    width =  max_i(width, stash->params.width);
    height =  max_i(height, stash->params.height);

    if (width == stash->params.width && height == stash->params.height)
        return 1;

    // Flush pending glyphs.
    Font_flush(stash);

    // Create new texture

    if (Font_main_renderCreate(stash->params.userPtr, width, height) == 0)
    {
        return 0;
    }
    // Copy old texture data over.
    data = (unsigned char*)malloc(width * height);
    if (data == NULL)
        return 0;
    for (i = 0; i < stash->params.height; i++)
    {
        unsigned char* dst = &data[i*width];
        unsigned char* src = &stash->texData[i*stash->params.width];
        memCpy(dst, src, stash->params.width);
        if (width > stash->params.width)
            memSet2(dst+stash->params.width, 0, width - stash->params.width);
    }
    if (height > stash->params.height)
        memSet2(&data[stash->params.height * width], 0, (height - stash->params.height) * width);

    free(stash->texData);
    stash->texData = data;

    // Increase atlas size
    font_atlas_Expand(stash->atlas, width, height);

    // Add existing data as dirty.
    for (i = 0; i < stash->atlas->nnodes; i++)
        maxy = max_i(maxy, stash->atlas->nodes[i].y);
    stash->dirtyRect[0] = 0;
    stash->dirtyRect[1] = 0;
    stash->dirtyRect[2] = stash->params.width;
    stash->dirtyRect[3] = maxy;

    stash->params.width = width;
    stash->params.height = height;
    stash->itw = 1.0f/stash->params.width;
    stash->ith = 1.0f/stash->params.height;

    return 1;
}



float Font_TextBounds(Font_main_ctx* stash,
                      float x, float y,
                      const char* str, const char* end,
                      float* bounds)
{
    Font_state* state = Font_getState( stash);
    unsigned int codepoint;
    unsigned int utf8state = 0;
    Font_quad q;
    Font_glyph* glyph = NULL;
    int prevGlyphIndex = -1;
    short isize = (short)(state->size*10.0f);
    short iblur = (short)state->blur;
    float scale;
    Font_font* font;
    float startx, advance;
    float minx, miny, maxx, maxy;

    if (stash == NULL) return 0;
    if (state->font < 0 || state->font >= stash->nfonts) return 0;
    font = stash->fonts[state->font];
    if (font->data == NULL) return 0;

    scale = stbtt_ScaleForPixelHeight(&font->font, (float)isize/10.0f);

    // Align vertically.
    y += Font_getVertAlign(stash, font, state->align, isize);

    minx = maxx = x;
    miny = maxy = y;
    startx = x;

    if (end == NULL)
        end = str + strLen(str);

    for (; str != end; ++str) {
        if (Font_decutf8(&utf8state, &codepoint, *(const unsigned char*)str))
            continue;
        glyph = Font_getGlyph(stash, font, codepoint, isize, iblur);
        if (glyph != NULL) {
            Font_getQuad(stash, font, prevGlyphIndex, glyph, scale, state->spacing, &x, &y, &q);
            if (q.x0 < minx) minx = q.x0;
            if (q.x1 > maxx) maxx = q.x1;
            if (stash->params.flags & FONT_ZERO_TOPLEFT) {
                if (q.y0 < miny) miny = q.y0;
                if (q.y1 > maxy) maxy = q.y1;
            } else {
                if (q.y1 < miny) miny = q.y1;
                if (q.y0 > maxy) maxy = q.y0;
            }
        }
        prevGlyphIndex = glyph != NULL ? glyph->index : -1;
    }

    advance = x - startx;

    // Align horizontally
    if (state->align & FONT_ALIGN_LEFT) {
        // empty
    } else if (state->align & FONT_ALIGN_RIGHT) {
        minx -= advance;
        maxx -= advance;
    } else if (state->align & FONT_ALIGN_CENTER) {
        minx -= advance * 0.5f;
        maxx -= advance * 0.5f;
    }

    if (bounds) {
        bounds[0] = minx;
        bounds[1] = miny;
        bounds[2] = maxx;
        bounds[3] = maxy;
    }

    return advance;
}










static inline void Font_add_vertex(Font_main_ctx * stash, float x, float y, float s, float t, unsigned int c)
{
    stash->verts[stash->nverts*2+0] = x;
    stash->verts[stash->nverts*2+1] = y;
    stash->tcoords[stash->nverts*2+0] = s;
    stash->tcoords[stash->nverts*2+1] = t;
    stash->colors[stash->nverts] = c;
    stash->nverts++;
}



void Font_DrawText(Font_main_ctx* stash,
                   float x, float y,
                   const char* str, const char* end)
{

    //  SDL_Log("Drawing1");
    Font_state* state = Font_getState(stash);
    unsigned int codepoint;
    unsigned int utf8state = 0;

    //  SDL_Log("Drawing1-1");
    Font_glyph* glyph = NULL;
    Font_quad q;
    int prevGlyphIndex = -1;
    short isize = (short)(state->size*10.0f);
    short iblur = (short)state->blur;
    float scale;
    Font_font* font;
    float width;
    //SDL_Log("Drawing1-2");
    if (stash == NULL) return ;
    if (state->font < 0 || state->font >= stash->nfonts) return ;
    font = stash->fonts[state->font];
    if (font->data == NULL) return ;
    //SDL_Log("Drawing1-3");
    scale = stbtt_ScaleForPixelHeight(&font->font, (float)isize/10.0f);

    if (end == NULL)
        end = str + strLen(str);
    //  SDL_Log("Drawing2");
    // Align horizontally
    if (state->align & FONT_ALIGN_LEFT) {
        // empty
    } else if (state->align & FONT_ALIGN_RIGHT) {
        width = Font_TextBounds(stash, x,y, str, end, NULL);
        x -= width;
    } else if (state->align & FONT_ALIGN_CENTER) {
        width = Font_TextBounds(stash, x,y, str, end, NULL);
        x -= width * 0.5f;
    }
    // Align vertically.
    y += Font_getVertAlign(stash, font, state->align, isize);
    // SDL_Log("Drawing3");
    for (; str != end; ++str) {
        if (Font_decutf8(&utf8state, &codepoint, *(const unsigned char*)str))
            continue;
        glyph = Font_getGlyph(stash, font, codepoint, isize, iblur);
        if (glyph != NULL) {
            Font_getQuad(stash, font, prevGlyphIndex, glyph, scale, state->spacing, &x, &y, &q);

            //           SDL_Log("Drawing4");
            if (stash->nverts+6 > FONS_VERTEX_COUNT)
                Font_flush(stash);

            Font_add_vertex(stash, q.x0, q.y0, q.s0, q.t0, state->color);
            Font_add_vertex(stash, q.x1, q.y1, q.s1, q.t1, state->color);
            Font_add_vertex(stash, q.x1, q.y0, q.s1, q.t0, state->color);

            Font_add_vertex(stash, q.x0, q.y0, q.s0, q.t0, state->color);
            Font_add_vertex(stash, q.x0, q.y1, q.s0, q.t1, state->color);
            Font_add_vertex(stash, q.x1, q.y1, q.s1, q.t1, state->color);
        }
        prevGlyphIndex = glyph != NULL ? glyph->index : -1;
    }

    //    SDL_Log("Drawing5");
    Font_flush(stash);

    return  ;
}

static char* strnCpy(char* destination, const char* source, size_t num)
{
    // return if no memory is allocated to the destination
    if (destination == NULL)
        return NULL;

    // take a pointer pointing to the beginning of destination string
    char* ptr = destination;

    // copy first num characters of C-string pointed by source
    // into the array pointed by destination
    while (*source && num--)
    {
        *destination = *source;
        destination++;
        source++;
    }

    // null terminate destination string
    *destination = '\0';

    // destination is returned by standard strncpy()
    return ptr;
}


#define FONT_NOTUSED(v)  (void)sizeof(v)
static int Font_loadFont(Font_main_ctx *context, stbtt_fontinfo *font, unsigned char *data, int dataSize)
{
    int stbError;
    FONT_NOTUSED(dataSize);

    font->userdata = context;
    stbError = stbtt_InitFont(font, data, 0);
    return stbError;
}



int Font_AddFontSdfMem(Font_main_ctx* stash, const char* name, unsigned char* data, int dataSize, int freeData, Font_Settings sdfSettings)
{
    int i, ascent, descent, fh, lineGap;
    Font_font* font;

    int idx = Font_allocFont(stash);
    if (idx == -1)
        return -1;

    font = stash->fonts[idx];
    font->sdfSettings = sdfSettings;


    SDL_Log("add font strncpy");
    strnCpy(font->name, name, sizeof(font->name));
    font->name[sizeof(font->name)-1] = '\0';
    SDL_Log("add font strncpy done \n");
    // Init hash lookup.
    for (i = 0; i < FONS_HASH_LUT_SIZE; ++i)
        font->lut[i] = -1;

    // Read in the font data.
    font->dataSize = dataSize;
    font->data = data;
    font->freeData = (unsigned char)freeData;

    // Init font
    stash->nscratch = 0;
    if (!Font_loadFont(stash, &font->font, data, dataSize)) goto error;

    // Store normalized line height. The real line height is got
    // by multiplying the lineh by font size.
    stbtt_GetFontVMetrics( &font->font, &ascent, &descent, &lineGap);
    fh = ascent - descent;
    font->ascender = (float)ascent / (float)fh;
    font->descender = (float)descent / (float)fh;
    font->lineh = (float)(fh + lineGap) / (float)fh;

    return idx;

error:
    Font_font_free(font);
    stash->nfonts--;
    return -1;
}

