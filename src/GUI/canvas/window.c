#include "window.h"


#include <stdio.h>

#include <stdlib.h>
#include <memory.h>
#include <time.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../button/stb_image_write.h"


#if OKPLATFORM_EMSCRIPTEN

#define GL_GLEXT_PROTOTYPES 1
//#include <SDL_opengles2.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "glad/glad.h"
#include "glad/khrplatform.h"
#endif


WINDOW_API Window *window_create(const char* name ,int w,int h)
{
    Window * win =(Window*)malloc(sizeof (Window));


    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        SDL_ClearError();
        return 0;
    }

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);

    win->sdlWindow = SDL_CreateWindow(name ,
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      w, h,
                                      SDL_WINDOW_BORDERLESS|SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    win->sdlGLContext = SDL_GL_CreateContext(win->sdlWindow);

    int size=sizeof (Window);
    char str[10];

    sprintf(str, "%d", size);

    SDL_Log("Window created size: %s bytes ", str);


    //platfom check
#if OKPLATFORM_LINUX==1
    SDL_Log("platform: unix");
    win->platform=PLATFORM_UNIX64;
#elif OKPLATFORM_IOS==1
    SDL_Log("platform: IOS");
#ifdef __APPLE__
#  define off64_t off_t
#  define fopen64 fopen
    //#  define GLFONTSTASH_IMPLEMENTATION_ES2
    //#  define OKGL_OPENGL_ES 1
#endif
    win->platform=PLATFORM_IOS;
#elif OKPLATFORM_MAC==1
    SDL_Log("platform: MACOS");
    win->platform=PLATFORM_MACOS;
#elif OKPLATFORM_ANDROID==1
    SDL_Log("platform: ANDROID");
    win->platform=PLATFORM_ANDROID;
#elif OKPLATFORM_EMSCRIPTEN==1
    SDL_Log("platform: EMSCRIPTEN");
    win->platform=PLATFORM_EMSCRIPTEN;




#elif OKPLATFORM_WINDOWS==1
    SDL_Log("platform: WINDOWS");
    win->platform=PLATFORM_WIN64;
#endif


    //    SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    //    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    //    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
    //    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // All values are "at least".

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
//SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);


    // Enable vsync.

#if OKPLATFORM_EMSCRIPTEN==1

#else
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    SDL_Log("OpenGL loaded\n");
    SDL_Log("Vendor:   %s\n", glGetString(GL_VENDOR));
    SDL_Log("Renderer: %s\n", glGetString(GL_RENDERER));
    SDL_Log("Version:  %s\n", glGetString(GL_VERSION));
    SDL_Log("-------------------");
#endif


    
    if(  win->platform==PLATFORM_IOS || win->platform==PLATFORM_ANDROID ||  win->platform==PLATFORM_EMSCRIPTEN)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

        win-> glMajorVersion=2; win->glMinorVersion=0;
    }
    else if(  win->platform==PLATFORM_MACOS||win->platform==PLATFORM_UNIX64|| win->platform==PLATFORM_WIN64)
    {
        GLint m;
        GLint n;
        const char* v=(const char*) glGetString(GL_VERSION);
        m=atoi(v);
        n=atoi((v+2));

        SDL_Log("ver %d",m);
        SDL_Log("    %d",n);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, n);

        win->glMajorVersion=m;
        win->glMinorVersion=n;



    }
    SDL_GL_SetSwapInterval(1);


    SDL_StopTextInput();
    SDL_CaptureMouse(SDL_TRUE);


    win->isRunning=true;

    return win;
}


WINDOW_API uint32_t window_getTimerMillis() {
    return SDL_GetTicks();
}

WINDOW_API uint64_t window_getTimerMicros() {
    return SDL_GetPerformanceCounter() * 1000000LL / SDL_GetPerformanceFrequency();
}

bool   window_screencapture(Window *win)
{
    int width, height;
    SDL_GetWindowSize(win->sdlWindow, &width, &height);
    //not save?possible leak
    unsigned char *buffer = (unsigned char *)malloc( width *  height * 4 );
    glReadPixels( 0, 0,  width,  height, GL_RGBA, GL_UNSIGNED_BYTE, buffer );
    char name[1024];
    long t =time(NULL);
    printf( " writing screenshot_%ld.png\n", t );
    sprintf( name, "screenshot_%ld.png", t );
    unsigned char *last_row = buffer + (  width * 4 * (  height - 1 ) );
    if ( !stbi_write_png( name,  width,  height, 4, last_row,
                          -4 *  width ) )
    {
        fprintf( stderr, "ERROR: could not write screenshot file %s\n", name );
    }
    free( buffer );
    return true;
}



WINDOW_API void window_free(Window *win)
{

    if (win->sdlGLContext) {
        SDL_GL_DeleteContext(win->sdlGLContext);
        win->sdlGLContext = NULL;
    }

    if (win->sdlWindow) {
        SDL_DestroyWindow(win->sdlWindow);
        win->sdlWindow = NULL;
    }
    free(win);
}
