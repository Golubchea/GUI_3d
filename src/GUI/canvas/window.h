#ifndef WINDOW_H
#define WINDOW_H




#ifdef __cplusplus
extern "C" {
#endif

#include "SDL.h"
#include "okplatform.h"
#include <stdbool.h>



#if OKPLATFORM_EMSCRIPTEN



#define GL_GLEXT_PROTOTYPES 1
//#include <SDL_opengles2.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "glad/glad.h"
#include "glad/khrplatform.h"
#endif


#ifdef WINDOW_STATIC
#define WINDOW_API static
#else
#define WINDOW_API extern
#endif

typedef enum Platform_t
{
    PLATFORM_UNKNOWN=0,
    PLATFORM_UNIX64 =1,
    PLATFORM_WIN64 =2,
    PLATFORM_MACOS=3,
    PLATFORM_IOS=4,
    PLATFORM_ANDROID=5,
    PLATFORM_EMSCRIPTEN=6
}Platform;


typedef struct Window_t
{
    SDL_Window* sdlWindow;
    SDL_GLContext sdlGLContext;
    Platform platform;
    bool isRunning;
    GLint glMajorVersion;
    GLint glMinorVersion;
    
}Window;

WINDOW_API Window *window_create(const char* name ,int w,int h);
WINDOW_API void window_free(Window * win);


WINDOW_API uint32_t window_getTimerMillis();
WINDOW_API uint64_t window_getTimerMicros();

WINDOW_API bool window_screencapture(Window *win);

#ifdef __cplusplus
}
#endif


#endif // ifndef OKWRAPPER_OKAPP_H_
