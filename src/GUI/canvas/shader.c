#include "shader.h"
//#include "window.h"
//#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#if OKPLATFORM_EMSCRIPTEN
#define GL_GLEXT_PROTOTYPES 1
//#include <SDL_opengles2.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "glad/glad.h"
#include "glad/khrplatform.h"
#endif



SHADER_API char *shader_convert_vs_es2to3(const char *shaderString, Window *win)
{

    char* preamble = "";
    char* output;

    if (!shaderString) {
        return NULL;
    }

    // TODO: better mappings for different versions.

    //#if OKGL_OPENGL_ES
    if((win->platform==PLATFORM_IOS || win->platform==PLATFORM_ANDROID) || win->platform==PLATFORM_EMSCRIPTEN)
    {
        if (win->glMajorVersion == 2) {
            preamble =

                    "#version 100\n" \
                    "precision mediump float;\n" \
                    "#define OPENGL_ES\n" \
                    "#define OPENGL_ES2\n";

        } else if (win->glMajorVersion >= 3) {
            // Transform version 100 -> 300 es (Somewhat. Probably more stuff missing still).
            preamble =
                    "#version 300 es\n" \
                    "precision mediump float;\n" \
                    "#define OPENGL_ES\n" \
                    "#define OPENGL_ES3\n" \
                    "\n" \
                    "#define attribute in\n" \
                    "#define varying out\n\n";
        }

    }
    else
    {
        if (win->glMajorVersion == 2) {
            preamble =
                    "#version 120\n" \
                    "#define lowp\n" \
                    "#define mediump\n" \
                    "#define highp\n";

        } else if (win->glMajorVersion == 3) {
            // Transform version 120 -> 130 (Somewhat. Probably more stuff missing still).
            preamble =
                    "#version 130\n" \
                    "#define lowp\n" \
                    "#define mediump\n" \
                    "#define highp\n" \
                    "\n" \
                    "#define attribute in\n" \
                    "#define varying out\n\n";

        } else if (win->glMajorVersion >= 4) {
            // Transform version 120 -> 130 (Somewhat. Probably more stuff missing still).
            preamble =
                    "#version 400 core\n" \
                    "#define lowp\n" \
                    "#define mediump\n" \
                    "#define highp\n" \
                    "\n" \
                    "#define attribute in\n" \
                    "#define varying out\n\n";
        }
    }


    int outputSize = sizeof(char) * (strlen(preamble) + strlen(shaderString) + 1);
    output = (char*) malloc(sizeof(char) * outputSize);
    if (output) {
        strcpy(output, preamble);
        strcat(output, shaderString);
    }
    return output;
}

SHADER_API char *shader_convert_fs_es2to3(const char *shaderString,Window *win)
{
    char* preamble = "";
    char* output;

    if (!shaderString) {
        return NULL;
    }

    // TODO: better mappings for different versions.

    
    if((win->platform==PLATFORM_IOS || win->platform==PLATFORM_ANDROID) || win->platform==PLATFORM_EMSCRIPTEN)
    {
        if (win->glMajorVersion == 2) {
            preamble =
                    "#version 100\n" \
                    "#if defined(GL_OES_standard_derivatives)\n" \
                    "   #extension GL_OES_standard_derivatives : enable\n" \
                    " #endif\n" \

                    " #define SUPERSAMPLE \n" \
                    "precision mediump float;\n" \
                    "#define OPENGL_ES\n" \
                    "#define OPENGL_ES2\n";

        } else if (win->glMajorVersion >= 3) {
            // Transform version 100 -> 300 es (Somewhat. Probably more stuff missing still).
            preamble =
                    "#version 300 es\n" \
                    "precision mediump float;\n" \
                    "#define OPENGL_ES\n" \
                    "#define OPENGL_ES3\n" \
                    "\n" \
                    "out highp vec4 fragmentOut;\n" \
                    "\n" \
                    "#define varying in\n" \
                    "#define texture2D(a, b) texture(a, b)\n" \
                    "#define textureCube(a, b) texture(a, b)\n" \
                    "#define gl_FragColor fragmentOut\n";
        }

    }
    else
    {
        if (win->glMajorVersion == 2) {
            preamble =
                    "#version 120\n" \
                    "#define lowp\n" \
                    "#define mediump\n" \
                    "#define highp\n";

        } else if (win->glMajorVersion == 3) {
            // Transform version 120 -> 130 (Somewhat. Probably more stuff missing still).
            preamble =
                    "#version 130\n" \
                    "#define lowp\n" \
                    "#define mediump\n" \
                    "#define highp\n" \
                    "\n" \
                    "out vec4 fragmentOut;\n" \
                    "\n" \
                    "#define varying in\n" \
                    "#define texture2D(a, b) texture(a, b)\n" \
                    "#define textureCube(a, b) texture(a, b)\n" \
                    "#define gl_FragColor fragmentOut\n\n";

        } else if (win->glMajorVersion >= 4) {
            // Transform version 120 -> 400 (Somewhat. Probably more stuff missing still).
            preamble =
                    "#version 400 core\n" \
                    "#define lowp\n" \
                    "#define mediump\n" \
                    "#define highp\n" \
                    "\n" \
                    "out vec4 fragmentOut;\n" \
                    "\n" \
                    "#define varying in\n" \
                    "#define texture2D(a, b) texture(a, b)\n" \
                    "#define textureCube(a, b) texture(a, b)\n" \
                    "#define gl_FragColor fragmentOut\n\n";
        }
    }

    
    


    int outputSize = sizeof(char) * (strlen(preamble) + strlen(shaderString) + 1);
    output = (char*) malloc(sizeof(char) * outputSize);
    if (output) {
        strcpy(output, preamble);
        strcat(output, shaderString);
    }
    return output;
}

