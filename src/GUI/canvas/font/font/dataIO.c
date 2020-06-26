#include "DataIO.h"


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>


DATAIO_API int dataIO_loadBinaryFile(const char* fileName, uint8_t** dataOut)
{
    *dataOut = NULL;

    SDL_RWops* rw = SDL_RWFromFile(fileName, "rb");
    if (!rw) {
        return -1;
    }

    // Limiting to int size for now. No massive files.
    int fileSize = (int) SDL_RWsize(rw);
    *dataOut = (uint8_t*) malloc((size_t) fileSize);

    int totalRead = 0;
    uint8_t* buf = *dataOut;
    while (totalRead < fileSize) {
        Sint64 read = SDL_RWread(rw, buf, 1, (size_t) (fileSize - totalRead));
        if (read == 0) {
            break;
        }
        totalRead += (int) read;
        buf += read;
    }
    SDL_RWclose(rw);
    if (totalRead != fileSize) {
        free(dataOut);
        *dataOut = NULL;
        return -1;
    }

    return totalRead;
}



DATAIO_API char* dataIO_loadTextFile(const char* fileName) {

    SDL_RWops* rw = SDL_RWFromFile(fileName, "r");
    if (!rw) {
        return NULL;
    }

    // Limiting to int size for now. No massive files.
    int fileSize = (int) SDL_RWsize(rw);
    char* dataOut = (char*) malloc((size_t) (fileSize + 1));

    int totalRead = 0;
    char* buf = dataOut;
    while (totalRead < fileSize) {
        Sint64 read = SDL_RWread(rw, buf, 1, (size_t) (fileSize - totalRead));
        if (read == 0) {
            break;
        }
        totalRead += (int) read;
        buf += read;
    }
    SDL_RWclose(rw);
    if (totalRead != fileSize) {
        free(dataOut);
        return NULL;
    }

    dataOut[totalRead] = '\0';
    return dataOut;
}
