#include "Font_Atlas.h"
#include <stdlib.h>
#include <stdint.h>

void *memSet(void *s, int c, size_t n)
{

    unsigned char* p=(unsigned char*)s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}

FONTS_API int font_atlas_InsertNode(Font_atlas* atlas, int idx, int x, int y, int w)
{
    int i;
    // Insert node
    if (atlas->nnodes+1 > atlas->cnodes) {
        atlas->cnodes = atlas->cnodes == 0 ? 8 : atlas->cnodes * 2;
        atlas->nodes = (Font_atlasNode*)realloc(atlas->nodes, sizeof(Font_atlasNode) * atlas->cnodes);
        if (atlas->nodes == NULL)
            return 0;
    }
    for (i = atlas->nnodes; i > idx; i--)
        atlas->nodes[i] = atlas->nodes[i-1];
    atlas->nodes[idx].x = (short)x;
    atlas->nodes[idx].y = (short)y;
    atlas->nodes[idx].width = (short)w;
    atlas->nnodes++;

    return 1;
}

FONTS_API void font_atlas_RemoveNode(Font_atlas* atlas, int idx)
{
    int i;
    if (atlas->nnodes == 0) return;
    for (i = idx; i < atlas->nnodes-1; i++)
        atlas->nodes[i] = atlas->nodes[i+1];
    atlas->nnodes--;
}

FONTS_API void font_atlas_Expand(Font_atlas* atlas, int w, int h)
{
    // Insert node for empty space
    if (w > atlas->width)
        font_atlas_InsertNode(atlas, atlas->nnodes, atlas->width, 0, w - atlas->width);
    atlas->width = w;
    atlas->height = h;
}

FONTS_API void font_atlas_Reset(Font_atlas * atlas, int w, int h)
{
    atlas->width = w;
    atlas->height = h;
    atlas->nnodes = 0;

    // Init root node.
    atlas->nodes[0].x = 0;
    atlas->nodes[0].y = 0;
    atlas->nodes[0].width = (short)w;
    atlas->nnodes++;
}

FONTS_API int font_atlas_AddSkylineLevel(Font_atlas* atlas, int idx, int x, int y, int w, int h)
{
    int i;

    // Insert new node
    if (font_atlas_InsertNode(atlas, idx, x, y+h, w) == 0)
        return 0;

    // Delete skyline segments that fall under the shadow of the new segment.
    for (i = idx+1; i < atlas->nnodes; i++) {
        if (atlas->nodes[i].x < atlas->nodes[i-1].x + atlas->nodes[i-1].width) {
            int shrink = atlas->nodes[i-1].x + atlas->nodes[i-1].width - atlas->nodes[i].x;
            atlas->nodes[i].x += (short)shrink;
            atlas->nodes[i].width -= (short)shrink;
            if (atlas->nodes[i].width <= 0) {
                font_atlas_RemoveNode(atlas, i);
                i--;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    // Merge same height skyline segments that are next to each other.
    for (i = 0; i < atlas->nnodes-1; i++) {
        if (atlas->nodes[i].y == atlas->nodes[i+1].y) {
            atlas->nodes[i].width += atlas->nodes[i+1].width;
            font_atlas_RemoveNode(atlas, i+1);
            i--;
        }
    }

    return 1;
}

FONTS_API int min_i(int a, int b)
{
    return a < b ? a : b;
}

FONTS_API int max_i(int a, int b)
{
    return a > b ? a : b;
}


FONTS_API int font_atlas_RectFits(Font_atlas* atlas, int i, int w, int h)
{
    // Checks if there is enough space at the location of skyline span 'i',
    // and return the max height of all skyline spans under that at that location,
    // (think tetris block being dropped at that position). Or -1 if no space found.
    int x = atlas->nodes[i].x;
    int y = atlas->nodes[i].y;
    int spaceLeft;
    if (x + w > atlas->width)
        return -1;
    spaceLeft = w;
    while (spaceLeft > 0) {
        if (i == atlas->nnodes) return -1;
        y = max_i(y, atlas->nodes[i].y);
        if (y + h > atlas->height) return -1;
        spaceLeft -= atlas->nodes[i].width;
        ++i;
    }
    return y;
}

FONTS_API int font_atlas_AddRect(Font_atlas* atlas, int rw, int rh, int* rx, int* ry)
{
    int besth = atlas->height, bestw = atlas->width, besti = -1;
    int bestx = -1, besty = -1, i;

    // Bottom left fit heuristic.
    for (i = 0; i < atlas->nnodes; i++) {
        int y = font_atlas_RectFits(atlas, i, rw, rh);
        if (y != -1) {
            if (y + rh < besth || (y + rh == besth && atlas->nodes[i].width < bestw)) {
                besti = i;
                bestw = atlas->nodes[i].width;
                besth = y + rh;
                bestx = atlas->nodes[i].x;
                besty = y;
            }
        }
    }

    if (besti == -1)
        return 0;

    // Perform the actual packing.
    if (font_atlas_AddSkylineLevel(atlas, besti, bestx, besty, rw, rh) == 0)
        return 0;

    *rx = bestx;
    *ry = besty;

    return 1;
}

void font_atlas_free(Font_atlas *atlas)
{
    if (atlas == NULL) return;
    if (atlas->nodes != NULL) free(atlas->nodes);
    free(atlas);
}

Font_atlas *font_atlas_create(int w, int h, int nnodes)
{
    Font_atlas* atlas = NULL;

    // Allocate memory for the font stash.
    atlas = (Font_atlas*)malloc(sizeof(Font_atlas));
    if (atlas == NULL) goto error;
     memSet(atlas, 0, sizeof(Font_atlas));

    atlas->width = w;
    atlas->height = h;

    // Allocate space for skyline nodes
    atlas->nodes = (Font_atlasNode*)malloc(sizeof(Font_atlasNode) * nnodes);
    if (atlas->nodes == NULL) goto error;
     memSet(atlas->nodes, 0, (int32_t)(sizeof(Font_atlasNode) * nnodes)  );
    atlas->nnodes = 0;
    atlas->cnodes = nnodes;

    // Init root node.
    atlas->nodes[0].x = 0;
    atlas->nodes[0].y = 0;
    atlas->nodes[0].width = (short)w;
    atlas->nnodes++;

    return atlas;

error:
    if (atlas) font_atlas_free(atlas);
    return NULL;
}
