

#include "rtsp_menu.h"
#include "unprojectPlaneMenu.h"
#include "gizmo/tiny-gizmo.hpp"
#include "ray_collision.h"
#define VER_MAJOR 0
#define VER_MINOR 1
void mainLoop();

//base resource data
mctx ctx;


Object3d *b=NULL;
Object3d *selected=NULL;
std::shared_ptr<gizmo_t>   gizmo=nullptr;

int main(int argc, char ** argv)
{
    int sum = 0, x = 1, y = 2;
    asm ( "add %1, %0" : "=r" (sum) : "r" (x), "0" (y) ); // sum = x + y;
    printf("sum = %d, x = %d, y = %d", sum, x, y); // sum = 3, x = 1, y = 2

    ctx.canvas=Canvas_create( "background.png");
    ctx.button_controller=Button_controller_create();
    ctx.mouse=Mouse_create();
    //initialize all gui
    ctx.menu=Menu_create(ctx.button_controller,19,ctx.canvas, ctx.mouse);




    Button *b8=Button_create("load",31,5,3,30,25,RGBA(0,255,47,255),load );
    b8->data=&ctx;
    Button_setIcon(b8,"\uf115");
    b8->drawBorder=0;
    b8->iconOnly=true;
    Button_controller_add(ctx.button_controller,b8);

    Button *b7=Button_create("save",31,45,3,30,25,RGBA(0,255,47,255),save );
    Button_setIcon(b7,"\ue810");
    b7->data=&ctx;
    b7->drawBorder=0;
    b7->iconOnly=true;
    Button_controller_add(ctx.button_controller,b7);

    Button *b_video=Button_create("b_video",31,85,3,30,25,RGBA(0,255,47,255),streamVC );
    b_video->data=&ctx;
    Button_setIcon(b_video,"\ue806");
    b_video->drawBorder=0;
    b_video->iconOnly=true;
    Button_controller_add(ctx.button_controller,b_video);


    //    Button *b_solve=Button_create("b_solve",31,135,3,30,25,RGBA(0,255,47,255),);
    //    b_solve->data=&ctx;
    //    Button_setIcon(b_video,"\ue802");
    //    b_solve->drawBorder=0;
    //    b_solve->iconOnly=true;
    //    Button_controller_add(ctx.button_controller,b_solve);



    Button *bconsole1=Button_create("console",19,ctx.canvas->width-390,ctx.canvas->height-35,70,25,RGBA(0,0,0,255),camera_getCurrentPosition );
    bconsole1->data=&ctx;
    Button_setIcon(bconsole1,"консоль");
    bconsole1->drawBorder=0;
    bconsole1->iconOnly=false;
    bconsole1->textIsIcon=1;
    Button_controller_add(ctx.button_controller,bconsole1);

    Button *bconsole2=Button_create("console2",19,ctx.canvas->width-390,ctx.canvas->height-20,70,25,RGBA(0,0,0,255),camera_getCurrentPosition );
    bconsole2->data=&ctx;
    Button_setIcon(bconsole2,"версия 0.1");
    bconsole2->drawBorder=0;
    bconsole2->iconOnly=false;
    bconsole2->textIsIcon=1;
    Button_controller_add(ctx.button_controller,bconsole2);




    //init 3d
    ctx.canvas3d=Canvas3d_create(ctx.canvas->window);
    ctx.camera=Camera_create( vec3_create(2.0f, 2.96f, 5.13f),vec2_create(ctx.canvas->width,ctx.canvas->height),
                              vec3_create(0.0f, 1.0f, 0.0f),-90.0f-25.0f,-23.19);

    Menu_unproject_create( ctx.canvas,ctx.camera,ctx.button_controller,ctx.canvas3d);


    b=Object3d_create(vec3_create(0,0.015,0),Color_create(1,0,0.0,0.2));
    b->style=STYLE::LINE_LOOP;
    Object3d_Cube(b,vec3_create(2.35,0.015,2.35));
    Canvas3d_add_object(ctx.canvas3d, b,"rotation_cube" );

    Object3d   *b2=Object3d_create(vec3_create(0.8,0.0,0.3),Color_create(0,1,0.0,0.2));
    b2->style=STYLE::LINE_LOOP;
    Object3d_Cube(b2,vec3_create(0.35,0.35,0.35));
    Canvas3d_add_object(ctx.canvas3d, b2,"sample_cube1" );


    //////////////////////////////gizmo



    gizmo= make_shared<gizmo_t>(ctx.canvas3d);
    gizmo->state.hotkey_ctrl=true;

    gizmo->ctx.render=[&](const geometry_mesh & r)
    {

        gizmo->gizmo3d->color[0]=0.0;
        gizmo->gizmo3d->color[1]=1;
        gizmo->gizmo3d->color[2]=0.0;
        gizmo->gizmo3d->color[3]=0.2;

        for (size_t i=0;i<r.vertices.size();++i)
        {
            gizmo->data_v.push_back(r.vertices[i].position.x);
            gizmo->data_v.push_back(r.vertices[i].position.y);
            gizmo->data_v.push_back(r.vertices[i].position.z);
        }
        for (size_t i=0;i<r.triangles.size();++i)
        {
            gizmo->data_i.push_back(r.triangles[i].x);
            gizmo->data_i.push_back(r.triangles[i].y);
            gizmo->data_i.push_back(r.triangles[i].z);
        }
        gizmo->gizmo3d->vertices=gizmo->data_v.data();
        gizmo->gizmo3d->vertices_size=gizmo->data_v.size()*sizeof (float);
        gizmo->gizmo3d->elements=gizmo->data_i.data();
        gizmo->gizmo3d->elements_size=gizmo->data_i.size()*sizeof (unsigned short);
    };

    Object3d *hit= Object3d_create(vec3_create(0,0,0),Color_create(1.0,0.0,0,0.9));
    Canvas3d_add_object(ctx.canvas3d,hit,"hit");
    Object3d_Cube(hit,vec3_create(0.05,0.05,0.05));
    hit->style=STYLE::LINE_LOOP;


    //////////////////////////////
    SDL_Log("-----window, shaders created system APP: %d mb",SDL_GetSystemRAM());
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop,0, true);
#else
    while (ctx.canvas->window->isRunning)
    {
        mainLoop();
    }
