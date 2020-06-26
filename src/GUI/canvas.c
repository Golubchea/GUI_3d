#include "canvas.h"


#if OKPLATFORM_EMSCRIPTEN
#define GL_GLEXT_PROTOTYPES 1
//#include <SDL_opengles2.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "canvas/glad/glad.h"
#include "canvas/glad/khrplatform.h"
#endif

#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "button/stb_image.h"
#include "button/stb_image_write.h"

#define GLFONTSTASH_IMPLEMENTATION_ES2


void __main_renderDelete(void *p);
int __main_renderCreate(void* p, int width, int height);
void __renderDraw(void* p  , const float* verts, const float* tcoords, const unsigned int* colors, int nverts);
void __renderUpdate(void*p, int* rect, const unsigned char* data);


extern size_t strLen2(const char *str)
{
    const char *s;

    for (s = str; *s; ++s)
        ;
    return (s - str);
}

// Function to implement strcat() function in C
extern char* strCat(char* destination, const char* source)
{
    // make ptr point to the end of destination string
    char* ptr = destination + strLen2(destination);

    // Appends characters of source to the destination string
    while (*source != '\0')
        *ptr++ = *source++;

    // null terminate destination string
    *ptr = '\0';

    // destination is returned by standard strcat()
    return destination;
}



int strCat2(char *destination, const char *source)
{
    // make ptr point to the end of destination string
    int i=strLen2(destination);
    char* ptr = destination + i;

    // Appends characters of source to the destination string
    while (*source != '\0')
        *ptr++ = *source++;

    // null terminate destination string
    *ptr = '\0';

    // destination is returned by standard strcat()
    return i;
}



extern void RemoveChars(char *s, char c)
{
    int writer = 0, reader = 0;

    while (s[reader])
    {
        if (s[reader]!=c)
        {
            s[writer++] = s[reader];
        }

        reader++;
    }

    s[writer]=0;
}


CANVAS_API Canvas *Canvas_create(const char * background)
{
    Canvas *gl=(Canvas *)malloc(sizeof (Canvas))  ;




    GLenum format=GL_RGBA;
    int w1;
    int h1;
    int n;
    unsigned char* data= stbi_load(background, &w1, &h1, &n,4);//, STBI_rgb_alpha);

    if(data==NULL)
    {
        SDL_Log("----------------------------------------------------------------------------------can't load %s",background);
    }
    else
    {
        SDL_Log("tex loaded: %d number of channels",n);
    }



    gl->needToRedraw=true;

    gl->width=w1;
    gl->height=h1;

    gl->width_image=w1;
    gl->height_image=h1;


    gl->window=window_create("name", gl->width, gl->height);
    gl->shaders= shader_create_sdf(gl->window);


    gl->font =Font_create("DroidSans.ttf","droid.ttf",__main_renderCreate,__renderUpdate,__renderDraw,__main_renderDelete);

    gl->data.texW=512;
    gl->data.texH=512;



    // Create may be called multiple times, delete existing texture.
    if (gl->tex != 0) {
        glDeleteTextures(1, &gl->tex);
        gl->tex = 0;
    }

    glGenTextures(1, &gl->tex);
    if (!gl->tex) return 0;

    // Only use VAO if they are supported. This way the same implementation works on OpenGL ES2 too.
#ifndef GLFONTSTASH_IMPLEMENTATION_ES2
    if (!gl->vertexArray) glGenVertexArrays(1, &gl->vertexArray);
    if (!gl->vertexArray) return 0;

    glBindVertexArray(gl->vertexArray);
#endif

    if (!gl->vertexBuffer) glGenBuffers(1, &gl->vertexBuffer);
    if (!gl->vertexBuffer) return 0;

    if (!gl->tcoordBuffer) glGenBuffers(1, &gl->tcoordBuffer);
    if (!gl->tcoordBuffer) return 0;

    if (!gl->colorBuffer) glGenBuffers(1, &gl->colorBuffer);
    if (!gl->colorBuffer) return 0;


    glBindTexture(GL_TEXTURE_2D, gl->tex);




#ifdef GLFONTSTASH_IMPLEMENTATION_ES2


    //glGenerateMipmap(GL_TEXTURE_2D);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, gl->data.texW, gl->data.texH, 0, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0,format, (GLsizei)w1, (GLsizei)h1, 0,format, GL_UNSIGNED_BYTE, data);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, gl->width, gl->height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    static GLint swizzleRgbaParams[4] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRgbaParams);
#endif

    free(data);


    //set matrices

    matrix_projection2d(gl->projection, 0.0f, (GLfloat)gl->width, (GLfloat)gl->height, 0.0f);
    //matrix_identityE(gl->modelView);
    matrix_identityE(gl->modelViewGUI);



    gl->shaderInSwitcherLoc= glGetUniformLocation(gl->shaders->shaderColored, "isImage");


    //    glEnable( GL_ALPHA_TEST );
    //    glAlphaFunc( GL_NOTEQUAL, 0.0 );

    gl->projectionMatrixLoc = glGetUniformLocation(gl->shaders->shaderColored, "projection");
    gl->modelViewMatrixLoc = glGetUniformLocation(gl->shaders->shaderColored, "modelView");
    gl->projectionMatrixSDFLoc = glGetUniformLocation(gl->shaders->shaderTextSdf, "projection");
    gl->modelViewMatrixSDFLoc = glGetUniformLocation(gl->shaders->shaderTextSdf, "modelView");

    glViewport(0, 0, gl->width, gl->height);




    return gl;
}

