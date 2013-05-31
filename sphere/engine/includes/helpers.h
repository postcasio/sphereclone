#ifndef HELPERS_H_
#define HELPERS_H_

#include <v8.h>

#define V8_FUNC(func) v8::FunctionTemplate::New(func)->GetFunction()

#ifndef DEBUG
#define DEBUG 0
#else
#define DEBUG 1
#endif

#define debug(str) if (DEBUG) { printf(str); }

#ifdef _WIN32

#define ALIGNED _declspec(align(2))

#else

#define ALIGNED __attribute__((aligned(2)))

#endif

#define RGBA_RMASK 0x000000FF
#define RGBA_GMASK 0x0000FF00
#define RGBA_BMASK 0x00FF0000
#define RGBA_AMASK 0xFF000000

#endif