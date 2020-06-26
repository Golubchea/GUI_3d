#include <vec3.h>

#pragma once
#define UNPROJECT_API extern


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

///
/// \brief unproject
/// range [0:width, height:0]
/// \param mouse_x
/// \param mouse_y
/// \param width
/// \param height
/// \return
///
UNPROJECT_API vec3 unproject(int mouse_x ,int mouse_y,int width,int height,float *projection,float *view);

UNPROJECT_API vec3 unproject2(int mouse_x, int mouse_y, int width, int height ,float *projection,float *view);
#ifdef __cplusplus
}
#endif // __cplusplus