CANVAS_API void Canvas_free(Canvas *gl )
{

    if (gl->tex != 0)
    {
        glDeleteTextures(1, &gl->tex);
        gl->tex = 0;
    }

#ifndef GLFONTSTASH_IMPLEMENTATION_ES2
    glBindVertexArray(0);
#endif

    if (gl->vertexBuffer != 0) {
        glDeleteBuffers(1, &gl->vertexBuffer);
        gl->vertexBuffer = 0;
    }

    if (gl->tcoordBuffer != 0) {
        glDeleteBuffers(1, &gl->tcoordBuffer);
        gl->tcoordBuffer = 0;
    }

    if (gl->colorBuffer != 0) {
        glDeleteBuffers(1, &gl->colorBuffer);
        gl->colorBuffer = 0;
    }

#ifndef GLFONTSTASH_IMPLEMENTATION_ES2
    if (gl->vertexArray != 0) {
        glDeleteVertexArrays(1, &gl->vertexArray);
        gl->vertexArray = 0;
    }
#endif
    if (gl->font!=NULL)
    {
        Font_free(gl->font);
    }

    shader_free_sdf(gl->shaders);
    window_free(gl->window);

    SDL_Log("cleared");


    free(gl);

}



CANVAS_API __inline void canvas_add_vertex(Canvas *canvas, float x, float y, float s, float t, unsigned int c)
{
    canvas->data.verts[canvas->data.nverts*2+0] = x;
    canvas->data.verts[canvas->data.nverts*2+1] = y;
    canvas->data.tcoords[canvas->data.nverts*2+0] = s;
    canvas->data.tcoords[canvas->data.nverts*2+1] = t;
    canvas->data.colors[canvas->data.nverts] = c;
    canvas->data.nverts++;
}