#endif
    gizmo=nullptr;

    Menu_free(ctx.menu);

    Camera_free(ctx.camera);
    Canvas3d_free(ctx.canvas3d);
    Button_controller_free(ctx.button_controller);
    Canvas_free(ctx.canvas);
    return EXIT_SUCCESS;
}


//template<typename T, qualifier Q>
//GLM_FUNC_QUALIFIER vec<2, T, Q> closestPointOnLine
//(
//    vec<2, T, Q> const& point,
//    vec<2, T, Q> const& a,
//    vec<2, T, Q> const& b
//)
//{
//    T LineLength = distance(a, b);
//    vec<2, T, Q> Vector = point - a;
//    vec<2, T, Q> LineDirection = (b - a) / LineLength;

//    // Project Vector to LineDirection to get the distance of point from a
//    T Distance = dot(Vector, LineDirection);

//    if(Distance <= T(0)) return a;
//    if(Distance >= LineLength) return b;
//    return a + LineDirection * Distance;
//}




void mainLoop()
{

    while (SDL_PollEvent(& ctx.canvas->event))
    {
        if(ctx.isText)
            Camera_processCameraMov(ctx.camera,&ctx.canvas->event);

        switch ( ctx.canvas->event.type)
        {
        case SDL_MOUSEWHEEL:
        {
            ctx.camera->speed+= (float)ctx.canvas->event.wheel.y*0.011f;



            SDL_Log("camera speed: %f", ctx.camera->speed);
            if(ctx.isText)
                Camera_ProcessMouseScroll(ctx.camera,(float)ctx.canvas->event.wheel.y); ///????
            break;
        }
        case SDL_MOUSEMOTION:
        {
            ctx.mouse->x=ctx.canvas->event.motion.x;
            ctx.mouse->y=ctx.canvas->event.motion.y;


            if(ctx.isText)
            {
                Camera_listen(ctx.camera,&ctx.canvas->event);
                ctx.canvas3d->kx=  compute_unproject_koefficients_x(ctx.canvas3d->projection,ctx.canvas3d->view);
                ctx.canvas3d->ky=  compute_unproject_koefficients_y(ctx.canvas3d->projection,ctx.canvas3d->view);
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            if(ctx.canvas->event.key.keysym.sym == SDLK_DOWN)
            {
                ctx.camera->Zoom=ctx.camera->Zoom-0.1;
                float  near =0.001;
                float far =100000.0;
                matrix_projection_Perspective(&ctx.canvas3d->projection[0],ctx.camera->Zoom,
                        ctx.canvas->width/ctx.canvas->height,near,far);
                SDL_Log("fov: %lf",ctx.camera->Zoom);
                break;
            }

            if ( ctx.canvas->event.key.keysym.sym == SDLK_ESCAPE)
            {
                goto quit;
                break;
            }
            if ( ctx.canvas->event.key.keysym.sym == SDLK_F12)
            {
                window_screencapture(ctx.canvas->window);
                break;
            }
            if ( ctx.canvas->event.key.keysym.sym == SDLK_INSERT)
            {
                ctx.canvas->needToRedraw=!ctx.canvas->needToRedraw;
                SDL_Log( "need to redraw %d",ctx.canvas->needToRedraw);
                break;
            }

#ifndef __EMSCRIPTEN__
            if ( ctx.canvas->event.key.keysym.sym == SDLK_F9)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            }
            if ( ctx.canvas->event.key.keysym.sym == SDLK_F10)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            }
#endif
            if ( ctx.canvas->event.key.keysym.sym == SDLK_RETURN)
            {
                textInputStop(&ctx,NULL,NULL,NULL);
                break;
            }
            if( ctx.canvas->event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
            {


                ctx.selected->icon_size=strCat2(ctx.selected->icon,SDL_GetClipboardText());
                break;
            }
            if( ctx.canvas->event.key.keysym.sym == SDLK_i )
            {
                matrix_rotate4x4_y_float(b->model,90);

                getViewOfObject(ctx.camera,b->model);
                matrix_rotate4x4_y_float(b->model,-90);
            }
            if( ctx.canvas->event.key.keysym.sym == SDLK_o )
            {
                matrix_rotate4x4_around_axis_float(b->model,vec3_create(0,1,0), 180);
                matrix_rotate4x4_x_float(b->model,-90);


                getViewOfObject(ctx.camera,b->model);
                matrix_rotate4x4_x_float(b->model,90);
                matrix_rotate4x4_around_axis_float(b->model,vec3_create(0,1,0),-180);

            }


            if ( ctx.canvas->event.key.keysym.sym == SDLK_BACKSPACE)
            {


                if(ctx.selected!=NULL)
                {
                    if(ctx.selected->icon_size>=0)
                    {
                        if(ctx.selected->textIsIcon)
                        {

                            ctx.selected->icon[ctx.selected->icon_size]=0;
                            ctx.selected->icon_size--;
                        }
                    }
                }

                break;
            }
            if ( ctx.canvas->event.key.keysym.sym == SDLK_f)
            {
                Object3d *b= Canvas3d_GetObject(ctx.canvas3d,"cam_pnp0");

                getViewOfObjectRoll(ctx.camera,b->model, M_PI/2.0 );




                break;
            }
            if ( ctx.canvas->event.key.keysym.sym == SDLK_g)
            {


                Object3d *axis= Object3d_create(vec3_create(0,0,0),
                                                Color_create(0,0.99,0.0,1));
                Canvas3d_add_object(ctx.canvas3d,axis,("found_target"+std::to_string(0)).c_str());

                vec3 front= vec3_scale2(ctx.camera->Front,3.76);

                vec3 kk=vec3_rotate_around_axis(front,ctx.camera->Right, 5.68);
                kk=vec3_rotate_around_axis(kk,ctx.camera->Up,  6.08);



                vec3 ray=vec3_add(&ctx.camera->Position,&kk);
                Object3d_Line(axis,ctx.camera->Position,ray);

                Object3d *cube= Object3d_create(ray,Color_create(0.99,0.0,0.0,1));
                cube->style=LINE_LOOP;
                Canvas3d_add_object(ctx.canvas3d,cube,("found_cube"+std::to_string(0)).c_str());
                Object3d_Cube(cube,vec3_create(0.02,0.02,0.02));

                break;
            }


            if ( ctx.canvas->event.key.keysym.sym == SDLK_t)
            {

                gizmo->state.hotkey_translate=true;
                gizmo->state.hotkey_rotate=false;
                gizmo->state.hotkey_scale=false;
                break;
            }
            if ( ctx.canvas->event.key.keysym.sym == SDLK_r)
            {
                gizmo->state.hotkey_translate=false;
                gizmo->state.hotkey_rotate=true;
                gizmo->state.hotkey_scale=false;
                break;
            }
            if ( ctx.canvas->event.key.keysym.sym == SDLK_c)
            {
                gizmo->state.hotkey_translate=false;
                gizmo->state.hotkey_rotate=false;
                gizmo->state.hotkey_scale=true;
                break;
            }
            if ( ctx.canvas->event.key.keysym.sym == SDLK_q)
            {
                gizmo->state.snap_translation=!gizmo->state.snap_translation;
                gizmo->state.snap_rotation=!gizmo->state.snap_rotation;
                gizmo->state.snap_scale=!gizmo->state.snap_scale;

                break;
            }
            if ( ctx.canvas->event.key.keysym.sym == SDLK_e)
            {
                gizmo->state.hotkey_local=!gizmo->state.hotkey_local;

                break;
            }




        }
        case SDL_MOUSEBUTTONDOWN:
        {
            if( ctx.canvas->event.button.button==SDL_BUTTON_LEFT )
            {
                ctx.selected=Button_controller_pressed(ctx.button_controller,ctx.mouse->x,ctx.mouse->y);
                mouseButtonDown(&ctx);
                gizmo->state.mouse_left =true;
            }
            else if( ctx.canvas->event.button.button==SDL_BUTTON_RIGHT )
            {
                ctx.c_menu=Context_menu_create(ctx.button_controller,ctx.mouse->x,ctx.mouse->y);

            }
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            if( ctx.canvas->event.button.button==SDL_BUTTON_LEFT )
            {
                Button_controller_released(ctx.button_controller,ctx.mouse->x,ctx.mouse->y);
                gizmo->state.mouse_left =false;
            }
            else if( ctx.canvas->event.button.button==SDL_BUTTON_RIGHT )
            {
                Context_menu_free(ctx.c_menu,ctx.button_controller);

            }
            // rmenu->selected=NULL;
            break;
        }
        case SDL_TEXTINPUT:
        {
            if (ctx.selected!=NULL)
            {
                if(ctx.selected->textIsIcon && ctx.selected->icon_size_max>ctx.selected->icon_size)
                {

                    SDL_Log("cursor: %d",ctx.selected->icon_size);
                    SDL_Log("max: %d",ctx.selected->icon_size_max);
                    ctx.selected->icon_size=strCat2(ctx.selected->icon,ctx.canvas->event.text.text);

                }
            }
            break;
        }
        case SDL_QUIT:
quit:
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#endif
            SDL_Log( "Event: SDL_QUIT");

            ctx.canvas->window->isRunning=false;
            ctx.canvas->needToRedraw=false;
            SDL_Quit();
            break;
        case SDL_WINDOWEVENT:
        {

            switch ( ctx.canvas->event.window.event)
            {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            {
                float  near =0.001;
                float far =10000.0;
                int oldx=ctx.canvas->width;
                int oldy=ctx.canvas->height;


                canvas_size_changed(ctx.canvas);


                matrix_projection_Perspective(&ctx.canvas3d->projection[0], 45,
                        (float)ctx.canvas->width/(float)ctx.canvas->height,near,far);


                Button_controller_action_resizeWindow(ctx.button_controller,ctx.canvas,oldx,oldy);
            }

                break;
            default:
                break;
            }
        }


        }


    }


    if(ctx.canvas->needToRedraw)
    {





        Camera_SetViewMatrix(ctx.camera,ctx.canvas3d->projection,ctx.canvas3d->view);


        ctx.t= SDL_GetTicks();
        GLint timeLoc = glGetUniformLocation(ctx.canvas->shaders->shaderTextSdf, "time");
        glUniform1f(timeLoc, ctx.t);

        Button_move(ctx.selected,ctx.mouse->x,ctx.mouse->y);
        Button_controller_action_everytime(ctx.button_controller);

        canvas_start_paint2dUI(ctx.canvas);
        glUseProgram(ctx.canvas->shaders->shaderColored);
        ctx.canvas->shader_state=0;
        glUniform1i(ctx.canvas->shaderInSwitcherLoc	,
                    ctx.canvas->shader_state);



        //background image
        canvas_DrawRectangle(ctx.canvas,
                             (float)ctx.canvas->width/2.0-(float)ctx.canvas->width_image/2.0
                             ,(float)ctx.canvas->height/2.0-(float)ctx.canvas->height_image/2.0
                             ,(float)ctx.canvas->width_image,
                             (float)ctx.canvas->height_image,
                             RGBA(100, 255, 100, 255) );



        ctx.canvas->shader_state=1;
        glUniform1i(ctx.canvas->shaderInSwitcherLoc	,
                    ctx.canvas->shader_state);
        glUseProgram(ctx.canvas3d->shader);

        //////////////////////////////


        auto cameraOrientation =  get_orientation(ctx.camera);
        const auto rayDir = get_ray_from_pixel(vec2_create(ctx.mouse->x,ctx.mouse->y),
                                               vec2_create(ctx.canvas->width,ctx.canvas->height),
                                               ctx.camera->Position,
                                               ctx.canvas3d->projection,
                                               ctx.canvas3d->view);

        gizmo->state.viewport_size = minalg::float2(ctx.canvas->width, ctx.canvas->height);
        gizmo->state.cam.near_clip = 0.00001;
        gizmo->state.cam.far_clip = 3200.0;
        gizmo->state.cam.yfov = 45.0;
        gizmo->state.cam.position = minalg::float3(
                    ctx.camera->Position.x,
                    ctx.camera->Position.y,
                    ctx.camera->Position.z);
        gizmo->state.cam.orientation = minalg::float4(
                    cameraOrientation.x,
                    cameraOrientation.y,
                    cameraOrientation.z,
                    cameraOrientation.w);
        gizmo->state.ray_origin = minalg::float3(  ctx.camera->Position.x,
                                                   ctx.camera->Position.y,
                                                   ctx.camera->Position.z);
        gizmo->state.ray_direction = minalg::float3(rayDir.direction.x, rayDir.direction.y, rayDir.direction.z);


        gizmo->ctx.update( gizmo->state);



        if(transform_gizmo("second-example-gizmo", gizmo->ctx,gizmo->r) )
        {
            //    cout<<"----------"<<gizmo->r.position<<"\n";
            // Object3d *obj=Canvas3d_GetObject(ctx.canvas3d,"epnp_test");
            //            Object3d *obj=Canvas3d_GetObject(ctx.canvas3d,"epnp_test");
            //            //            matrix_SetTranslation(obj->model,gizmo->r.position.x,gizmo->r.position.y,gizmo->r.position.z);
            //            //            matrix_Setscale(obj->model,gizmo->r.scale.x,gizmo->r.scale.y,gizmo->r.scale.z);
            minalg::float4x4 t= gizmo->r.matrix();




            convert_matrix(&t,&b->model[0]);

        }



        //matrix_rotate4x4_z_float()

        gizmo->ctx.draw();



        Object3d *hit=Canvas3d_GetObject(ctx.canvas3d,"hit");
        RayHitInfo t;
        Object3d *obj=Canvas3d_GetObject(ctx.canvas3d,"rotation_cube");


            t=GetCollisionRayModel(rayDir ,obj);

            matrix_SetTranslation(hit->model,t.position.x,t.position.y,t.position.z);


        //////////////////////////////
        Canvas3d_render(ctx.canvas3d);
        glUseProgram(ctx.canvas->shaders->shaderColored);


        if(!gizmo->data_i.empty())
            gizmo->data_i.clear();

        if(!gizmo->data_v.empty())
            gizmo->data_v.clear();

        //Button
        Button_controller_drawShapes(ctx.button_controller,ctx.canvas);


        canvas_start_paintText(ctx.canvas);
        {
            Button_controller_drawTexts(ctx.button_controller,ctx.canvas);
            Button_controller_drawIcons(ctx.button_controller,ctx.canvas);
            Font_SetFont(ctx.canvas->font->fs, ctx.canvas->font->fontSdf);



        }
        canvas_end_paint2dUI(ctx.canvas);
    }
    else
    {
        SDL_Delay(20);
        SDL_GL_SwapWindow( ctx.canvas->window->sdlWindow);
    }

}



