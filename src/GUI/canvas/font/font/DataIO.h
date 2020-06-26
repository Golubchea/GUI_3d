#ifndef DATAIO_H
#define DATAIO_H




#ifdef __cplusplus
extern "C" {
#endif

#include "SDL.h"






#ifdef DATAIO_STATIC
#define DATAIO_API static
#else
#define DATAIO_API extern
#endif



DATAIO_API int dataIO_loadBinaryFile(const char* fileName, uint8_t** dataOut);
DATAIO_API char* dataIO_loadTextFile(const char* fileName) ;


#ifdef __cplusplus
}
#endif


#endif // ifndef