CANVAS_API void canvas_flushLine(void *userPtr)
{

    Canvas* gl = (Canvas*)userPtr;

    if (gl->data.nverts > 0)
    {
#ifdef GLFONTSTASH_IMPLEMENTATION_ES2
        if (gl->tex == 0) return;
#else
        if (gl->tex == 0 || gl->vertexArray == 0) return;

        glBindVertexArray(gl->vertexArray);
#endif
        //gl->tex = 0 ;
        //glActiveTexture(GL_TEXTURE0);
        //    glBindTexture(GL_TEXTURE_2D, gl->tex);

        glEnableVertexAttribArray(GLFONS_VERTEX_ATTRIB);
        glBindBuffer(GL_ARRAY_BUFFER, gl->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, gl->data.nverts * 2 * sizeof(float), gl->data.verts, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(GLFONS_VERTEX_ATTRIB, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        //printf("%d",sizeof(float) );

        glEnableVertexAttribArray(GLFONS_COLOR_ATTRIB);
        glBindBuffer(GL_ARRAY_BUFFER, gl->colorBuffer);
        // glBindBuffer(GL_ARRAY_BUFFER, gl->tcoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, gl->data.nverts * sizeof(unsigned int), gl->data.colors, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(GLFONS_COLOR_ATTRIB, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, NULL);


        glEnableVertexAttribArray(GLFONS_TCOORD_ATTRIB);
        glBindBuffer(GL_ARRAY_BUFFER, gl->tcoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, gl->data.nverts * 2 * sizeof(float), gl->data.tcoords, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(GLFONS_TCOORD_ATTRIB, 2, GL_FLOAT, GL_FALSE, 0, NULL);



        glDrawArrays(GL_LINES, 0, gl->data.nverts);

        glDisableVertexAttribArray(GLFONS_VERTEX_ATTRIB);
        glDisableVertexAttribArray(GLFONS_TCOORD_ATTRIB);
        glDisableVertexAttribArray(GLFONS_COLOR_ATTRIB);

#ifndef GLFONTSTASH_IMPLEMENTATION_ES2
        glBindVertexArray(0);
#endif
        gl->data.nverts = 0;
    }

}





CANVAS_API void canvas_flush(void *userPtr)
{

    Canvas* gl = (Canvas*)userPtr;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gl->tex);

    glEnableVertexAttribArray(GLFONS_VERTEX_ATTRIB);
    glBindBuffer(GL_ARRAY_BUFFER, gl->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, gl->data.nverts * 2 * sizeof(float), gl->data.verts, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GLFONS_VERTEX_ATTRIB, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(GLFONS_TCOORD_ATTRIB);
    glBindBuffer(GL_ARRAY_BUFFER, gl->tcoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, gl->data.nverts * 2 * sizeof(float), gl->data.tcoords, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GLFONS_TCOORD_ATTRIB, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(GLFONS_COLOR_ATTRIB);
    glBindBuffer(GL_ARRAY_BUFFER, gl->colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, gl->data.nverts * sizeof(unsigned int), gl->data.colors, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GLFONS_COLOR_ATTRIB, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, NULL);

    glDrawArrays(GL_TRIANGLES, 0, gl->data.nverts);

    glDisableVertexAttribArray(GLFONS_VERTEX_ATTRIB);
    glDisableVertexAttribArray(GLFONS_TCOORD_ATTRIB);
    glDisableVertexAttribArray(GLFONS_COLOR_ATTRIB);
    // glBindTexture(GL_TEXTURE_2D, 0);
#ifndef GLFONTSTASH_IMPLEMENTATION_ES2
    glBindVertexArray(0);
#endif
    gl->data.nverts = 0;
}




CANVAS_API void canvas_DrawRectangle(Canvas* stash,
                                     float x, float y,
                                     float w,float h, unsigned int color)
{
    Quad q;
    q.x0=x;
    q.x1=x+w;

    q.y0=y;
    q.y1=y+h;




    q.s0=1.0; //red
    q.t0=1.0; //green
    q.s1=0.0; //red & alpha
    q.t1=0.0;

    canvas_add_vertex(stash, q.x0, q.y0, q.s0, q.t0,color);
    canvas_add_vertex(stash, q.x1, q.y1, q.s1, q.t1, color);
    canvas_add_vertex(stash, q.x1, q.y0, q.s1, q.t0, color);

    canvas_add_vertex(stash, q.x0, q.y0, q.s0, q.t0, color);
    canvas_add_vertex(stash, q.x0, q.y1, q.s0, q.t1, color);
    canvas_add_vertex(stash, q.x1, q.y1, q.s1, q.t1, color);


    canvas_flush(stash);

}



CANVAS_API void canvas_DrawLine(Canvas* stash,
                                float x0, float y0,
                                float x1,float y1, unsigned int color)
{
    Quad q;



    q.s0=1.0; //red
    q.t0=1.0; //green
    q.s1=0.0; //red & alpha
    q.t1=0.0;

    canvas_add_vertex(stash, x0, y0, q.s0, q.t0, color);
    canvas_add_vertex(stash, x1, y1, q.s1, q.t1, color);

    canvas_add_vertex(stash, x0, y0, q.s0, q.t0, color);
    canvas_add_vertex(stash, x1, y1, q.s1, q.t1, color);

    canvas_add_vertex(stash, x0, y0, q.s0, q.t0, color);
    canvas_add_vertex(stash, x1, y1, q.s1, q.t1, color);

    canvas_add_vertex(stash, x0, y0, q.s0, q.t0, color);
    canvas_add_vertex(stash, x1, y1, q.s1, q.t1, color);
    //    canvas_add_vertex(stash, x0, y0, q.s1, q.t1, color);

    //    canvas_add_vertex(stash, x0, y0, q.s1, q.t1, color);
    //    canvas_add_vertex(stash, x0, y0, q.s1, q.t1, color);


    //      canvas_add_vertex(stash, x1, y1, q.s1, q.t1, color);

    //    canvas_add_vertex(stash, x1, y1, q.s1, q.t1, color);
    //    canvas_add_vertex(stash, x1, y1, q.s0, q.t0, color);

    canvas_flush(stash);

}


CANVAS_API void canvas_DrawRectangleLine2(Canvas* stash,
                                          float x0, float y0,
                                          float x1, float y1,
                                          float x2, float y2,
                                          float x3, float y3,
                                          unsigned int color)
{
    Quad q;



    q.s0=1.0; //red
    q.t0=1.0; //green
    q.s1=0.5; //red & alpha
    q.t1=0.5;



    canvas_add_vertex(stash, x0, y0, q.s0, q.t0, color);
    canvas_add_vertex(stash, x1, y1, q.s1, q.t1, color);

    canvas_add_vertex(stash, x1, y1, q.s1, q.t1, color);
    canvas_add_vertex(stash, x2, y2, q.s1, q.t1, color);

    canvas_add_vertex(stash, x2, y2, q.s1, q.t1, color);
    canvas_add_vertex(stash, x3, y3, q.s1, q.t1, color);

    canvas_add_vertex(stash, x3, y3, q.s1, q.t1, color);
    canvas_add_vertex(stash, x0, y0, q.s0, q.t0, color);


    canvas_flushLine(stash);

}


CANVAS_API void canvas_DrawRectangleLine(Canvas* stash,
                                         float x, float y,
                                         float w,float h, unsigned int color)
{

    Quad q;
    q.x0=x;
    q.x1=x+w;

    q.y0=y;
    q.y1=y+h;




    q.s0=1.0; //red
    q.t0=1.0; //green
    q.s1=0.5; //red & alpha
    q.t1=0.5;



    canvas_add_vertex(stash, q.x0, q.y0, q.s0, q.t0, color);
    canvas_add_vertex(stash, q.x0, q.y1, q.s1, q.t1, color);

    canvas_add_vertex(stash, q.x0, q.y1, q.s1, q.t1, color);
    canvas_add_vertex(stash, q.x1, q.y1, q.s1, q.t1, color);

    canvas_add_vertex(stash, q.x1, q.y1, q.s1, q.t1, color);
    canvas_add_vertex(stash, q.x1, q.y0, q.s1, q.t1, color);

    canvas_add_vertex(stash, q.x1, q.y0, q.s1, q.t1, color);
    canvas_add_vertex(stash, q.x0, q.y0, q.s0, q.t0, color);


    canvas_flushLine(stash);



}
CANVAS_API void canvas_DrawRectangleRoundedUp(Canvas* stash,
                                              float x, float y,
                                              float w,float h,float r,int n, unsigned int color)
{

    Quad q;
    q.x0=x;
    q.x1=x+w;

    q.y0=y;
    q.y1=y+h;




    q.s0=1.0; //red
    q.t0=1.0; //green
    q.s1=0.0; //red & alpha
    q.t1=0.0;


    float cx;
    float cy;
    float stp=(M_PI/2.0) /(float)n;

    cx= x+r;
    cy= y+r;

    for(unsigned int i=0;i<n;++i)
    {
        canvas_add_vertex(stash, cx+r*cos(i*stp+(M_PI)),cy+r*sin(i*stp+(M_PI)) , q.s0, q.t0,color);
        canvas_add_vertex(stash, cx+r*cos((i+1)*stp+(M_PI)),cy+r*sin((i+1)*stp+(M_PI)) ,q.s1, q.t1, color);
        canvas_add_vertex(stash, cx, cy, q.s1, q.t0, color);
    }
    //bottom []
    canvas_add_vertex(stash, x+w,y+h , q.s0, q.t0,color);
    canvas_add_vertex(stash, x  ,y+r ,q.s1, q.t1, color);
    canvas_add_vertex(stash, x  ,y+h, q.s1, q.t0, color);

    canvas_add_vertex(stash, x+w ,y+h ,q.s1, q.t0, color);
    canvas_add_vertex(stash, x+w ,y+r ,q.s0, q.t0,color);
    canvas_add_vertex(stash, x   ,y+r ,q.s1, q.t1, color);
    //
    canvas_add_vertex(stash, x+w,y  , q.s0, q.t0,color);
    canvas_add_vertex(stash, x+r  ,y+r ,q.s1, q.t1, color);
    canvas_add_vertex(stash, x+r  ,y , q.s1, q.t0, color);

    canvas_add_vertex(stash, x+w ,y  ,q.s1, q.t0, color);
    canvas_add_vertex(stash, x+w ,y+r ,q.s0, q.t0,color);
    canvas_add_vertex(stash, x+r   ,y+r ,q.s1, q.t1, color);


    canvas_flush(stash);

}

CANVAS_API void canvas_DrawRectangleRoundedDown(Canvas* stash,
                                                float x, float y,
                                                float w,float h,float r,int n, unsigned int color)
{
    Quad q;
    q.x0=x;
    q.x1=x+w;

    q.y0=y;
    q.y1=y+h;




    q.s0=1.0; //red
    q.t0=1.0; //green
    q.s1=0.0; //red & alpha
    q.t1=0.0;


    float cx= x+w-r;
    float cy= y+h-r;
    float stp=(M_PI/2.0) /(float)n;
    for(unsigned int i=0;i<n;++i)
    {
        canvas_add_vertex(stash, cx+r*cos(i*stp),cy+r*sin(i*stp) , q.s0, q.t0,color);
        canvas_add_vertex(stash, cx+r*cos((i+1)*stp),cy+r*sin((i+1)*stp) ,q.s1, q.t1, color);
        canvas_add_vertex(stash, cx, cy, q.s1, q.t0, color);
    }

    cx= x+r;
    cy= y+h-r;

    for(unsigned int i=0;i<n;++i)
    {
        canvas_add_vertex(stash, cx+r*cos(i*stp+(M_PI/2.0)),cy+r*sin(i*stp+(M_PI/2.0)) , q.s0, q.t0,color);
        canvas_add_vertex(stash, cx+r*cos((i+1)*stp+(M_PI/2.0)),cy+r*sin((i+1)*stp+(M_PI/2.0)) ,q.s1, q.t1, color);
        canvas_add_vertex(stash, cx, cy, q.s1, q.t0, color);
    }



    //middle
    canvas_add_vertex(stash,x+r,y , q.s0, q.t0,color);
    canvas_add_vertex(stash, x+w-r,y ,q.s1, q.t1, color);
    canvas_add_vertex(stash,x+r,y+h, q.s1, q.t0, color);

    canvas_add_vertex(stash,x+r,y+h, q.s1, q.t0, color);
    canvas_add_vertex(stash, x+w-r,y ,q.s1, q.t1, color);
    canvas_add_vertex(stash,x+w-r,y+h , q.s0, q.t0,color);
    //right
    canvas_add_vertex(stash,x+w ,y  , q.s0, q.t0,color);
    canvas_add_vertex(stash,x+w ,y+h-r ,q.s1, q.t1, color);
    canvas_add_vertex(stash,x+w-r,y+h-r, q.s1, q.t0, color);

    canvas_add_vertex(stash,x+w ,y  , q.s0, q.t0,color);
    canvas_add_vertex(stash,x+w-r,y+h-r, q.s1, q.t0, color);
    canvas_add_vertex(stash,x+w-r ,y ,q.s1, q.t1, color);


    //    //left
    canvas_add_vertex(stash,x  ,y  , q.s0, q.t0,color);
    canvas_add_vertex(stash,x  ,y+h-r ,q.s1, q.t1, color);
    canvas_add_vertex(stash,x +r,y+h-r, q.s1, q.t0, color);

    canvas_add_vertex(stash,x  ,y  , q.s0, q.t0,color);
    canvas_add_vertex(stash,x +r,y+h-r, q.s1, q.t0, color);
    canvas_add_vertex(stash,x +r ,y ,q.s1, q.t1, color);


    canvas_flush(stash);

}

CANVAS_API void canvas_DrawRectangleRounded(Canvas* stash,
                                            float x, float y,
                                            float w,float h,float r,int n, unsigned int color)
{
    Quad q;
    q.x0=x;
    q.x1=x+w;

    q.y0=y;
    q.y1=y+h;




    q.s0=1.0; //red
    q.t0=1.0; //green
    q.s1=0.0; //red & alpha
    q.t1=0.0;


    float cx= x+w-r;
    float cy= y+h-r;
    float stp=(M_PI/2.0) /(float)n;
    for(unsigned int i=0;i<n;++i)
    {
        canvas_add_vertex(stash, cx+r*cos(i*stp),cy+r*sin(i*stp) , q.s0, q.t0,color);
        canvas_add_vertex(stash, cx+r*cos((i+1)*stp),cy+r*sin((i+1)*stp) ,q.s1, q.t1, color);
        canvas_add_vertex(stash, cx, cy, q.s1, q.t0, color);
    }

    cx= x+r;
    cy= y+h-r;

    for(unsigned int i=0;i<n;++i)
    {
        canvas_add_vertex(stash, cx+r*cos(i*stp+(M_PI/2.0)),cy+r*sin(i*stp+(M_PI/2.0)) , q.s0, q.t0,color);
        canvas_add_vertex(stash, cx+r*cos((i+1)*stp+(M_PI/2.0)),cy+r*sin((i+1)*stp+(M_PI/2.0)) ,q.s1, q.t1, color);
        canvas_add_vertex(stash, cx, cy, q.s1, q.t0, color);
    }


    cx= x+r;
    cy= y+r;

    for(unsigned int i=0;i<n;++i)
    {
        canvas_add_vertex(stash, cx+r*cos(i*stp+(M_PI)),cy+r*sin(i*stp+(M_PI)) , q.s0, q.t0,color);
        canvas_add_vertex(stash, cx+r*cos((i+1)*stp+(M_PI)),cy+r*sin((i+1)*stp+(M_PI)) ,q.s1, q.t1, color);
        canvas_add_vertex(stash, cx, cy, q.s1, q.t0, color);
    }

    cx=  x+w-r;
    cy= y+r;

    for(unsigned int i=0;i<n;++i)
    {
        canvas_add_vertex(stash, cx+r*cos(i*stp+(M_PI/2.0*3.0)),cy+r*sin(i*stp+(M_PI/2.0*3.0)) , q.s0, q.t0,color);
        canvas_add_vertex(stash, cx+r*cos((i+1)*stp+(M_PI/2.0*3.0)),cy+r*sin((i+1)*stp+(M_PI/2.0*3.0)) ,q.s1, q.t1, color);
        canvas_add_vertex(stash, cx, cy, q.s1, q.t0, color);
    }
    //middle
    canvas_add_vertex(stash,x+r,y , q.s0, q.t0,color);
    canvas_add_vertex(stash, x+w-r,y ,q.s1, q.t1, color);
    canvas_add_vertex(stash,x+r,y+h, q.s1, q.t0, color);

    canvas_add_vertex(stash,x+r,y+h, q.s1, q.t0, color);
    canvas_add_vertex(stash, x+w-r,y ,q.s1, q.t1, color);
    canvas_add_vertex(stash,x+w-r,y+h , q.s0, q.t0,color);
    //right
    canvas_add_vertex(stash,x+w-r,y+r , q.s0, q.t0,color);
    canvas_add_vertex(stash,x+w-r,y+h-r ,q.s1, q.t1, color);
    canvas_add_vertex(stash,x+w,y+h-r, q.s1, q.t0, color);

    canvas_add_vertex(stash,x+w ,y+r , q.s0, q.t0,color);
    canvas_add_vertex(stash,x+w-r,y+r ,q.s1, q.t1, color);
    canvas_add_vertex(stash,x+w,y+h-r, q.s1, q.t0, color);

    //left
    canvas_add_vertex(stash,x+r,y+r , q.s0, q.t0,color);
    canvas_add_vertex(stash,x+r,y+h-r ,q.s1, q.t1, color);
    canvas_add_vertex(stash,x,y+h-r, q.s1, q.t0, color);

    canvas_add_vertex(stash,x ,y+r , q.s0, q.t0,color);
    canvas_add_vertex(stash,x+r,y+r ,q.s1, q.t1, color);
    canvas_add_vertex(stash,x,y+h-r, q.s1, q.t0, color);


    canvas_flush(stash);


}



void canvas_start_paint2dUI(Canvas *canvas)
{
    glClearColor(0.59f, 0.59f, 0.59f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    //
    // Enable blending.
    //
    glEnable(GL_BLEND);
    // Note: using premultiplied alpha blending.
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(canvas->shaders->shaderColored);



    // glUniformMatrix4fv(canvas->projectionMatrixLoc, 1, GL_FALSE, &canvas->projection[0]);

    glUniformMatrix4fv(canvas->projectionMatrixLoc, 1, GL_FALSE, &canvas->projection[0]);
    glUniformMatrix4fv(canvas->modelViewMatrixLoc, 1, GL_FALSE, &canvas->modelViewGUI[0]);
}

void canvas_start_paintText(Canvas *canvas)
{
    glUseProgram(canvas->shaders->shaderTextSdf);
    glUniformMatrix4fv(canvas->projectionMatrixSDFLoc, 1, GL_FALSE, &canvas->projection[0]);
    glUniformMatrix4fv(canvas->modelViewMatrixSDFLoc, 1, GL_FALSE, &canvas->modelViewGUI[0]);
}




void canvas_end_paint2dUI(Canvas *canvas)
{
    glDisable(GL_BLEND);
    //canvas->needToRedraw=false;
    SDL_Delay(30);
    SDL_GL_SwapWindow( canvas->window->sdlWindow);
}

void canvas_size_changed(Canvas *canvas)
{
    SDL_GL_GetDrawableSize( canvas->window->sdlWindow, &canvas->width, &canvas->height);
    SDL_Log( "(w,h ): %d,%d ",canvas->width,canvas->height);


    // This creates a coordinate system where one pixel is one unit and where (0, 0) is the top left corner.
    glViewport(0, 0,(GLfloat) canvas->width,(GLfloat)canvas->height);
    matrix_projection2d( canvas->projection, 0.0f, (GLfloat)canvas->width, (GLfloat)canvas->height, 0.0f);


    matrix_identityE(canvas->modelViewGUI);
}

//text API


void __renderUpdate(void*p, int* rect, const unsigned char* data)
{

    GLcontext *gl=(GLcontext*)p;
#ifdef GLFONTSTASH_IMPLEMENTATION_ES2
    GLint alignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    glBindTexture(GL_TEXTURE_2D, gl->tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // TODO: for now the whole texture is updated every time. Profile how bad is this.
    // as ES2 doesn't seem to support GL_UNPACK_ROW_LENGTH the only other option is to make a temp copy of the updated
    // portion with contiguous memory which is probably even worse.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, gl->width, gl->height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

#else
    // OpenGl desktop/es3 should use this version:
    int w = rect[2] - rect[0];
    int h = rect[3] - rect[1];

    if (gl->tex == 0) return;

    // Push old values
    GLint alignment, rowLength, skipPixels, skipRows;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowLength);
    glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skipPixels);
    glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skipRows);

    glBindTexture(GL_TEXTURE_2D, gl->tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, gl->width);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, rect[0]);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, rect[1]);

    glTexSubImage2D(GL_TEXTURE_2D, 0, rect[0], rect[1], w, h, GL_RED, GL_UNSIGNED_BYTE, data);

    // Pop old values
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, skipPixels);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, skipRows);
#endif

}


