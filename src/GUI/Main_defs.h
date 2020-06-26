#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifdef MAIN_DEF_STATIC
#define MAIN_DEF_API static
#else
#define MAIN_DEF_API extern
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef struct Mouse
{
    int x;
    int y;
}Mouse;

static Mouse * Mouse_create()
{
    Mouse* me=(Mouse*)malloc(sizeof (Mouse));
    return me;
}
static  void Mouse_free(Mouse *m)
{
    free(m);
}

static unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return (r) | (g << 8) | (b << 16) | (a << 24);
}
#ifdef __cplusplus
}
#endif
