//#include "canvas3d.h"
//#include "canvas/shader.h"
//#include <stdlib.h>
//#include "canvas/window.h"
//#include "canvas/mathGL.h"
//#include <vec3.h>
//#include "objects/Object3d.h"

//#if OKPLATFORM_EMSCRIPTEN
//#define GL_GLEXT_PROTOTYPES 1
////#include <SDL_opengles2.h>
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
//#else
//#include "canvas/glad/glad.h"
//#include "canvas/glad/khrplatform.h"
//#endif



//Canvas3d *Canvas3d_create(Window *win)
//{

//    Canvas3d * c =(Canvas3d *)malloc(sizeof(Canvas3d));




//    float  near =0.001;
//    float far =10000.0;
//    int w;
//    int h;
//    SDL_GL_GetDrawableSize(win->sdlWindow, &w, &h);

//    SDL_Log("DONE LOAD mat_calculate");

//    SDL_Log("w= %d h= %d",w,h);
//    // matrix_projection2d(&c->projection[0], 0.0f, (GLfloat)w, (GLfloat)h, 0.0f);

//    matrix_projection_Perspective(&c->projection[0],45,(float)w/(float)h,near,far);
//    matrix_identity(&c->view[0]);

//    //    Object3d *box= Object3d_create(vec3_create(0,0,0),Color_create(0,1,0,1));
//    //    Object3d *box2= Object3d_create(vec3_create(0,1,1),Color_create(0,0,1,1));

//    Object3d *axis= Object3d_create(vec3_create(0,0,0),Color_create(0,1,0,1));
//    Object3d *grid= Object3d_create(vec3_create(0,0,0),Color_create(0,0,0,0.5));

//    map_init(&c->objects);



//    map_set(&c->objects, "grid1", grid);
//    map_set(&c->objects, "axis1", axis);


//    Object3d_axis(axis,10.0);
//    Object3d_Grid(grid,1,vec3_create(10,10,10));





//    glGenBuffers(1, &c->VBO);
//    glGenBuffers(1, &c->EBO);


//    const char *v= ""
//                   "uniform mat4 projection;"
//                   "uniform mat4 view;"
//                   "uniform mat4 model;"
//                   "attribute vec3 Position;"
//                   "void main()"
//                   "{"
//                   "gl_Position = projection* view* model* vec4(Position.xyz, 1);"
//                   "}";
//    const char *f = ""
//                    "uniform vec4 color;"
//                    "void main()"
//                    "{"
//                    "   gl_FragColor = vec4(color.rgba );"
//                    "}";

//    c->shader=shader_linkProgram1(v,f,win);
//    //SDL_Log("DONE LOAD tx1");


//    c->attrib_pos = glGetAttribLocation(c->shader, "Position");
//    c->uniform_projection_matrix= glGetUniformLocation(c->shader, "projection");
//    c->uniform_view_matrix = glGetUniformLocation(c->shader, "view");
//    c->uniform_model_matrix = glGetUniformLocation(c->shader, "model");
//    c->uniform_color =glGetUniformLocation(c->shader, "color");

//    return c;
//}



//void Canvas3d_free(Canvas3d *c)
//{
//    const char *key;
//    map_iter_t iter = map_iter(&c->objects );
//    while ((key = map_next(&c->objects, &iter)))
//    {
//        Object3d* b= (Object3d* )*map_get(&c->objects, key);
//        Object3d_free(b);

//    }
//    map_deinit(&c->objects);

//    //
//    glDeleteProgram(c->shader);
//    //glDeleteTextures(1, &font_tex);
//    glDeleteBuffers(1, &c->VBO);
//    glDeleteBuffers(1, &c->EBO);


//    free(c);
//}


//void Canvas3d_render(Canvas3d *c)
//{

//    //    glClearColor(1.0, 1.0, 1.0, 1.0);
//    //    glClearStencil(0);
//    // glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

//    glUseProgram(c->shader);
//    glUniformMatrix4fv(c->uniform_view_matrix, 1, GL_FALSE, &c->view[0]);
//    glUniformMatrix4fv(c->uniform_projection_matrix, 1, GL_FALSE, &c->projection[0]);


//    //glUniform1i(uniform_mytexture, /*GL_TEXTURE*/0);
//    glEnableVertexAttribArray(c->attrib_pos);
//    // Describe our vertices array to OpenGL (it can't guess its format automatically)
//    glBindBuffer(GL_ARRAY_BUFFER,c->VBO);




//    const char *key;
//    map_iter_t iter = map_iter(&c->objects );
//    while ((key = map_next(&c->objects, &iter)))
//    {
//        Object3d* b= (Object3d* )*map_get(&c->objects, key);



//        if(b->vertices!=NULL && b->elements!=NULL)
//        {

//            glUniformMatrix4fv(c->uniform_model_matrix, 1, GL_FALSE, & b->model[0]);
//            glUniform4fv(c->uniform_color, 1,  b->color);


//            glBufferData(GL_ARRAY_BUFFER, b->vertices_size, b->vertices, GL_STATIC_DRAW);
//            glVertexAttribPointer(c->attrib_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

//            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->EBO);
//            glBufferData(GL_ELEMENT_ARRAY_BUFFER, b->elements_size,b->elements, GL_STATIC_DRAW);

//            //            int size;
//            //            glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
//            //            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//            switch (b->style)
//            {
//            case LINE:
//                glDrawElements(GL_LINES, b->elements_size / sizeof(GLushort), GL_UNSIGNED_SHORT, (void*)0);
//                break;
//            case POLY:
//                glDrawElements(GL_TRIANGLES, b->elements_size / sizeof(GLushort), GL_UNSIGNED_SHORT, (void*)0);
//                break;
//            case LINE_LOOP:
//                glDrawElements(GL_LINE_LOOP, b->elements_size / sizeof(GLushort), GL_UNSIGNED_SHORT, (void*)0);
//                break;
//            default:
//                glDrawElements(GL_TRIANGLES,  b->elements_size / sizeof(GLushort), GL_UNSIGNED_SHORT, (void*)0);
//                break;
//            }
//        }

//        // count++;
//    }
//    glBindBuffer(GL_ARRAY_BUFFER,0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
//    glDisableVertexAttribArray(c->attrib_pos);



//}

//void Canvas3d_size_changed(Canvas3d *c,int w,int h)
//{
//    // This creates a coordinate system where one pixel is one unit and where (0, 0) is the top left corner.
//    glViewport(0, 0,(GLfloat) w,(GLfloat)h);
//    matrix_projection_Perspective(c->projection,45,(float)w/(float)h,0.00001,10000);
//}

//void Canvas3d_add_object(Canvas3d *c, Object3d *e, const char *name)
//{
//     map_set(&c->objects, name, e);
//}



//Object3d* Canvas3d_GetObject(Canvas3d *c,const char *key)
//{
//    Object3d* b= *map_get(&c->objects, key);
//    if(b==NULL)
//    {
//        return NULL;
//    }
//    else
//    {
//        return b;
//    }

//}