void __renderDraw(void* p  , const float* verts, const float* tcoords, const unsigned int* colors, int nverts)
{
    GLcontext *gl=(GLcontext*)p;
#ifdef GLFONTSTASH_IMPLEMENTATION_ES2
    if (gl->tex == 0) return;
#else
    if (gl->tex == 0 || gl->vertexArray == 0) return;

    glBindVertexArray(gl->vertexArray);
#endif

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gl->tex);

    glEnableVertexAttribArray(GLFONS_VERTEX_ATTRIB);
    glBindBuffer(GL_ARRAY_BUFFER, gl->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, nverts * 2 * sizeof(float), verts, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GLFONS_VERTEX_ATTRIB, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(GLFONS_TCOORD_ATTRIB);
    glBindBuffer(GL_ARRAY_BUFFER, gl->tcoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, nverts * 2 * sizeof(float), tcoords, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GLFONS_TCOORD_ATTRIB, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(GLFONS_COLOR_ATTRIB);
    glBindBuffer(GL_ARRAY_BUFFER, gl->colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, nverts * sizeof(unsigned int), colors, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GLFONS_COLOR_ATTRIB, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, NULL);

    glDrawArrays(GL_TRIANGLES, 0, nverts);

    glDisableVertexAttribArray(GLFONS_VERTEX_ATTRIB);
    glDisableVertexAttribArray(GLFONS_TCOORD_ATTRIB);
    glDisableVertexAttribArray(GLFONS_COLOR_ATTRIB);

    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //   glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glUseProgram(0);
#ifndef GLFONTSTASH_IMPLEMENTATION_ES2
    glBindVertexArray(0);
#endif
}


int __main_renderCreate(void* p, int width, int height)
{
    GLcontext *gl=(GLcontext*)p;

    SDL_Log("rc1 gl->tex =%d",gl->tex);
    GLint alignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);




    SDL_Log("align =%d",alignment);


    // Create may be called multiple times, delete existing texture.
    if (gl->tex != 0) {
        glDeleteTextures(1, &gl->tex);
        gl->tex = 0;
    }
    SDL_Log("rc2");
    glGenTextures(1, &gl->tex);
    if (!gl->tex) return 0;
    SDL_Log("rc3");
    // Only use VAO if they are supported. This way the same implementation works on OpenGL ES2 too.
#ifndef GLFONTSTASH_IMPLEMENTATION_ES2
    if (!gl->vertexArray) glGenVertexArrays(1, &gl->vertexArray);
    if (!gl->vertexArray) return 0;

    glBindVertexArray(gl->vertexArray);
#endif

    if (!gl->vertexBuffer) glGenBuffers(1, &gl->vertexBuffer);
    if (!gl->vertexBuffer) return 0;

    if (!gl->tcoordBuffer) glGenBuffers(1, &gl->tcoordBuffer);
    if (!gl->tcoordBuffer) return 0;

    if (!gl->colorBuffer) glGenBuffers(1, &gl->colorBuffer);
    if (!gl->colorBuffer) return 0;
    SDL_Log("rc4");
    gl->width = width;
    gl->height = height;
    glBindTexture(GL_TEXTURE_2D, gl->tex);
    SDL_Log("rc5");
#ifdef GLFONTSTASH_IMPLEMENTATION_ES2
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, gl->width, gl->height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, gl->width, gl->height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    static GLint swizzleRgbaParams[4] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRgbaParams);
#endif
    SDL_Log("rc6");
    return 1;
}



