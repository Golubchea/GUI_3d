#include "Font.h"


#include "font/DataIO.h"
//#define FONTSTASH_IMPLEMENTATION
//#define GLFONTSTASH_IMPLEMENTATION

void fontStashResetAtlas(Font_main_ctx* stash, int width, int height) {
    Font_ResetAtlas(stash, width, height);
    SDL_Log( "reset atlas to %d x %d", width, height);
}

static GLint getInt(GLenum pname) {
    GLint value;
    glGetIntegerv(pname, &value);
    return value;
}


void fontStashEexpandAtlas(Font_main_ctx* stash) {
    int w = 0, h = 0;
    const int maxTexturesize = getInt(GL_MAX_TEXTURE_SIZE);

    Font_GetAtlasSize(stash, &w, &h);
    if (w < h) {
        w *= 2;
    } else {
        h *= 2;
    }

    if (w > maxTexturesize || h > maxTexturesize) {
        fontStashResetAtlas(stash, maxTexturesize, maxTexturesize);
    } else {
        Font_ExpandAtlas(stash, w, h);
        SDL_Log( "expanded atlas to %d x %d", w, h);
    }
}


void fontStashError(void* s , int error, int value)
{
Font_main_ctx* stash=(Font_main_ctx*)s;
    switch (error) {
    case FONT_ATLAS_FULL:
        SDL_Log(  "Font atlas full.");
        fontStashEexpandAtlas(stash);
        break;
    case FONT_SCRATCH_FULL:
        SDL_Log("Font error: scratch full, tried to allocate %d", value);
        break;
    case FONT_STATES_OVERFLOW:
        SDL_Log( "Font error: states overflow.");
        break;
    case FONT_STATES_UNDERFLOW:
        SDL_Log( "Font error: states underflow.");
        break;
    default:
        SDL_Log( "Font error: unknown.");
        break;
    }
}


 void fonsSetErrorCallback(Font_main_ctx* stash, void (*callback)(void* uptr, int error, int val), void* uptr)
{
    if (stash == NULL) return;
    stash->handleError = callback;
    stash->errorUptr = uptr;
}


Font_main_ctx* glfonsCreate(int width, int height, int flags,
                            int (*renderCreate)(void* uptr, int width, int height) ,
                            void (*renderUpdate)(void* uptr, int* rect, const unsigned char* data),
                            void (*renderDraw)(void* uptr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts),
                            void (*renderDelete)(void* uptr))
 {
     Font_parameters params;
     GLcontext* gl;
     SDL_Log("glfonsCreate(int width, int height, int flags)");
     gl = (GLcontext*)malloc(sizeof(GLcontext));
     if (gl == NULL) goto error;

     SDL_Log(" memset( 1)");
     memSet( gl, (int32_t)0, (int32_t)sizeof(GLcontext));

     memSet( (uint32_t*)gl, 0, (uint32_t)0);

     SDL_Log("memset(2)");
     memSet(&params, 0, (int32_t)sizeof(params));
     params.width = width;
     params.height = height;
     params.flags = (unsigned char)flags;


     gl->renderCreate=renderCreate;
     gl->renderUpdate=renderUpdate;
     gl->renderDraw=renderDraw;
     gl->renderDelete=renderDelete;
     params.userPtr = gl;
     SDL_Log(" params setted");
     return font_main_create(&params);

 error:
     if (gl != NULL) free(gl);
     return NULL;
 }



FONTS_API Font *Font_create(const char* fontPath_TTF,
                            const char* fontPathICONS_TTF,
                            int (*renderCreate)(void* uptr, int width, int height) ,
                            void (*renderUpdate)(void* uptr, int* rect, const unsigned char* data),
                            void (*renderDraw)(void* uptr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts),
                            void (*renderDelete)(void* uptr) )
{
    // Make sure to release old fonts first.
    //releaseFonts();
    SDL_Log("loading fonts");
    Font *out =(Font *)malloc(sizeof (Font ));
    //
    // Initialize fontstash.
    //
    SDL_Log("+malloc");
    out->lineHeight=0.0;
    out->fs=glfonsCreate(512, 512, FONT_ZERO_TOPLEFT,renderCreate,renderUpdate,renderDraw,renderDelete);

    if (out->fs == NULL) {
        SDL_Log( "Could not create font stash.");
        return NULL;
    }
    SDL_Log("glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);");
    fonsSetErrorCallback(out->fs, fontStashError, out->fs);
    SDL_Log("set callback");
    //
    // Load font data.
    //

    SDL_Log("loading droid");




    out->fontDataDroidSans = NULL;
    int fontDataDroidSansSize = dataIO_loadBinaryFile(fontPath_TTF, &out->fontDataDroidSans);
    if(fontDataDroidSansSize==-1)
    {
        SDL_Log( " cant load %s",fontPath_TTF  );
    }
    SDL_Log( " reading font file size : '%d' bytes",fontDataDroidSansSize  );
    if (fontDataDroidSansSize <= 0) {
        SDL_Log( "Error reading font file: '%s'", fontPath_TTF);
        return NULL;
    }
    SDL_Log("loading droid2");

    out->fontDataDroidIcons = NULL;
    int fontDataDroidIconSize = dataIO_loadBinaryFile(fontPathICONS_TTF, &out->fontDataDroidIcons);
    SDL_Log( " reading font file size : '%d' bytes",fontDataDroidIconSize  );
    if (fontDataDroidIconSize <= 0) {
        SDL_Log( "Error reading font file: '%s'", fontPathICONS_TTF);
        return NULL;
    }


    //
    // Create fonts.
    //

    // Note that we tell fontstash to not free the memory after it's done with the font, because we reuse the
    // data for multiple fonts.
    int callFree = 0;

    SDL_Log("set fonts params");
    Font_Settings basicSdf = {0};
    basicSdf.sdfEnabled = 1;
    basicSdf.onedgeValue = 142;
    basicSdf.padding = 1;
    basicSdf.pixelDistScale = 62.0;


    Font_Settings basicSdf2 = {0};
    basicSdf.sdfEnabled = 1;
    basicSdf.onedgeValue = 142;
    basicSdf.padding = 1;
    basicSdf.pixelDistScale = 62.0;
    SDL_Log("creating fonts 1");




    out->fontSdf = Font_AddFontSdfMem(out->fs, "DroidSansSdf", out->fontDataDroidSans, fontDataDroidSansSize, callFree, basicSdf);
    if (out->fontSdf ==-1) {
        SDL_Log( "Could not add SDF font.");
        return NULL;
    }

    SDL_Log("creating fonts 2");
    out->fontIcon = Font_AddFontSdfMem(out->fs, "ICONS", out->fontDataDroidIcons, fontDataDroidIconSize, callFree, basicSdf2);
    if (out->fontIcon==-1) {
        SDL_Log( "Could not add SDF font.");
        return NULL;
    }



    return out;
}




FONTS_API void Font_free(Font *f)
{
    if (f->fs)
    {
         font_main_free(f->fs);
        f->fs = NULL;
    }

    free(f->fontDataDroidSans);
    f->fontDataDroidSans = NULL;

    free(f->fontDataDroidIcons);
    f->fontDataDroidIcons = NULL;

    free(f);
}
