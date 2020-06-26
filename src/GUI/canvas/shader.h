#ifndef SHADER_H
#define SHADER_H




#ifdef __cplusplus
extern "C" {
#endif
#include "SDL.h"
#include <math.h>


#include "window.h"
#ifdef SHADER_STATIC
#define SHADER_API static
#else
#define SHADER_API extern
#endif

typedef struct Shader_sdf_t
{
    GLuint shaderColored;
    //GLuint shaderText;
    GLuint shaderTextSdf ;
}Shader_sdf;



///
SHADER_API  char* shader_convert_vs_es2to3(const char* shaderString,
                                           Window *win) ;
 
SHADER_API  char* shader_convert_fs_es2to3(const char* shaderString,
                                           Window *win) ;
///
/// \brief shader_checkStatus
/// \param object
/// \param operation
/// \return
///
SHADER_API GLboolean shader_checkStatus(GLuint object, GLenum operation) ;

///
/// \brief shader_getVersion
/// \param glMajorVersion
/// \param glMinorVersion
/// \return
///
SHADER_API const char* shader_getVersion(GLint* glMajorVersion, GLint* glMinorVersion) ;
///
/// \brief shader_create
/// \param shaderString
/// \param shaderType
/// \return
///
SHADER_API  GLuint shader_create(const char* shaderString, GLenum shaderType) ;
///
/// \brief shader_linkProgram
/// \param vertexShaderString
/// \param fragmentShaderString
/// \return
///
SHADER_API GLuint shader_linkProgram1(const char* vertexShaderString, const char* fragmentShaderString,Window *win ) ;

SHADER_API Shader_sdf* shader_create_sdf(Window *win);
SHADER_API void shader_free_sdf(Shader_sdf *a);


SHADER_API void shader_release (GLuint shader);

#ifdef __cplusplus
}
#endif


#endif // ifndef