void __main_renderDelete(void *p)
{
    GLcontext *gl=(GLcontext*)p;
    if (gl->tex != 0) {
        glDeleteTextures(1, &gl->tex);
        gl->tex = 0;
    }

#ifndef GLFONTSTASH_IMPLEMENTATION_ES2
    glBindVertexArray(0);
#endif

    if (gl->vertexBuffer != 0) {
        glDeleteBuffers(1, &gl->vertexBuffer);
        gl->vertexBuffer = 0;
    }

    if (gl->tcoordBuffer != 0) {
        glDeleteBuffers(1, &gl->tcoordBuffer);
        gl->tcoordBuffer = 0;
    }

    if (gl->colorBuffer != 0) {
        glDeleteBuffers(1, &gl->colorBuffer);
        gl->colorBuffer = 0;
    }

#ifndef GLFONTSTASH_IMPLEMENTATION_ES2
    if (gl->vertexArray != 0) {
        glDeleteVertexArrays(1, &gl->vertexArray);
        gl->vertexArray = 0;
    }
#endif

    free(gl);
}














//for web

#include "canvas3d.h"
#include "canvas/shader.h"
#include <stdlib.h>
#include "canvas/window.h"
#include "canvas/mathGL.h"
#include <vec3.h>
#include "objects/Object3d.h"