SHADER_API GLuint shader_create(const char *shaderString, GLenum shaderType)
{
    GLuint shader;
    shader = glCreateShader(shaderType);
    // OKGL_CHECK_ERROR();
    if (!shader) {
        return 0;
    }

    glShaderSource(shader, 1, &shaderString, NULL);
    glCompileShader(shader);
    if (!shader_checkStatus(shader, GL_COMPILE_STATUS)) {

#ifdef DEBUG
        // Print the whole problem shader with line numbering.
        okimplgl__logInfo(OKIMPLGL_LOG_TAG, "SHADER:");
        char* shaderStringTmp = malloc(sizeof(char) * (strlen(shaderString) + 1));
        strcpy(shaderStringTmp, shaderString);

        char* line = strtok(shaderStringTmp, "\n");
        int lineNumber = 1;
        while (line != NULL) {
            okimplgl__logInfo(OKIMPLGL_LOG_TAG, "%4d| %s", lineNumber, line);
            line = strtok(NULL, "\n");
            lineNumber++;
        }
        free(shaderStringTmp);
#endif

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}


SHADER_API const char* shader_getVersion(GLint* glMajorVersion, GLint* glMinorVersion)
{

    const char* version = (const char*) glGetString(GL_VERSION);

    //#if OKGL_OPENGL_ES //&& (!defined(GL_MAJOR_VERSION) || !defined(GL_MINOR_VERSION))
    // NOTE: this is for old opengl ES versions that dont have GL_MAJOR_VERSION and GL_MINOR_VERSION
    // Assuming it's OpenGL ES2 then (not supporting ES1).
    *glMajorVersion = 2;
    *glMinorVersion = 0;
    //#else
    //
    //#endif


    return version;
}


SHADER_API GLuint shader_linkProgram1(const char *vertexShaderString, const char *fragmentShaderString,Window *win)//todo
{
    GLuint program;

    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    SDL_Log("glCreateProgram();");
    program = glCreateProgram();
    if (!program) {
        return 0;
    }
    SDL_Log("glCreateProgram(); out");

    
    
    SDL_Log("gl version: %d . %d",win->glMajorVersion, win->glMinorVersion);
    
    char* vertexShaderStringFinal = shader_convert_vs_es2to3(vertexShaderString,
                                                             win);
    if (vertexShaderStringFinal) {
        vertexShader = shader_create(vertexShaderStringFinal, GL_VERTEX_SHADER);
        free(vertexShaderStringFinal);
    }

    char* fragmentShaderStringFinal = shader_convert_fs_es2to3(fragmentShaderString,
                                                               win);
    if (fragmentShaderStringFinal) {
        fragmentShader = shader_create(fragmentShaderStringFinal, GL_FRAGMENT_SHADER);
        free(fragmentShaderStringFinal);
    }


    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    // OKGL_CHECK_ERROR_ALWAYS();

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!shader_checkStatus(program, GL_LINK_STATUS)) {
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

SHADER_API GLboolean shader_checkStatus(GLuint object, GLenum operation)
{

    GLint operationStatus;

    if (operation == GL_COMPILE_STATUS) {
        glGetShaderiv(object, operation, &operationStatus);
    } else {
        glGetProgramiv(object, operation, &operationStatus);
    }

    if (!operationStatus) {
        SDL_Log("+++++++++++++++++++++++++++++++++++++++++++++++++Error compiling shader");

        GLint infoLogLength = 0;
        if (operation == GL_COMPILE_STATUS) {
            glGetShaderiv(object, GL_INFO_LOG_LENGTH, &infoLogLength);
        } else {
            glGetProgramiv(object, GL_INFO_LOG_LENGTH, &infoLogLength);
        }

        if (infoLogLength > 1) {
            char* infoLog = malloc(sizeof(char) * infoLogLength);
            if (operation == GL_COMPILE_STATUS) {
                glGetShaderInfoLog(object, infoLogLength, NULL, infoLog);
            } else {
                glGetProgramInfoLog(object, infoLogLength, NULL, infoLog);
            }
            SDL_Log("Error compiling shader : %s",infoLog);

            free(infoLog);
        }
        return GL_FALSE;
    }

    return GL_TRUE;


}


SHADER_API Shader_sdf* shader_create_sdf(Window * win)
{
    // Make sure to release old shaders first (might be reloading).


    //releaseShaders();

    Shader_sdf *out =(Shader_sdf *)malloc(sizeof (Shader_sdf ));

    //
    // A basic shader from a string.
    //
    const char* vShaderColored = ""
                                 "uniform mat4 modelView;\n"
                                 "uniform mat4 projection;\n"
                                 "attribute vec4 vertexPosition;\n"
                                 "attribute vec2 vertexTexCoord;\n"
                                 "attribute vec4 vertexColor;\n"
                                 "varying vec2 interpolatedTexCoord;\n"
                                 "varying vec4 interpolatedColor;\n"
                                 "void main() {\n"
                                 "  interpolatedColor = vertexColor;\n"
                                 "  interpolatedTexCoord = vertexTexCoord;\n"
                                 "  gl_Position = projection * modelView * vertexPosition;\n"
                                 "}\n";

    const char* fShaderColored = ""
                                 "uniform sampler2D diffuse; "
                                 "uniform int isImage; "
                                 "varying vec4 interpolatedColor; "
                                 "varying vec2 interpolatedTexCoord; "
                                 "void main() { "
                                 "vec4 t;  "
                                 "if (isImage==0) "
                                 "{ "
                                 "vec2 TexCoord=vec2(1.0-interpolatedTexCoord.x,1.0-interpolatedTexCoord.y);"
                                 "t = texture2D(diffuse,TexCoord);   "
                                 "gl_FragColor = vec4( t.rgba);    "
                                 "}"
                                 "if(isImage==1) "
                                 "{"
                                 "gl_FragColor  =  vec4(interpolatedColor.rgb,interpolatedColor.a)*interpolatedColor.a ;\n"
                                 "}"
                                 "}";

    
    //ogl es error
    out->shaderColored = shader_linkProgram1(vShaderColored, fShaderColored,win);


    char* vShaderText =""
                       "uniform mat4 modelView;\n"
                       "uniform mat4 projection;\n"
                       "attribute vec4 vertexPosition;\n"
                       "attribute vec2 vertexTexCoord;\n"
                       "attribute vec4 vertexColor;\n"
                       "varying vec2 interpolatedTexCoord;\n"
                       "varying vec4 interpolatedColor;\n"

                       "void main() {\n"
                       "interpolatedColor = vertexColor;\n"
                       "interpolatedTexCoord = vertexTexCoord;\n"
                       "gl_Position = projection * modelView * vertexPosition;\n"
                       "}\n";



    char* fShaderTextSdf = ""
                           "// fwidth() is not supported by default on OpenGL ES. Enable it.\n"
                           "uniform sampler2D sdf;\n"
                           "varying vec2 interpolatedTexCoord;\n"
                           "varying vec4 interpolatedColor;\n"
                           "const float shadowBlur =0.8;\n"
                           "const vec4 shadowColor = vec4(0.0, 0.0, 0.0, 0.9);\n"
                           "const float outlineEdgeWidth = 0.14;\n"
                           "const vec4 outlineColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
                           "const float glyphEdge = 0.5;\n"
                           "float contour(float dist, float edge, float width) {\n"
                           "  return clamp(smoothstep(edge - width, edge + width, dist), 0.0, 1.0);\n"
                           "}\n"
                           "float getSample(vec2 texCoords, float edge, float width) {\n"
                           "  return contour(texture2D(sdf, texCoords).a, edge, width);\n"
                           "}\n"
                           "void main() {\n"
                           "  vec4 tex = texture2D(sdf, interpolatedTexCoord);\n"
                           "  float dist  = tex.a;\n"
                           "  float width = fwidth(dist);\n"
                           "  vec4 textColor = clamp(interpolatedColor, 0.0, 1.0);\n"
                           "  float outerEdge = glyphEdge;\n"
                           "  #if defined(SUPERSAMPLE)\n"
                           "    float alpha = contour(dist, outerEdge, width);\n"
                           "    float dscale = 0.354; // half of 1/sqrt2; you can play with this\n"
                           "    vec2 uv = interpolatedTexCoord.xy;\n"
                           "    vec2 duv = dscale * (dFdx(uv) + dFdy(uv));\n"
                           "    vec4 box = vec4(uv - duv, uv + duv);\n"
                           "    float asum = getSample(box.xy, outerEdge, width)\n"
                           "               + getSample(box.zw, outerEdge, width)\n"
                           "               + getSample(box.xw, outerEdge, width)\n"
                           "               + getSample(box.zy, outerEdge, width);\n"
                           "    // weighted average, with 4 extra points having 0.5 weight each,\n"
                           "    // so 1 + 0.5*4 = 3 is the divisor\n"
                           "    alpha = (alpha + 0.5 * asum) / 3.0;\n"
                           "  #else\n"
                           "    // No supersampling.\n"
                           "outerEdge = outerEdge + outlineEdgeWidth;\n"
                           "    float alpha = contour(dist, outerEdge, width);\n"
                           "  #endif\n"
                           "  gl_FragColor = vec4(textColor.rgb, textColor.a * alpha);\n"
                           "float shadowAlpha = clamp(smoothstep(max(outerEdge - shadowBlur, 0.00000000001), outerEdge + shadowBlur, dist), 0.0, 1.0);\n"
                           "vec4 shadow = shadowColor * shadowAlpha;\n"
                           "gl_FragColor = shadow * (1.0 - gl_FragColor.a) + gl_FragColor * gl_FragColor.a;\n"


                           "}\n";




    //    char* fShaderTextSdf=
    //            "uniform sampler2D sdf;\n"
    //            "uniform float time;\n"
    //            "varying vec2 interpolatedTexCoord;\n"
    //            "varying vec4 interpolatedColor;\n"
    //            "const float glyphEdge = 0.5;\n"
    //            "#define SUPERSAMPLE\n"
    //            "#define OUTLINE"
    //            "const float outlineEdgeWidth = 0.04;\n"
    //            "const vec4 outlineColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
    //            "#define SHADOW"
    //            "const float shadowBlur = 0.3;\n"
    //            "const vec4 shadowColor = vec4(0.0, 0.0, 0.0, 0.5);\n"
    //            "#define GROW_ANIMATION\n"
    //            "#if defined(GL_OES_standard_derivatives)\n"
    //            "  #extension GL_OES_standard_derivatives : enable\n"
    //            "#endif\n"
    //            "float contour(float dist, float edge, float width) {\n"
    //            "  return clamp(smoothstep(edge - width, edge + width, dist), 0.0, 1.0);\n"
    //            "}\n"
    //            "float getSample(vec2 texCoords, float edge, float width) {\n"
    //            "  return contour(texture2D(sdf, texCoords).a, edge, width);\n"
    //            "}"
    //            "  void main() {\n"
    //            "  vec4 tex = texture2D(sdf, interpolatedTexCoord);\n"
    //            "  float dist  = tex.a;\n"
    //            "  float width = fwidth(dist);\n"
    //            "  vec4 textColor = clamp(interpolatedColor, 0.0, 1.0);\n"
    //            "  float outerEdge = glyphEdge;\n"

    //            "  #if defined(GROW_ANIMATION)\n"
    //            "    outerEdge -= (sin(time * 3.3) + 0.8) * 0.1;\n"
    //            "  #endif\n"
    //            "  #if defined(SUPERSAMPLE)\n"
    //            "    vec2 uv = interpolatedTexCoord.xy;\n"
    //            "    float alpha = contour(dist, outerEdge, width);\n"
    //            "    float dscale = 0.354; // half of 1/sqrt2; you can play with this\n"
    //            "    vec2 duv = dscale * (dFdx(uv) + dFdy(uv));\n"
    //            "    vec4 box = vec4(uv-duv, uv+duv);\n"
    //            "    float asum = getSample(box.xy, outerEdge, width)\n"
    //            "               + getSample(box.zw, outerEdge, width)\n"
    //            "               + getSample(box.xw, outerEdge, width)\n"
    //            "               + getSample(box.zy, outerEdge, width);\n"

    //            "    // weighted average, with 4 extra points having 0.5 weight each,\n"
    //            "    // so 1 + 0.5*4 = 3 is the divisor\n"
    //            "    alpha = (alpha + 0.5 * asum) / 3.0;\n"
    //            "  #else\n"
    //            "    //float alpha = clamp(smoothstep(outerEdge - width, outerEdge + width, dist), 0.0, 1.0);\n"
    //            "    float alpha = contour(dist, outerEdge, width);"
    //            "  #endif\n"
    //            "  // Basic simple SDF text without effects. Normal blending.\n"
    //            "  gl_FragColor = vec4(textColor.rgb, textColor.a * alpha);\n"
    //            "  #if defined(OUTLINE)\n"
    //            "    outerEdge = outerEdge - outlineEdgeWidth;\n"
    //            "    #if defined(GROW_ANIMATION)\n"
    //            "      outerEdge -= (sin(time * 10.3) + 0.8) * 0.05;\n"
    //            "    #endif\n"
    //            "    float outlineOuterAlpha = clamp(smoothstep(outerEdge - width, outerEdge + width, dist), 0.0, 1.0);\n"
    //            "    float outlineAlpha = outlineOuterAlpha - alpha;\n"
    //            "    gl_FragColor.rgb = mix(outlineColor.rgb, gl_FragColor.rgb, alpha);\n"
    //            "    gl_FragColor.a = max(gl_FragColor.a, outlineColor.a * outlineOuterAlpha);\n"
    //            "  #endif\n"
    //            "  #if defined(SHADOW)\n"
    //            "    float shadowAlpha = clamp(smoothstep(max(outerEdge - shadowBlur, 0.05), outerEdge + shadowBlur, dist), 0.0, 1.0);\n"
    //            "    vec4 shadow = shadowColor * shadowAlpha;\n"
    //            "    gl_FragColor = shadow * (1.0 - gl_FragColor.a) + gl_FragColor * gl_FragColor.a;\n"

    //            "  #else\n"
    //            "    // Premultiplied alpha output.\n"
    //            "    gl_FragColor.rgb *= gl_FragColor.a;\n"
    //            "  #endif\n"
    //            "  #if defined(GLOW_ANIMATION)\n"
    //            "    float glowIntensityAnim = (sin(time * 4.3) + 2.0) * 0.25;\n"
    //            "    float glowArea = clamp(smoothstep(glyphEdge - 0.25, glyphEdge + 0.0, dist), 0.0, 1.0);\n"
    //            "    vec4 glow = vec4(vec3(glowIntensityAnim), 0.0) * glowArea;\n"
    //            "    gl_FragColor = glow * (1.0 - gl_FragColor.a) + gl_FragColor * gl_FragColor.a;\n"
    //            "  #endif\n"
    //            "}\n";





    ////    "  // Premultiplied alpha output.\n"
    // //   "  gl_FragColor.rgb *= gl_FragColor.a;\n"

    out->shaderTextSdf = shader_linkProgram1(vShaderText, fShaderTextSdf,win);



    return out;
}



void shader_free_sdf(Shader_sdf *a)
{
    glDeleteProgram(a->shaderColored);
    a->shaderColored = 0;

    //glDeleteProgram(a->shaderText);
    //a->shaderText = 0;

    glDeleteProgram(a->shaderTextSdf);
    a->shaderTextSdf = 0;

    free(a);
}

SHADER_API void shader_release (GLuint shader)
{


    glDeleteProgram(shader );
    shader  = 0;


}