#if OKPLATFORM_EMSCRIPTEN
#define GL_GLEXT_PROTOTYPES 1
//#include <SDL_opengles2.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "canvas/glad/glad.h"
#include "canvas/glad/khrplatform.h"
#endif



Canvas3d *Canvas3d_create(Window *win)
{

    Canvas3d * c =(Canvas3d *)malloc(sizeof(Canvas3d));




    float  near =0.001;
    float far =10000.0;
    int w;
    int h;
    SDL_GL_GetDrawableSize(win->sdlWindow, &w, &h);

    SDL_Log("DONE LOAD mat_calculate");

    SDL_Log("w= %d h= %d",w,h);
    // matrix_projection2d(&c->projection[0], 0.0f, (GLfloat)w, (GLfloat)h, 0.0f);

    matrix_projection_Perspective(&c->projection[0],45,(float)w/(float)h,near,far);
    matrix_identityE(&c->view[0]);

    //    Object3d *box= Object3d_create(vec3_create(0,0,0),Color_create(0,1,0,1));
    //    Object3d *box2= Object3d_create(vec3_create(0,1,1),Color_create(0,0,1,1));

    Object3d *axis_x= Object3d_create(vec3_create(0,0,0),
                                      Color_create(0.99,0,0.0,1));
    Object3d *axis_y= Object3d_create(vec3_create(0,0,0),
                                      Color_create(0,0.99,0.0,1));
    Object3d *axis_z= Object3d_create(vec3_create(0,0,0),
                                      Color_create(0,0,0.99,1));




    Object3d *grid= Object3d_create(vec3_create(0,0,0),Color_create(0,0,0,0.1));

      Object3d *gridy= Object3d_create(vec3_create(0,0,0),Color_create(0,0,0,0.1));


    map_init(&c->objects);



    map_set(&c->objects, "gridx", grid);
     map_set(&c->objects, "gridy", gridy);

    map_set(&c->objects, "axis_x", axis_x);
    map_set(&c->objects, "axis_y", axis_y);
    map_set(&c->objects, "axis_z", axis_z);



    //Object3d_axis(axis,10.0);

    Object3d_Line(axis_x,vec3_create(0,0,0),vec3_create(10,0,0));
    Object3d_Line(axis_y,vec3_create(0,0,0),vec3_create(0,10,0));
    Object3d_Line(axis_z,vec3_create(0,0,0),vec3_create(0,0,10));


    Object3d_Grid(grid,1,vec3_create(1000,1000,1000));
    Object3d_Grid(gridy,1,vec3_create(1000,1000,1000));





    glGenBuffers(1, &c->VBO);
    glGenBuffers(1, &c->EBO);


    const char *v= ""
                   "uniform mat4 projection;"
                   "uniform mat4 view;"
                   "uniform mat4 model;"
                   "attribute vec3 Position;"
                   "void main()"
                   "{"
                   "gl_Position = projection* view* model* vec4(Position.xyz, 1);"
                   "}";
    const char *f = ""
                    "uniform vec4 color;"
                    "void main()"
                    "{"
                    "   gl_FragColor = vec4(color.rgba );"
                    "}";

    c->shader=shader_linkProgram1(v,f,win);
    //SDL_Log("DONE LOAD tx1");


    c->attrib_pos = glGetAttribLocation(c->shader, "Position");
    c->uniform_projection_matrix= glGetUniformLocation(c->shader, "projection");
    c->uniform_view_matrix = glGetUniformLocation(c->shader, "view");
    c->uniform_model_matrix = glGetUniformLocation(c->shader, "model");
    c->uniform_color =glGetUniformLocation(c->shader, "color");

    return c;
}



void Canvas3d_free(Canvas3d *c)
{
    const char *key;
    map_iter_t iter = map_iter(&c->objects );
    while ((key = map_next(&c->objects, &iter)))
    {
        Object3d* b= (Object3d* )*map_get(&c->objects, key);
        Object3d_free(b);

    }
    map_deinit(&c->objects);

    //
    glDeleteProgram(c->shader);
    //glDeleteTextures(1, &font_tex);
    glDeleteBuffers(1, &c->VBO);
    glDeleteBuffers(1, &c->EBO);


    free(c);
}


void Canvas3d_render(Canvas3d *c)
{

    //    glClearColor(1.0, 1.0, 1.0, 1.0);
    //    glClearStencil(0);
    // glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    glUseProgram(c->shader);
    glUniformMatrix4fv(c->uniform_view_matrix, 1, GL_FALSE, &c->view[0]);
    glUniformMatrix4fv(c->uniform_projection_matrix, 1, GL_FALSE, &c->projection[0]);


    //glUniform1i(uniform_mytexture, /*GL_TEXTURE*/0);
    glEnableVertexAttribArray(c->attrib_pos);
    // Describe our vertices array to OpenGL (it can't guess its format automatically)
    glBindBuffer(GL_ARRAY_BUFFER,c->VBO);




    const char *key;
    map_iter_t iter = map_iter(&c->objects );
    while ((key = map_next(&c->objects, &iter)))
    {
        Object3d* b= (Object3d* )*map_get(&c->objects, key);



        if(b->vertices!=NULL && b->elements!=NULL)
        {

            glUniformMatrix4fv(c->uniform_model_matrix, 1, GL_FALSE, & b->model[0]);
            glUniform4fv(c->uniform_color, 1,  b->color);


            glBufferData(GL_ARRAY_BUFFER, b->vertices_size, b->vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(c->attrib_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, b->elements_size,b->elements, GL_STATIC_DRAW);

            //            int size;
            //            glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
            //            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            switch (b->style)
            {
            case LINE:
                glDrawElements(GL_LINES, b->elements_size / sizeof(GLushort), GL_UNSIGNED_SHORT, (void*)0);
                break;
            case POLY:
                glDrawElements(GL_TRIANGLES, b->elements_size / sizeof(GLushort), GL_UNSIGNED_SHORT, (void*)0);
                break;
            case LINE_LOOP:
                glDrawElements(GL_LINE_LOOP, b->elements_size / sizeof(GLushort), GL_UNSIGNED_SHORT, (void*)0);
                break;
            default:
                glDrawElements(GL_TRIANGLES,  b->elements_size / sizeof(GLushort), GL_UNSIGNED_SHORT, (void*)0);
                break;
            }
        }

        // count++;
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glDisableVertexAttribArray(c->attrib_pos);



}

void Canvas3d_size_changed(Canvas3d *c,int w,int h)
{
    // This creates a coordinate system where one pixel is one unit and where (0, 0) is the top left corner.
    glViewport(0, 0,(GLfloat) w,(GLfloat)h);
    matrix_projection_Perspective(c->projection,45,(float)w/(float)h,0.00001,10000);
}

void Canvas3d_add_object(Canvas3d *c, Object3d *e, const char *name)
{
    map_set(&c->objects, name, e);
}


int Canvas3d_delete_object(Canvas3d *c,  const char *name)
{

    map_remove(&c->objects, name);

}


Object3d* Canvas3d_GetObject(Canvas3d *c,const char *key)
{
    Object3d* b= *map_get(&c->objects, key);
    if(b==NULL)
    {
        return NULL;
    }
    else
    {
        return b;
    }

}


















